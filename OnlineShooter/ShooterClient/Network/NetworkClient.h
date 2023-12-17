#pragma once

#include <System/System.h>

#include <UDP/UDPClient.h>

#include <chrono>

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

	// Player id to define which entity to control
	int m_playerId = -1;

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
						   std::string& data);

	void m_GetSerializedUserInputProto(int requestid, int playerid, 
									   int input, bool isPressed,
									   std::string& serializedOut);

};
