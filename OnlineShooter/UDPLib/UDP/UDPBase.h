#pragma once

// WinSock2 Windows Sockets
#define WIN32_LEAN_AND_MEAN

#define LOCALHOST "127.0.0.1"

#include "utils/common.h"
#include "utils/Buffer.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// Include win32 lib
#pragma comment(lib, "Ws2_32.lib")

class UDPBase 
{
public:
    // ctors & dtors
    UDPBase();
    ~UDPBase();

    // Getters
    SOCKET& GetSocket();

    // Initializes WSA, addres info and socket
    bool Initialize(const char* host, const char* port);
    // Release and delete all resources
    void Destroy();

    // Serializes and send message to the destination socket
    void SendRequest(SOCKET& destSocket, const std::string& dataTypeIn, 
                     const std::string& dataIn);

    // Deserializes message received from socket and returns the message type and string
    bool ReceiveRequest(SOCKET& origSocket, std::string& dataTypeOut, 
                        std::string& dataOut);

protected:
    SOCKET m_serverSocket;
    addrinfo* m_pInfo;
    // Timeinterval to wait for new msgs
    timeval m_tv;

    // Deals with any socket error responses from the WSA
    void m_SocketError(const char* function, SOCKET socket, bool isFatalError);

    // Deals with any results error responses from WSA
    void m_ResultError(const char* function, int result, bool isFatalError);

    // Serializes protobuffer packet into string packet, 
    // include length prefix header, and returns the packet total size
    uint32 m_SerializePacket(const std::string& dataTypeIn,
        const std::string& dataIn,
        Buffer& bufferOut);

    // Deserialize message from string to protobuffer packet
    bool m_DeserializePacket(const std::string& strPacketIn,
        std::string& dataTypeOut,
        std::string& dataOut);

    // All wsa is initialized and socket created
    bool m_isInitialized;
};