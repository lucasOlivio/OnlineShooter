#pragma once

#include <vector>
#include <string>

#define BUFFER_SIZE 512

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