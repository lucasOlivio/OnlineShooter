#pragma once

#include "UDPBase.h"

class UDPClient : public UDPBase
{
public:
	// ctors & dtors
	UDPClient();
	virtual ~UDPClient();

	bool Initialize(const char* host, int port);

	// Read all messages incomming from server
	void ReadNewMsgs();

protected:
	int m_infoLen;
};