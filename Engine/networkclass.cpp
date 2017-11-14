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

void NetworkClass::Shutdown()
{
	m_hwnd = 0;

	if (closesocket(sock) == SOCKET_ERROR) {
		MessageBox(m_hwnd, L"Failed to close socket", L"Error", MB_OK);
	}

	WSACleanup();

	return;
}

bool NetworkClass::Frame()
{
	if (connectedMode) {
		int count;
		// Send data needed sent
		count = send(sock, writeBuffer_, writeCount_, 0);
		if (count <= 0)
		{
			MessageBox(m_hwnd, L"Connection lost or broken.", L"Error", MB_OK);
			return false;
		}

		writeCount_ -= count;

		// Remove the sent data from the start of the buffer.
		memmove(writeBuffer_, writeBuffer_ + count, writeCount_);

		// Receive data needing recieved
		int spaceLeft = (sizeof readBuffer_) - readCount_;
		count = recv(sock, readBuffer_ + readCount_, spaceLeft, 0);
		if (count <= 0)
		{
			printf("Client connection closed or broken\n");
			return true;
		}

		// We've successfully read some more data into the buffer.
		readCount_ += count;

		if (readCount_ < sizeof NetMessage)
		{
			// ... but we've not received a complete message yet.
			// So we can't do anything until we receive some more.
			return false;
		}

		// We've got a complete message.
		//processMessage((const NetMessage *)readBuffer_);

		// Clear the buffer, ready for the next message.
		readCount_ = 0;
	}

	return true;
}

bool NetworkClass::Initialize(HWND &hwnd)
{
	m_hwnd = hwnd;

	//Initialize WinSock and check for correct version
	int error = WSAStartup(0x0202, &w);
	if (error != 0)
	{
		MessageBox(m_hwnd, L"WSA Startup failed.", L"Error", MB_OK);
		return false;
	}
	if (w.wVersion != 0x0202)
	{
		MessageBox(m_hwnd, L"Wrong WinSock version.", L"Error", MB_OK);
		return false;
	}

	// Create a UDP socket.
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		MessageBox(m_hwnd, L"Socket Failed.", L"Error", MB_OK);
		return false;
	}

	// Set socket to non-blocking mode
	unsigned long value = 1;
	ioctlsocket(sock, FIONBIO, &value);

	if (FindPublicIP(myPublicIP) == false) {
		MessageBox(m_hwnd, L"Error finding public IP.", L"Error", MB_OK);
		return false;
	}

	if (FindLocalIP(myLocalIP) == false) {
		MessageBox(m_hwnd, L"Error finding local IP.", L"Error", MB_OK);
		return false;
	}

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = inet_addr(myLocalIP);
	// htons converts the port number to network byte order (big-endian).
	listenAddr.sin_port = htons(4444);

	return true;
}

bool NetworkClass::FindLocalIP(char* &localIPHolder)
{
	// Get the local hostname
	char szHostName[255];
	if (gethostname(szHostName, 255) == SOCKET_ERROR) {
		MessageBox(m_hwnd, L"Error getting local host name.", L"Error", MB_OK);
		return false;
	}

	//Populate a host entry structure which will contain all the IP related information for this machine:
	struct hostent *host_entry;
	host_entry = gethostbyname(szHostName);
	if (host_entry == NULL) {
		MessageBox(m_hwnd, L"Error finding host address.", L"Error", MB_OK);
		return false;
	}

	//Take first entry and convert to string using inet_ntoa
	localIPHolder = inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);

	return true;
}

bool NetworkClass::FindPublicIP(char* &publicIPHolder)
{
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

	//Create a new socket to connect to 'Find My IP Service' and connect
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	host = gethostbyname(url.c_str());
	if (host == NULL) {
		MessageBox(m_hwnd, L"Could not connect to external host.", L"Error", MB_OK);
		connectedMode = false;
	}

	if (connectedMode == true) {
		SockAddr.sin_port = htons(80);
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

		if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
			MessageBox(m_hwnd, L"Could not connect.", L"Error", MB_OK);
			return false;
		}

		//Sent HTTP request
		if (send(Socket, getHTTP.c_str(), strlen(getHTTP.c_str()), 0) == SOCKET_ERROR)
		{
			MessageBox(m_hwnd, L"Could not send.", L"Error", MB_OK);
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

		if (szPublicIP == "") {
			FindPublicIP(publicIPHolder); //If this didnt work (due to no response, etc) run the Function again.
		}
		else {
			publicIPHolder = szPublicIP; //Assign IP value to in_variable
		}

		if (closesocket(Socket) == SOCKET_ERROR) {
			MessageBox(m_hwnd, L"Failed to close socket", L"Error", MB_OK);
			return false;
		}

		return true;
	}

	return false;
}

bool NetworkClass::EstablishConnection(char * opponentAddress)
{
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(4444);
	sendAddr.sin_addr.s_addr = inet_addr(opponentAddress);

	/*NetMessage welcomeMessage;
	welcomeMessage.type = MT_WELCOME;

	if (sendto(sock, welcomeMessage, MESSAGESIZE, 0, (const sockaddr *)&sendAddr, sizeof(sendAddr)) == SOCKET_ERROR)
	{
		return false;
	}*/


	return true;
}

bool NetworkClass::SendMessage(const NetMessage * message)
{
	if (writeCount_ + sizeof(NetMessage) > sizeof(writeBuffer_))
	{
		MessageBox(m_hwnd, L"Write Buffer Full", L"Error", MB_OK);
		return false;
	}

	memcpy(writeBuffer_ + writeCount_, message, sizeof(NetMessage));
	writeCount_ += sizeof(NetMessage);
}
