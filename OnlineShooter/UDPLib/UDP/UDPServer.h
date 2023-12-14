#pragma once

#include "UDPBase.h"
#include <map>

class UDPServer : public UDPBase
{
private:
	// Clients connected to server
	FD_SET m_activeSockets;
	// Sockets with msgs waiting to be read
	FD_SET m_socketsToRead;
	// Timeinterval to wait for new msgs
	timeval m_tv;

public:
	// ctors & dtors
	UDPServer();
	virtual ~UDPServer();

	bool Initialize(const char* host, const char* port);
	void Destroy();

    // Bind address with socket and starts listening
    void StartListening();

	// Use select to check which sockets have msgs and update SET
	void UpdateSocketsToRead();

	// Check if there is any new connection and add to the active sockets SET
	void AddSocket();

	// Read all messages waiting on the sockets and insert in the output map
	void ReadNewMsgs(std::map<SOCKET, myTcp::sPacketData>& mapNewMsgsOut);
};