#pragma once

#include "UDPBase.h"

class UDPClient : public UDPBase
{
public:
	// ctors & dtors
	UDPClient();
	virtual ~UDPClient();

	bool Initialize(const char* host, const char* port);
	void Destroy();

	bool Connect();

	// Set the socket to be blocking or not
	bool SetBlocking(u_long mode);

	// Wait "tv_sec" for msg from server
	bool CheckMsgFromServer(int tv_sec);
};