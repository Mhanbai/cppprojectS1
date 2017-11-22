#include "networkclass.h"

NetworkClass::NetworkClass()
{
}

NetworkClass::NetworkClass(const NetworkClass &)
{
}

NetworkClass::~NetworkClass()
{
}

bool NetworkClass::Initialize(GraphicsClass* &graphics)
{
	connected = true;
	m_graphics = graphics;
	m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Attempting to establish connection...", 10, 10, 1.0f, 1.0f, 1.0f);

	//Initialize WinSock and check for correct version
	int error = WSAStartup(0x0202, &w);
	if (error != 0)
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not start WinSock", 10, 10, 1.0f, 1.0f, 1.0f);
		connected = false;
		return false;
	}

	if (w.wVersion != 0x0202)
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Wrong WinSock version", 10, 10, 1.0f, 1.0f, 1.0f);
		connected = false;
		return false;
	}

	// Create a UDP socket.
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not create UDP socket", 10, 10, 1.0f, 1.0f, 1.0f);
		connected = false;
		return false;
	}

	// Set socket to non-blocking mode
	unsigned long value = 1;
	ioctlsocket(sock, FIONBIO, &value);

	//Check to see if connected to internet - if not return false with WinSock set up aleady complete
	if (CheckNetwork(myLocalIP, myPublicIP) == false) {
		connected = false;
		return false;
	}

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = inet_addr(myLocalIP);
	// htons converts the port number to network byte order (big-endian).
	listenAddr.sin_port = htons(4444);

	if (bind(sock, (const sockaddr *)&listenAddr, sizeof(listenAddr)) != 0)
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Bind Failed!", 10, 10, 1.0f, 1.0f, 1.0f);
		return false;
	}
	else
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connected to Internet!", 10, 10, 1.0f, 1.0f, 1.0f);
	}
	return true;
}

void NetworkClass::Shutdown()
{
	closesocket(sock);
	WSACleanup();

	return;
}

void NetworkClass::Frame(float time)
{
	totalGameTime = totalGameTime + (time / 1000);

	if (attemptingToEstablish == true) {
		if (messageFromOpponent) {
			m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Successfully established a connection!", 10, 10, 1.0f, 1.0f, 1.0f);
			twoWayConnection = true;
			totalGameTime = 0.0f;
		}
	}

	int count;
	// Send data needed sent
	count = sendto(sock, writeBuffer_, sizeof(NetMessage), 0, (const sockaddr *)&sendAddr, sizeof(sendAddr));
	if (count <= 0) {
		
	}
	else
	{
		writeCount_ -= count;

		// Remove the sent data from the start of the buffer.
		//memmove(writeBuffer_, writeBuffer_ + count, writeCount_);
	}

	// Receive data needing recieved
	int spaceLeft = (sizeof readBuffer_) - readCount_;
	count = recvfrom(sock, readBuffer_, sizeof(NetMessage), 0, &from, &fromlen);
	if (count <= 0) {
		//m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Cannot recieve messages!", 10, 10, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		// We've successfully read some more data into the buffer.
		readCount_ += count;

		if (readCount_ < sizeof NetMessage)
		{
			// ... but we've not received a complete message yet.
			// So we can't do anything until we receive some more.
			return;
		}
		else
		{
			// We've got a complete message.
			ProcessMessage((const NetMessage *)readBuffer_);

			// Clear the buffer, ready for the next message.
			readCount_ = 0;
		}
	}

	return;
}

// This class can be used to complete initialization if CheckNetwork returns false
bool NetworkClass::RecheckNetwork()
{
	if (CheckNetwork(myLocalIP, myPublicIP) == false) {
		connected = false;
		return false;
	}

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = inet_addr(myLocalIP);
	// htons converts the port number to network byte order (big-endian).
	listenAddr.sin_port = htons(4444);

	m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connected!", 10, 10, 1.0f, 1.0f, 1.0f);
	return true;
}

