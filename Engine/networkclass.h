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
#include <winsock2.h>
#include <iphlpapi.h>
#include <wininet.h> 
#include <string>
#include <string.h>
#include <locale>
#include <sstream>
#include "netprotocol.h"

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
	bool Frame();
	bool Initialize(HWND &hwnd);
	bool FindLocalIP(char* &localIPHolder);
	bool FindPublicIP(char* &publicIPHolder);
	bool EstablishConnection(char* opponentAddress);
	bool SendMessage(const NetMessage *message);
	char* myLocalIP;
	char* myPublicIP;

private:
	HWND m_hwnd;
	//Network variables
	WSADATA w;
	SOCKET sock;
	sockaddr_in listenAddr;
	sockaddr_in sendAddr;
	bool connectedMode;
	int count;
	// Data we need to send to the client.
	char writeBuffer_[100 * sizeof NetMessage];
	int writeCount_;
	// The data we've read from the client.
	char readBuffer_[sizeof NetMessage];
	int readCount_;
};

#endif