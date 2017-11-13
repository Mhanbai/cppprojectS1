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

	if (FindPublicIP(myPublicIP) == false) {
		MessageBox(m_hwnd, L"Error finding public IP.", L"Error", MB_OK);
		return false;
	}

	if (FindLocalIP(myLocalIP) == false) {
		MessageBox(m_hwnd, L"Error finding local IP.", L"Error", MB_OK);
		return false;
	}

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(myLocalIP);
	// htons converts the port number to network byte order (big-endian).
	serverAddr.sin_port = htons(4444);

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

	//Create a new socket to connect to 'Find My IP Service' and connect
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	host = gethostbyname(url.c_str());

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
		strcpy(lineBuffer[lineIndex], stoken.c_str());
		int dot = 0;
		for (int ii = 0; ii< strlen(lineBuffer[lineIndex]); ii++) {

			if (lineBuffer[lineIndex][ii] == '.') dot++;
			if (dot >= 3) {
				dot = 0;
				strcpy(szPublicIP, lineBuffer[lineIndex]); //If we're at the IP, copy it to variable
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