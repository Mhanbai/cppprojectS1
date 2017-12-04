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
	netConnected = true; //Default connected to internet as 'true'
	m_graphics = graphics; //Fill reference to graphics

	m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Attempting to establish connection...", 10, 10, 1.0f, 1.0f, 1.0f);

	//Initialize WinSock and check for correct version
	int error = WSAStartup(0x0202, &w);
	if (error != 0)
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not start WinSock", 10, 10, 1.0f, 1.0f, 1.0f);
		netConnected = false;
		return false;
	}

	if (w.wVersion != 0x0202)
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Wrong WinSock version", 10, 10, 1.0f, 1.0f, 1.0f);
		netConnected = false;
		return false;
	}

	// Create a UDP socket.
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not create UDP socket", 10, 10, 1.0f, 1.0f, 1.0f);
		netConnected = false;
		return false;
	}

	// Set socket to non-blocking mode
	unsigned long value = 1;
	ioctlsocket(sock, FIONBIO, &value);

	//Check to see if connected to internet - if not return false with WinSock set up aleady complete
	if (CheckNetwork(myLocalIP, myPublicIP) == false) {
		netConnected = false;
		return false;
	}

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
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

	//Put a blank message at the start of 'positionUpdates' in order to prevent null pointer error in 'Process Message' function
	Update blank;
	positionUpdates.push_back(blank);
	positionUpdates.push_back(blank);
	positionUpdates.push_back(blank);

	return true;
}

void NetworkClass::Shutdown()
{
	closesocket(sock);
	WSACleanup();

	return;
}

bool NetworkClass::Frame(float time)
{
	//Game time equals the sum of the frame timer output divided by 1000 
	totalGameTime = totalGameTime + (time / 1000);

	//If the game is trying to establish a connection...
	if (establishingConnection == true) {
		//And a message has been recieved...
		if (messageReceived == true) {
			//Set connectionEstablished variable to true so game.cpp knows to start a game
			m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection established!", 10, 10, 1.0f, 1.0f, 1.0f);
			establishingConnection = false;
			connectionEstablished = true;
		}
		else if ((totalGameTime - startTime) > 3.0f) {
			//If we've been trying to establish a connection for over 3 seconds, send the message again
			EstablishConnection();
			startTime = totalGameTime;
		}
	}

	int count;
	if (writeCount_ > 0) {
		// Send data needing sent
		count = sendto(sock, writeBuffer_, sizeof(NetMessage), 0, (const sockaddr *)&sendAddr, sizeof(sendAddr));
		if (count == SOCKET_ERROR) {
			char errorBuffer[32];
			sprintf_s(errorBuffer, "Send Error: %i", WSAGetLastError());
			m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus2, errorBuffer, 10, 30, 1.0f, 1.0f, 1.0f);
		}
		//else
		{
			//Deduct the sum of the data thats just been sent from writeCount
			writeCount_ -= count;

			// Remove the sent data from the start of the buffer.
			memmove(writeBuffer_, writeBuffer_ + count, writeCount_);
		}
	}

	// Receive data needing recieved
	int spaceLeft = (sizeof readBuffer_) - readCount_;

	count = recvfrom(sock, readBuffer_, sizeof(readBuffer_), 0, &from, &fromlen);
	if (count == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			char errorBuffer[32];
			sprintf_s(errorBuffer, "Recieve Error: %i", WSAGetLastError());
			m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus3, errorBuffer, 10, 50, 1.0f, 1.0f, 1.0f);
		}
	}
	else
	{
		//m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus3, "Recieved message!", 10, 30, 1.0f, 1.0f, 1.0f);
		// We've successfully read some more data into the buffer.
		readCount_ += count;

		if (readCount_ < sizeof NetMessage)
		{
			// ... but we've not received a complete message yet.
			// So we can't do anything until we receive some more.
			return true;
		}
		else
		{
			// We've got a complete message.
			ProcessMessage((const NetMessage *)readBuffer_);

			// Clear the buffer, ready for the next message.
			readCount_ = 0;
		}
	}

	if ((messageReceived == true) && (raceHasStarted == true)){
		if (ping >= 0.5f) {
			errorTimer += (time / 1000);
			if (errorTimer > 1.0f) {
				m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Attempting to reconnect...", 10, 10, 1.0f, 1.0f, 1.0f);
				//Pause game until ping goes down
				errorPause = true;
				//Wait and close game if it doesnt
				errorTimer += (time / 1000);
				if ((errorTimer > 5.0f) && (errorTimer <= 8.0f)) {
					char killBuffer[64];
					sprintf_s(killBuffer, "Connection Lost. Closing game in %.2f", 8.0f - errorTimer);
					m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, killBuffer, 10, 10, 1.0f, 1.0f, 1.0f);
				}
				else if (errorTimer > 8.0f) {
					return false;
				}
			}
		}
		else if ((ping < 0.5f) && (ping >= 0.4f)) {
			errorTimer = 0.0f;
		}
		else if ((ping < 0.4f) && (ping >= 0.2f)) {
			errorPause = false;
			totalGameTime = totalGameTime - errorTimer;
			errorTimer = 0.0f;
			m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "WARNING: Network difficulties", 10, 10, 1.0f, 1.0f, 1.0f);
		}
		else if (ping < 0.2f) {
			errorPause = false;
			totalGameTime = totalGameTime - errorTimer;
			errorTimer = 0.0f;
			m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection established!", 10, 10, 1.0f, 1.0f, 1.0f);
		}
	}

	ping = totalGameTime - lastMessageRecieved;

	return true;
}

