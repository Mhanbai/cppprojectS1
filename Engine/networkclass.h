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
#include <vector>
#include "netprotocol.h"
#include "graphicsclass.h"

struct Update
{
	float timeStamp;
	float posX;
	float posZ;
	float angle;
	Update()
		: timeStamp(0.0f), posX(0.0f), posZ(0.0f), angle (0.0f)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Network Class
////////////////////////////////////////////////////////////////////////////////
class NetworkClass
{
public:
	//Basic functionality
	NetworkClass();
	NetworkClass(const NetworkClass&);
	~NetworkClass();
	bool Initialize(GraphicsClass* &graphics);
	void Shutdown();
	bool Frame(float time);

	//Check computer is connected to internet, and recheck if previous check failed
	bool CheckNetwork(char* &localIPHolder, char* &publicIPHolder);
	bool RecheckNetwork();

	//Variables and functions for timing
	void ResetGameTime();
	float ping = 0.0f;
	bool errorPause = false;
	float errorTimer = 0.0f;
	bool raceHasStarted = false;
	float lastMessageRecieved = 0.0f;

	//Establish a connection to an opponent, and retry if first attempt failed
	void EstablishConnection(char* opponentAddress);
	void EstablishConnection();

	//Send messages
	void SendNetMessage(const NetMessage *message);
	void PositionUpdate(float x, float z, float time, float angle);
	void SendVictory(float timestamp);
	bool opponentHasWon = false;

	//Process recieved messages
	void ProcessMessage(const NetMessage *message);

	//Logic for connecting to another player
	bool netConnected = true;
	bool establishingConnection = false;
	bool messageReceived = false;
	bool connectionEstablished = false;
	bool updateAvailable = false;
	int trackPosition;

	//Variables that store public and local IPs
	char* myLocalIP;
	char* myPublicIP;

	//Contain position update messages from opponent
	std::vector<Update> positionUpdates;

private:
	//Network variables
	WSADATA w;
	SOCKET sock;

	//Address to listen on
	sockaddr_in listenAddr;

	//Address to send to & address recieved back
	sockaddr_in sendAddr;
	sockaddr from;
	int fromlen = sizeof(from);

	//Variable to store amount of data recieved and sent
	int count;

	// Data we need to send to the client.
	char writeBuffer_[100 * sizeof NetMessage];
	int writeCount_ = 0;

	// The data we've read from the client.
	char readBuffer_[sizeof NetMessage];
	int readCount_ = 0;

	// Timer
	float totalGameTime = 0.0f;
	float startTime = 0.0f;

	// Reference to Graphics so network status text can be updated
	GraphicsClass* m_graphics;

	//Address the player types in to connect to
	char* desiredOpponent;
	char* opponentConnecting;
};

#endif