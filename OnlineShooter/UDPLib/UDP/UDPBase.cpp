#include "UDPBase.h"
#include "packet.pb.h"

UDPBase::UDPBase()
{
    m_serverSocket = INVALID_SOCKET;
	m_isInitialized = false;
}

UDPBase::~UDPBase()
{
}

SOCKET& UDPBase::GetSocket()
{
    return m_serverSocket;
}

bool UDPBase::Initialize(const char* host, int port)
{
	if (m_isInitialized)
	{
		// Already initialized
		return true;
	}

	// Initialize WinSock
	WSADATA wsaData;
	int result;

	// Set version 2.2 with MAKEWORD(2,2)
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		m_ResultError("WSAStartup", result, true);
		return false;
	}
	printf("WSAStartup successfully!\n");

	m_info.sin_family = AF_INET;
	m_info.sin_port = htons(port);
	m_info.sin_addr.s_addr = inet_addr(host);

	// Creates new socket
	m_serverSocket = socket(AF_INET,      // IPv4
							SOCK_DGRAM,   // Datagram
							IPPROTO_UDP); // UDP
	if (m_serverSocket == INVALID_SOCKET) {
		m_SocketError("socket", m_serverSocket, true);
		return false;
	}
	printf("socket created successfully!\n");

	m_isInitialized = true;

    return true;
}

bool UDPBase::Initialize(u_long host, int port)
{
	if (m_isInitialized)
	{
		// Already initialized
		return true;
	}

	// Initialize WinSock
	WSADATA wsaData;
	int result;

	// Set version 2.2 with MAKEWORD(2,2)
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		m_ResultError("WSAStartup", result, true);
		return false;
	}
	printf("WSAStartup successfully!\n");

	m_info.sin_family = AF_INET;
	m_info.sin_port = htons(port);
	m_info.sin_addr.s_addr = htonl(host);

	// Creates new socket
	m_serverSocket = socket(AF_INET,      // IPv4
		SOCK_DGRAM,   // Datagram
		IPPROTO_UDP); // UDP
	if (m_serverSocket == INVALID_SOCKET) {
		m_SocketError("socket", m_serverSocket, true);
		return false;
	}
	printf("socket created successfully!\n");

	m_isInitialized = true;

	return true;
}

void UDPBase::Destroy()
{
	if (m_serverSocket != INVALID_SOCKET)
	{
		closesocket(m_serverSocket);
	}
	WSACleanup();
	m_isInitialized = false;

	ResetMsgs();

	return;
}

bool UDPBase::SetBlocking(u_long mode)
{
	int result = ioctlsocket(m_serverSocket, FIONBIO, &mode);
	if (result != NO_ERROR)
	{
		std::string errorMsg = "ioctlsocket failed with error: " + std::to_string(result);
		m_SocketError(errorMsg.c_str(), m_serverSocket, true);
		return false;
	}

	return true;
}

void UDPBase::SendRequest(const std::string& dataTypeIn, const std::string& dataIn,
						  const sockaddr_in& addrIn, const int& addrLenIn)
{
	if (!m_isInitialized)
	{
		return;
	}

	// Build string packet to send
	Buffer buffer(BUFFER_SIZE);
	int packetSize = m_SerializePacket(dataTypeIn, dataIn, buffer);

	// Send data and validate if succesfull
	int result = sendto(m_serverSocket, (const char*)(&buffer.vecBufferData[0]), packetSize, 
						0, (SOCKADDR*)&addrIn, addrLenIn);
	if (result == SOCKET_ERROR) {
		m_SocketError("send", result, false);
		return;
	}
	return;
}

