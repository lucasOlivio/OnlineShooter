#pragma once

#include <System/System.h>

#include <UDP/UDPClient.h>

#include <chrono>

enum eEntityState
{
	IS_ACTIVE,
	IS_CONNECTED,
	HAS_AMMO,
	IS_DEAD
};

const int MAX_PLAYERS = 4;

class ClientSystem : public iSystem
{
public:
	ClientSystem() {};
	virtual ~ClientSystem() { };

	virtual bool Start(const std::vector<Entity*>& entities, int argc, char** argv);

	virtual void Execute(const std::vector<Entity*>& entities, float dt);

	virtual void End();

private:
	UDPClient* m_pUDPClient;

	// Request id for user inputs
	int m_nextRequestId;

	int m_nextAvailableId = 0;
	std::chrono::high_resolution_clock::time_point m_nextSendTime;

	void m_HandleMsgs(const std::vector<Entity*>& entities, float dt);
	void m_SendUserInput(const std::vector<Entity*>& entities, float dt);

	void m_UpdatePlayerId(const std::vector<Entity*>& entities, 
						  const std::string& dataIn);
	bool m_HandleGameScene(const std::vector<Entity*>& entities, 
						   float dt, 
						   const std::string& dataIn);
};
