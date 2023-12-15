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

	struct sClientInfo
	{
		sockaddr_in addr;
		int addrLen;

		bool IsEqualTo(sockaddr_in addr)
		{
			if (this->addr.sin_addr.s_addr == addr.sin_addr.s_addr &&
				this->addr.sin_port == addr.sin_port)
			{
				return true;
			}
			return false;
		}

		bool IsEqualTo(sClientInfo client)
		{
			return this->IsEqualTo(client.addr);
		}
	};

protected:
	// ConnectedClients
	std::vector<sClientInfo*> m_connectedClients;

	// Find the client id from the list
	int m_GetClientId(sockaddr_in& addr, int& addrLen);
};