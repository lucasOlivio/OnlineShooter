#pragma once

#include "UDPBase.h"
#include "utils/common.h"
#include <map>

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

class UDPServer : public UDPBase
{
public:
	// ctors & dtors
	UDPServer();
	virtual ~UDPServer();

	void Destroy();

    // Bind address with socket and starts listening
    void StartListening();

	// Find specific client id from the list
	int GetClientId(const sockaddr_in& addrIn, const int& addrLenIn);

	// Get whole list of clients currently connected to the server
	void GetConnectedClients(std::vector<sClientInfo*>& clients);

	// Check if there is any new connection and add to the list of connected clients
	void AddClient(sockaddr_in& addr, int& addrLen);

	// Remove client from connected list by id in list or address
	void Removeclient(const sockaddr_in& addrIn, const int& addrLenIn);
	void Removeclient(int clientId);

	// Read message in socket and add client if new
	virtual void ReadNewMsgs();

protected:
	// ConnectedClients
	std::vector<sClientInfo*> m_connectedClients;
};