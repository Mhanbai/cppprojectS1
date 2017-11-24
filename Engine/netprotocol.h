/* Definitions for the network protocol that the two clients use to communicate */

#ifndef PROTOCOL_H
#define PROTOCOL_H

// Message types.
enum MessageType
{
	MT_UNKNOWN = 0,
	MT_WELCOME = 1,
	MT_POSITIONUPDATE = 2,
	MT_RACEFINISHED = 3
};

// The message structure.
// This is a "plain old data" type, so we can send it over the network.
// (In a real program, we would want this structure to be packed.)
struct NetMessage
{
	MessageType type;
	int trackPos;
	float posX;
	float posZ;
	float timeStamp;

	NetMessage()
		: type(MT_UNKNOWN), trackPos(-1), timeStamp(-1.0f), posX(0.0f), posZ(0.0f)
	{
	}
};

#endif