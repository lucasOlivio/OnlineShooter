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
	bool StartListening();

	// Find specific client id from the list
	int GetClientId(const sockaddr_in& addrIn, const int& addrLenIn);

	// Get whole list of clients currently connected to the server
	void GetConnectedClients(std::vector<sClientInfo*>& clients);

	// Check if there is any new connection and add to the list of connected clients
	// Returns the added clientid
	int AddClient(sockaddr_in& addr, int& addrLen);

	// Remove client from connected list by id in list or address
	// Returns the removed client id
	int Removeclient(const sockaddr_in& addrIn, const int& addrLenIn);
	int Removeclient(int clientId);

	// Read message in socket and add client if new
	// Returns the clientid that sent the msg, new added clientid or the removed clientid
	virtual int ReadNewMsgs(sockaddr_in& addr, int& addrLen, int& clientIdOut);

protected:
	// ConnectedClients
	std::vector<sClientInfo*> m_connectedClients;
};