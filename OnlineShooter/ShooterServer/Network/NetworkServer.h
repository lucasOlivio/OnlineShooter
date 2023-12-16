#pragma once

#include <System/System.h>
#include <System/Components/RigidBody.h>
#include <System/Components/Transform.h>

#include <UDP/UDPServer.h>

#include <chrono>

enum eEntityState
{
	IS_ACTIVE, 
	IS_CONNECTED, 
	HAS_AMMO, 
	IS_DEAD
};

class ServerSystem : iSystem
{
public:
	ServerSystem() {};
	virtual ~ServerSystem() { };

	virtual bool Start(const std::vector<Entity*>& entities);

	virtual void Execute(const std::vector<Entity*>& entities, float dt);

	virtual void End();

private:
	UDPServer* m_pUDPServer;

	// Request id for game scene updates
	int m_nextRequestId;

	int m_nextAvailableId = 0;
	std::chrono::high_resolution_clock::time_point m_nextSendTime;

	void m_HandleMsgs(const std::vector<Entity*>& entities, float dt);
	void m_BroadcastGameScene(const std::vector<Entity*>& entities, float dt);

	void m_SendNextId(const sockaddr_in& addrIn, const int& addrLenIn);
	bool m_HandleUserInput(const std::string& dataIn);
};