bool NetworkClass::CheckNetwork(char* &localIPHolder, char* &publicIPHolder)
{
	// This function should attempt to find local IP and public IP addresses. If it failed, it should return false to indicate there is something wrong with the connection.
	struct hostent *host;
	char buffer[10000];
	char lineBuffer[200][80] = { ' ' };
	int lineIndex = 0;
	std::locale local;
	std::string websiteHTML;
	std::string url = "api.ipify.org";
	std::string getHTTP = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";
	char szPublicIP[16];
	SOCKET Socket;
	SOCKADDR_IN SockAddr;
	connectedMode = true;

	///////////////////////////////////////////////////////////////
	/////******Find Local IP Address******************************
	///////////////////////////////////////////////////////////////

	// Get the local hostname
	char szHostName[255];
	if (gethostname(szHostName, 255) == SOCKET_ERROR) {
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not find Local Host", 10, 10, 1.0f, 1.0f, 1.0f);

		connected = false;
		return false;
	}

	//Populate a host entry structure which will contain all the IP related information for this machine:
	struct hostent *host_entry;
	host_entry = gethostbyname(szHostName);
	if (host_entry == NULL) {
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not find Local Host address", 10, 10, 1.0f, 1.0f, 1.0f);

		connected = false;
		return false;
	}

	//Assign converted string to external 'LocalIP' holder
	localIPHolder = inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);

	///////////////////////////////////////////////////////////////
	/////******Find Public IP Address******************************
	///////////////////////////////////////////////////////////////

	//Create a new socket to connect to 'Find My IP Service' and connect
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	host = gethostbyname(url.c_str());
	if (host == NULL) {
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not find external host name", 10, 10, 1.0f, 1.0f, 1.0f);

		connected = false;
		return false;
	}

	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not connect to external host", 10, 10, 1.0f, 1.0f, 1.0f);
		connected = false;
		return false;
	}

	//Sent HTTP request
	if (send(Socket, getHTTP.c_str(), strlen(getHTTP.c_str()), 0) == SOCKET_ERROR)
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not send message to external host", 10, 10, 1.0f, 1.0f, 1.0f);
		connected = false;
		return false;
	}

	//Recieve response with IP on end
	int nDataLength;
	while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
		int i = 0;
		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
			websiteHTML += buffer[i];
			i += 1;
		}
	}

	// Sort response into lines
	for (size_t i = 0; i < websiteHTML.length(); ++i) {
		websiteHTML[i] = tolower(websiteHTML[i], local);
	}

	std::istringstream ss(websiteHTML);
	std::string stoken;

	while (getline(ss, stoken, '\n')) {
		strcpy_s(lineBuffer[lineIndex], stoken.c_str());
		int dot = 0;
		for (unsigned int ii = 0; ii < strlen(lineBuffer[lineIndex]); ii++) {

			if (lineBuffer[lineIndex][ii] == '.') dot++;
			if (dot >= 3) {
				dot = 0;
				strcpy_s(szPublicIP, lineBuffer[lineIndex]); //If we're at the IP, copy it to variable
			}
		}

		lineIndex++;
	}

	publicIPHolder = szPublicIP;

	if (closesocket(Socket) == SOCKET_ERROR) {
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Warning: Failed to close socket", 10, 10, 1.0f, 1.0f, 1.0f);
	}

	return true;
}

void NetworkClass::EstablishConnection(char * opponentAddress)
{
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(4444);
	sendAddr.sin_addr.s_addr = inet_addr(opponentAddress);

	NetMessage welcomeMessage;
	welcomeMessage.type = MT_WELCOME;

	SendMessage(&welcomeMessage);

	m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Attempting to establish a connection...", 10, 10, 1.0f, 1.0f, 1.0f);

	startTime = totalGameTime;
	attemptingToEstablish = true;
}

void NetworkClass::SendMessage(const NetMessage * message)
{
	if (writeCount_ + sizeof(NetMessage) > sizeof(writeBuffer_))
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Write buffer full!", 10, 10, 1.0f, 1.0f, 1.0f);
	}

	memcpy(writeBuffer_ + writeCount_, message, sizeof(NetMessage));
	writeCount_ += sizeof(NetMessage);
}

void NetworkClass::ProcessMessage(const NetMessage * message)
{
	if (message->type == MT_WELCOME) {
		messageFromOpponent = true;
	}
	else if (message->type == MT_POSITIONUPDATE) {
		//Update something somewhere with new position
	}
	else if (message->type == MT_RACEFINISHED) {
		//Update something somewhere with fact race is finished
	}
}
