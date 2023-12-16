#pragma once

#define LOCALHOST "127.0.0.1"

#include "utils/common.h"
#include "utils/Buffer.h"

class UDPBase 
{
public:
    // ctors & dtors
    UDPBase();
    ~UDPBase();

    // Getters
    SOCKET& GetSocket();
    sockaddr_in& GetInfo();

    // Initializes WSA, addres info and socket
    bool Initialize(const char* host, int port);
    bool Initialize(u_long host, int port);

    // Release and delete all resources
    void Destroy();

    // Set the socket to be blocking or not
    bool SetBlocking(u_long mode);

    // Serializes and send message to the destination socket
    void SendRequest(const std::string& dataTypeIn, const std::string& dataIn,
					 const sockaddr_in& addrIn, const int& addrLenIn);

    // Deserializes message received from socket and 
    // returns the packet length, message type and string
    int ReceiveRequest(std::string& dataTypeOut, std::string& dataOut, 
                        sockaddr_in& addrOut, int& addrLenOut);

    // Read all messages incomming from socket and store in list
    virtual int ReadNewMsgs(sockaddr_in& addr, int& addrLen);

    // Retrieve the packet cache
    virtual void GetNewMsgs(std::vector<myUDP::sPacketData*>& newPackets);

    // Delete the packet cache and pointers
    virtual void ResetMsgs();

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