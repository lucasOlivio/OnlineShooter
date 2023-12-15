#pragma once

#include "UDPBase.h"
#include "utils/common.h"
#include <map>

class UDPServer : public UDPBase
{
public:
	// ctors & dtors
	UDPServer();
	virtual ~UDPServer();

	void Destroy();

    // Bind address with socket and starts listening
    void StartListening();

	// Check if there is any new connection and add to the list of connected clients
	void AddClient(sockaddr_in& addr, int& addrLen);

	// Read message in socket and add client if new
	virtual void ReadNewMsgs();

protected:
	struct sClientInfo
	{
		sockaddr_in addr;
		int addrLen;
	};

	// ConnectedClients
	std::vector<sClientInfo*> m_connectedClients;
};