#pragma once

// For simplicity we keep the "inet_addr" for now so we need this flag
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// WinSock2 Windows Sockets
#define WIN32_LEAN_AND_MEAN

#include <vector>
#include <string>

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// Include win32 lib
#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 512
#define LOCALHOST "127.0.0.1"
#define DEFAULT_PORT 7777

typedef unsigned int uint;
typedef int int32;
typedef short int16;
typedef char int8;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

namespace myUDP
{
	struct sPacketData
	{
		sockaddr_in addr;
		int addrLen;
		std::string dataType;
		std::string data;
	};
}