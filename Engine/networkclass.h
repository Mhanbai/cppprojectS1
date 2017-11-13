#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")

////////////////////////////////////////////////////////////////////////////////
// Filename: networkclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _NETWORKCLASS_H_
#define _NERWORKCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <winsock.h>
#include <iphlpapi.h>
#include <wininet.h> 
#include <string>
#include <string.h>
#include <locale>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////
// Class name: Network Class
////////////////////////////////////////////////////////////////////////////////
class NetworkClass
{
public:
	NetworkClass();
	NetworkClass(const NetworkClass&);
	~NetworkClass();
	void Shutdown();
	bool Initialize(HWND &hwnd);
	bool FindLocalIP(char* &localIPHolder);
	bool FindPublicIP(char* &publicIPHolder);
	char* myLocalIP;
	char* myPublicIP;

private:
	HWND m_hwnd;
	//Network variables
	WSADATA w;
	SOCKET sock;
	sockaddr_in serverAddr;

};

#endif