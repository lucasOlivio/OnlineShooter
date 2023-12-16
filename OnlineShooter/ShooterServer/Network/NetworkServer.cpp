#include "NetworkServer.h"

#include <System/Input.h>
#include <shooter.pb.h>
#include <UDP/utils/common.h>

const int MAX_PLAYERS = 4;

bool ServerSystem::Start(const std::vector<Entity*>& entities)
{
    m_pUDPServer = new UDPServer();
    bool udpInitialized = m_pUDPServer->Initialize(LOCALHOST, DEFAULT_PORT);
    if (!udpInitialized)
    {
        printf("ServerSystem: Failed to initialize UDP server!\n");
        return false;
    }

    return true;
}

void ServerSystem::Execute(const std::vector<Entity*>& entities, float dt)
{
    m_HandleMsgs(entities, dt);
    m_BroadcastGameScene(entities, dt);
}

void ServerSystem::End()
{
    m_pUDPServer->Destroy();
}

void ServerSystem::m_HandleMsgs(const std::vector<Entity*>& entities, float dt)
{
    m_pUDPServer->ReadNewMsgs();

    std::vector<myUDP::sPacketData*> msgs;
    m_pUDPServer->GetNewMsgs(msgs);

    // Handle the new msgs
    for (myUDP::sPacketData* pPacket : msgs)
    {
        if (pPacket->dataType == "getid")
        {
            m_SendNextId(pPacket->addr, pPacket->addrLen);
            continue;
        }
        else if (pPacket->dataType == "userinput")
        {
            bool isHandled = m_HandleUserInput(pPacket->data);

            if (!isHandled)
            {
                printf("ServerSystem: error handling user input from: \n");
                std::cout << "IP: " << inet_ntoa(pPacket->addr.sin_addr) << std::endl;
            }
            continue;
        }
    }
}

void ServerSystem::m_BroadcastGameScene(const std::vector<Entity*>& entities, float dt)
{
    // Build game scene proto

    // Iterate over each client and send the updated gamescene  
}

void ServerSystem::m_SendNextId(const sockaddr_in& addrIn, const int& addrLenIn)
{
    if (m_nextAvailableId >= MAX_PLAYERS)
    {
        // Max players reached, no more connections allowed
        m_pUDPServer->Removeclient(addrIn, addrLenIn);
        return;
    }

    shooter::GetId getid;
    std::string responseSerialized;

    getid.set_playerid(m_nextAvailableId);
  
    getid.SerializeToString(&responseSerialized);

    m_pUDPServer->SendRequest("getid", responseSerialized, addrIn, addrLenIn);

    m_nextAvailableId += 1;
}

bool ServerSystem::m_HandleUserInput(const std::string& dataIn)
{
    shooter::UserInput newInput;
    bool isDeserialized = newInput.ParseFromString(dataIn);
    if (!isDeserialized)
    {
        printf("ServerSystem: Error deserializing msg!");
        return false;
    }

    eInputType input = (eInputType)newInput.input();
    if (input == eInputType::NONE)
    {
        return true;
    }

    // If input forward, apply velocity forward to the entity direction

    // If input backward, apply velocity backwards to the entity direction

    // If input turn left, rotate entity to the left

    // If input turn right, rotate entity to the right

    // If input action, create a bullet entity in front of entity
    // with a velocity relative to the forward direction of entity

    return true;
}
