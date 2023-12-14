#include "UDPClient.h"

UDPClient::UDPClient()
{
    // For clients we can't stay waiting new msgs for too long
    this->m_tv.tv_sec = 0;
    this->m_tv.tv_usec = (long)0.1;
}

UDPClient::~UDPClient()
{
}

bool UDPClient::Initialize(const char* host, const char* port)
{
    bool UDPInitialized = this->UDPBase::Initialize(host, port);
    if (!UDPInitialized)
    {
        // Error initializing sockets
        return false;
    }

    // Set recv as non-blocking
    this->SetBlocking(1);

    // Try to connect to server
    bool isConnected = this->Connect();
    if (!isConnected)
    {
        // Not able to connect to server
        return false;
    }

    return true;
}

void UDPClient::Destroy()
{
    UDPBase::Destroy();
    return;
}

bool UDPClient::Connect()
{
    connect(this->m_serverSocket, this->m_pInfo->ai_addr, (int)this->m_pInfo->ai_addrlen);
    if (this->m_serverSocket == INVALID_SOCKET) {
        this->m_SocketError("connect", this->m_serverSocket, true);
        return false;
    }
    printf("Connect to the server successfully!\n");
    return true;
}

bool UDPClient::SetBlocking(u_long mode)
{
    int result = ioctlsocket(this->m_serverSocket, FIONBIO, &mode);
    if (result != NO_ERROR)
    {
        std::string errorMsg = "ioctlsocket failed with error: " + std::to_string(result);
        this->m_SocketError(errorMsg.c_str(), this->m_serverSocket, true);
        return false;
    }

    return true;
}

bool UDPClient::CheckMsgFromServer(int tv_sec)
{
    timeval local_tv;
    local_tv.tv_sec = tv_sec;
    local_tv.tv_usec = 0;

    FD_SET socketsToRead;

    // Resets sockets to read
    FD_ZERO(&socketsToRead);

    // Add our server socket to our set to check for new connections
    FD_SET(this->m_serverSocket, &socketsToRead);

    // Non-blocking way of checking if there is any new msg from the sockets
    int count = select(0, &socketsToRead, NULL, NULL, &local_tv);
    if (count == 0)
    {
        // Timevalue expired
        return false;
    }
    if (count == SOCKET_ERROR)
    {
        this->m_ResultError("select", count, false);
        return false;
    }

    return true;
}