// This class can be used to complete initialization if CheckNetwork returns false
bool NetworkClass::RecheckNetwork()
{
	if (CheckNetwork(myLocalIP, myPublicIP) == false) {
		netConnected = false;
		return false;
	}

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	// htons converts the port number to network byte order (big-endian).
	listenAddr.sin_port = htons(4444);

	m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connected!", 10, 10, 1.0f, 1.0f, 1.0f);
	return true;
}

void NetworkClass::ResetGameTime()
{
	totalGameTime = 0.0f;
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

	///////////////////////////////////////////////////////////////
	/////******Find Local IP Address******************************
	///////////////////////////////////////////////////////////////

	// Get the local hostname
	char szHostName[255];
	if (gethostname(szHostName, 255) == SOCKET_ERROR) {
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not find Local Host", 10, 10, 1.0f, 1.0f, 1.0f);

		netConnected = false;
		return false;
	}

	//Populate a host entry structure which will contain all the IP related information for this machine:
	struct hostent *host_entry;
	host_entry = gethostbyname(szHostName);
	if (host_entry == NULL) {
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not find Local Host address", 10, 10, 1.0f, 1.0f, 1.0f);

		netConnected = false;
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

		netConnected = false;
		return false;
	}

	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not connect to external host", 10, 10, 1.0f, 1.0f, 1.0f);
		netConnected = false;
		return false;
	}

	//Sent HTTP request
	if (send(Socket, getHTTP.c_str(), strlen(getHTTP.c_str()), 0) == SOCKET_ERROR)
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Connection Disabled: Could not send message to external host", 10, 10, 1.0f, 1.0f, 1.0f);
		netConnected = false;
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
	desiredOpponent = opponentAddress;

	//Set address data to what is input by the user on the main menu
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(4444);

	if (opponentAddress[0] != '\0') {
		sendAddr.sin_addr.s_addr = inet_addr(opponentAddress);
	}
	else {
		sendAddr.sin_addr.s_addr = inet_addr(myLocalIP);
	}

	//Create a new message of type 'Welcome'
	NetMessage welcomeMessage;
	welcomeMessage.type = MT_WELCOME;
	if (messageReceived == true) {
		welcomeMessage.trackPos = 1;
	}
	else {
		welcomeMessage.trackPos = 0;
	}

	//Send that message to the write queue
	SendNetMessage(&welcomeMessage);

	m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Attempting to establish a connection...", 10, 10, 1.0f, 1.0f, 1.0f);

	//Set the current start time to the current time, so Frame() knows when to resend the message
	startTime = totalGameTime;
	establishingConnection = true;
}

//Same function as above, just doesnt need to take an addres variable as that's already stored
void NetworkClass::EstablishConnection()
{
	NetMessage welcomeMessage;
	welcomeMessage.type = MT_WELCOME;
	if (messageReceived == true) {
		welcomeMessage.trackPos = 1;
	}
	else {
		welcomeMessage.trackPos = 0;
	}
	SendNetMessage(&welcomeMessage);
	m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Attempting to establish a connection...", 10, 10, 1.0f, 1.0f, 1.0f);
	startTime = totalGameTime;
}

void NetworkClass::SendNetMessage(const NetMessage * message)
{
	//If theres more messages on the write buffer than it can handle, complain to the user
	if (writeCount_ + sizeof(NetMessage) > sizeof(writeBuffer_))
	{
		m_graphics->m_Text->UpdateSentence(m_graphics->m_Text->networkStatus, "Write buffer full!", 10, 10, 1.0f, 1.0f, 1.0f);
	}

	//Copy the message needing sent to the write buffer, and increase the number we have to represent its size.
	memcpy(writeBuffer_ + writeCount_, message, sizeof(NetMessage));
	writeCount_ += sizeof(NetMessage);
}

void NetworkClass::PositionUpdate(float x, float z, float time, float angle)
{
	NetMessage positionUpdate;
	positionUpdate.type = MT_POSITIONUPDATE;
	positionUpdate.timeStamp = time;
	positionUpdate.posX = x;
	positionUpdate.posZ = z;
	SendNetMessage(&positionUpdate);
}

void NetworkClass::SendVictory(float timestamp)
{
	NetMessage victoryUpdate;
	victoryUpdate.type = MT_RACEFINISHED;
	victoryUpdate.timeStamp = timestamp;
	SendNetMessage(&victoryUpdate);
}

void NetworkClass::ProcessMessage(const NetMessage * message)
{
	lastMessageRecieved = message->timeStamp;

	if (message->type == MT_WELCOME) {
		messageReceived = true;
		trackPosition = message->trackPos;
	}
	else if (message->type == MT_POSITIONUPDATE) {
		//If the time of this update is greater than the time of the previous update
		if ((message->timeStamp > positionUpdates[positionUpdates.size() - 1].timeStamp) && 
			//If the game is not paused due to a poor connection
			(errorPause == false) &&
			//If the update is within the bounds of the track
			((message->posX < 500.0f) && (message->posX > -500.0f)) &&
			((message->posZ < 500.0f) && (message->posZ > -500.0f)))
		
		{

			Update update;
			update.timeStamp = message->timeStamp;
			update.posX = message->posX;
			update.posZ = message->posZ;
			positionUpdates.push_back(update);
			updateAvailable = true;
		}
	}
	else if (message->type == MT_RACEFINISHED) {
		opponentHasWon = true;
	}
}
