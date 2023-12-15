#include "UDPServer.h"

UDPServer::UDPServer()
{
}

UDPServer::~UDPServer()
{
}

void UDPServer::Destroy()
{
    UDPBase::Destroy();

	for (sClientInfo* pClient : m_connectedClients)
	{
		delete pClient;
	}
	m_connectedClients.clear();

    return;
}

void UDPServer::StartListening()
{
	if (!m_isInitialized)
	{
		return;
	}

	int result;

	// Binds socket to address
	result = bind(m_serverSocket, (SOCKADDR*)&m_info, sizeof(m_info));
	if (result == SOCKET_ERROR) {
		m_ResultError("bind", result, true);
		return;
	}
	printf("bind was successful!\n");

	// Set recv as non-blocking
	this->SetBlocking(1);

	printf("listen successful\n");

	return;
}

void UDPServer::AddClient(sockaddr_in& addr, int& addrLen)
{
	if (m_GetClientId(addr, addrLen) > -1)
	{
		// Client already on list
		return;
	}

	// New connected client
	sClientInfo* pNewClient = new sClientInfo();
	pNewClient->addr = addr;
	pNewClient->addrLen = sizeof(addr);
	m_connectedClients.push_back(pNewClient);
}

void UDPServer::ReadNewMsgs()
{
	sockaddr_in addr;
	int addrLen;

	UDPBase::ReadNewMsgs(addr, addrLen);

    // Now we add the client to our list if its not there
	AddClient(addr, addrLen);
}

int UDPServer::m_GetClientId(sockaddr_in& addr, int& addrLen)
{
	// Compare to see if the addr is already registered
	// If it is not registered, we add it
	for (int i = 0; i < m_connectedClients.size(); i++)
	{
		sClientInfo* pClient = m_connectedClients[i];

		if (pClient->IsEqualTo(addr))
		{
			return i;
		}
	}

	// Client not found
	return -1;
}
