#pragma once

// For simplicity we keep the "inet_addr" for now so we need this flag
#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
    bool Initialize(const char* host, int port);
    // Release and delete all resources
    void Destroy();

    // Set the socket to be blocking or not
    bool SetBlocking(u_long mode);

    // Serializes and send message to the destination socket
    void SendRequest(SOCKET& destSocket, const std::string& dataTypeIn, 
                     const std::string& dataIn);

    // Deserializes message received from socket and returns the message type and string
    bool ReceiveRequest(SOCKET& origSocket, std::string& dataTypeOut, 
                        std::string& dataOut, sockaddr_in& addr, int& addrLen);

    // Read all messages incomming from socket and store in list
    virtual void ReadNewMsgs(sockaddr_in& addr, int& addrLen);

protected:
    SOCKET m_serverSocket;
    sockaddr_in m_info;

    // All wsa is initialized and socket created
    bool m_isInitialized;

    // All msg received will be stored here for later proccessing
    std::vector<myUDP::sPacketData*> m_lastPackets;

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
};