int UDPBase::ReceiveRequest(std::string& dataTypeOut, std::string& dataOut,
							 sockaddr_in& addrOut, int& addrLenOut)
{
	if (!m_isInitialized)
	{
		return false;
	}

	std::string strPacket = "";
	uint32 packetSize = 0;

	Buffer buffer(BUFFER_SIZE);

	// Get total packet size first to prepare buffer
	buffer.vecBufferData.resize(sizeof(packetSize));
	int result = recvfrom(m_serverSocket, (char*)(&buffer.vecBufferData[0]), sizeof(packetSize),
						  0, (SOCKADDR*)&addrOut, &addrLenOut);
	int ierr = WSAGetLastError();
	if (ierr == WSAEWOULDBLOCK) {  // currently no data available
		dataTypeOut = "";
		dataOut = "";
		return 0;
	}
	if (result == SOCKET_ERROR) {
		m_SocketError("recv", result, false);
		return result;
	}
	if (result == 0)
	{
		// User disconnected
		return result;
	}

	// + 1 for the /0 end of string
	packetSize = buffer.ReadUInt32LE() + 1;
	buffer.vecBufferData.resize(packetSize);
	// Now we can get the rest of the message, with the rest total size
	result = recvfrom(m_serverSocket, (char*)(&buffer.vecBufferData[0]), packetSize,
					  0, (SOCKADDR*)&addrOut, &addrLenOut);
	if (result == SOCKET_ERROR) {
		m_SocketError("recv", result, false);
		return result;
	}

	// Transform the data into our readable string
	strPacket = buffer.ReadString(0, packetSize - 1);
	bool isDeserialized = m_DeserializePacket(strPacket, dataTypeOut, dataOut);
	if (!isDeserialized)
	{
		return result;
	}

	return result;
}

int UDPBase::ReadNewMsgs(sockaddr_in& addr, int& addrLen)
{
	// Get any waiting msg from socket
	myUDP::sPacketData* pPacketOut = new myUDP::sPacketData();
	int result = ReceiveRequest(pPacketOut->dataType, pPacketOut->data, addr, addrLen);
	if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		m_ResultError("recvfrom failed", result, true);

		delete pPacketOut;

		return -1;
	}

	if (result == 0)
	{
		delete pPacketOut;

		return 0;
	}

	pPacketOut->addr = addr;
	pPacketOut->addrLen = addrLen;
	m_lastPackets.push_back(pPacketOut);

	return result;
}

void UDPBase::GetNewMsgs(std::vector<myUDP::sPacketData*>& newPackets)
{
	newPackets = m_lastPackets;
}

void UDPBase::ResetMsgs()
{
	for (myUDP::sPacketData* pPacket : m_lastPackets)
	{
		delete pPacket;
	}
	m_lastPackets.clear();
}

void UDPBase::m_SocketError(const char* function, SOCKET socket, bool isFatalError)
{
	printf("%s failed with error %d\n", function, WSAGetLastError());

	if (isFatalError)
	{
		Destroy();
	}
	return;
}

void UDPBase::m_ResultError(const char* function, int result, bool isFatalError)
{
	printf("%s failed with error %d\n", function, result);

	if (isFatalError)
	{
		Destroy();
	}
	return;
}

uint32 UDPBase::m_SerializePacket(const std::string& dataTypeIn,
	const std::string& dataIn,
	Buffer& bufferOut)
{
	// Build protobuffer
	std::string packetSerialized;
	packet::PacketData packetData;
	packetData.set_data(dataIn.c_str());
	packetData.set_datatype(dataTypeIn.c_str());
	packetSerialized = packetData.SerializeAsString();

	// Build prefix header
	bufferOut.WriteUInt32LE((uint32)packetSerialized.size());
	bufferOut.WriteString(packetSerialized);

	return (uint32)bufferOut.vecBufferData.size();
}

bool UDPBase::m_DeserializePacket(const std::string& strPacketIn,
	std::string& dataTypeOut,
	std::string& dataOut)
{
	// Build protobuffer
	packet::PacketData packetData;
	bool success = packetData.ParseFromString(strPacketIn);

	if (!success)
	{
		return false;
	}

	dataTypeOut = packetData.datatype();
	dataOut = packetData.data();

	return true;
}