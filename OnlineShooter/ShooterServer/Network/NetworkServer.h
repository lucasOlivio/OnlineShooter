#pragma once

#include <System/System.h>

#include <UDP/UDPServer.h>

#include <chrono>
#include <deque>

class ServerSystem : public iSystem
{
public:
	ServerSystem() {};
	virtual ~ServerSystem() { };

	virtual bool Start(const std::vector<Entity*>& entities, int argc, char** argv);

	virtual void Execute(const std::vector<Entity*>& entities, float dt);

	virtual void End();

private:
	UDPServer* m_pUDPServer;

	// Mapping clientid -> playerir
	std::map<int /*clientid*/, int /*playerid*/> m_clientplayer;

	// Request id for game scene updates
	std::map<int /*entity ids*/, int /*next requestid*/> m_nextRequestIds;

	std::deque<int> m_availablePlayerIds;
	std::chrono::high_resolution_clock::time_point m_nextSendTime;

	void m_HandleMsgs(const std::vector<Entity*>& entities, float dt);
	void m_BroadcastGameScene(const std::vector<Entity*>& entities, float dt);

	// Updates cache controls and return next available id
	int m_AddPlayer(sockaddr_in& addr, int& addrLen, int clientId);
	// Return the player id to the list of available ids
	void m_RemovePlayer(sockaddr_in addr, int addrLen, int clientId);

	void m_SendNextId(int nextPlayerId, sockaddr_in addrIn, int addrLenIn);
	bool m_HandleUserInput(const std::string& dataIn);
};
