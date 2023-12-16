#include "UDPClient.h"

UDPClient::UDPClient()
{
}

UDPClient::~UDPClient()
{
}

bool UDPClient::Initialize(const char* host, int port)
{
    bool UDPInitialized = this->UDPBase::Initialize(host, port);
    if (!UDPInitialized)
    {
        // Error initializing sockets
        return false;
    }

    // Set recv as non-blocking
    this->SetBlocking(1);

    // Set server addr info
    m_info.sin_addr.s_addr = inet_addr(host);

    return true;
}

void UDPClient::ReadNewMsgs()
{
    m_infoLen = sizeof(m_info);
    UDPBase::ReadNewMsgs(m_info, m_infoLen);
}
