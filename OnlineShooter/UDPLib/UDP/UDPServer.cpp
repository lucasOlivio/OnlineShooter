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

bool UDPServer::StartListening()
{
	if (!m_isInitialized)
	{
		return false;
	}

	int result;

	// Binds socket to address
	result = bind(m_serverSocket, (SOCKADDR*)&m_info, sizeof(m_info));
	if (result == SOCKET_ERROR) {
		m_ResultError("bind", result, true);
		return false;
	}
	printf("bind was successful!\n");

	// Set recv as non-blocking
	this->SetBlocking(1);

	printf("listen successful\n");

	return true;
}

int UDPServer::GetClientId(const sockaddr_in& addrIn, const int& addrLenIn)
{
	// Compare to see if the addr is already registered
	// If it is not registered, we add it
	for (int i = 0; i < m_connectedClients.size(); i++)
	{
		sClientInfo* pClient = m_connectedClients[i];

		if (pClient->IsEqualTo(addrIn))
		{
			return i;
		}
	}

	// Client not found
	return -1;
}

void UDPServer::GetConnectedClients(std::vector<sClientInfo*>& clients)
{
	clients = m_connectedClients;
}

void UDPServer::AddClient(sockaddr_in& addr, int& addrLen)
{
	if (GetClientId(addr, addrLen) > -1)
	{
		// Client already on list
		return;
	}

	// New connected client
	sClientInfo* pNewClient = new sClientInfo();
	m_connectedClients.push_back(pNewClient);

	pNewClient->addr = addr;
	pNewClient->addrLen = sizeof(addr);
}

void UDPServer::Removeclient(const sockaddr_in& addrIn, const int& addrLenIn)
{
	int clientId = GetClientId(addrIn, addrLenIn);

	Removeclient(clientId);
}

void UDPServer::Removeclient(int clientId)
{
	if (clientId < 0 || clientId > m_connectedClients.size())
	{
		return;
	}

	m_connectedClients.erase(m_connectedClients.begin() + clientId);
}

int UDPServer::ReadNewMsgs()
{
	sockaddr_in addr;
	int addrLen = sizeof(addr);

	int result = UDPBase::ReadNewMsgs(addr, addrLen);

	if (result == WSAECONNRESET)
	{
		// Client disconnected
		Removeclient(addr, addrLen);

		return result;
	}

	if (result > 0)
	{
		// Now we add the client to our list if its not there
		AddClient(addr, addrLen);
	}

	return result;
}
