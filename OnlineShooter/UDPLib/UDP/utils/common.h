#pragma once

#include <vector>
#include <string>

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
		std::string dataType;
		std::string data;
	};
}