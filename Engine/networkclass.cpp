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

	char* myIP = FindIP();

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(myIP);
	// htons converts the port number to network byte order (big-endian).
	serverAddr.sin_port = htons(4444);

	return true;
}

char * NetworkClass::FindIP()
{
	// Get the local hostname
	char szHostName[255];
	gethostname(szHostName, 255);

	//Populate a host entry structure which will contain all the IP related information for this machine:
	struct hostent *host_entry;
	host_entry = gethostbyname(szHostName);

	//Take first entry and convert to string using inet_ntoa
	char * szLocalIP;
	szLocalIP = inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);

	return szLocalIP;
}
