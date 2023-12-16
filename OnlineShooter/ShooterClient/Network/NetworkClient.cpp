#include "NetworkClient.h"

#include <System/Input.h>
#include <System/Components/RigidBody.h>
#include <System/Components/Transform.h>
#include <System/Components/Network.h>
#include <shooter.pb.h>
#include <UDP/utils/common.h>

bool ClientSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
    m_nextSendTime = std::chrono::high_resolution_clock::now();

    m_pUDPClient = new UDPClient();
    bool udpInitialized = m_pUDPClient->Initialize(LOCALHOST, DEFAULT_PORT);
    if (!udpInitialized)
    {
        printf("ClientSystem: Failed to initialize UDP server!\n");
        return false;
    }

    // Get player id from server

    return true;
}

void ClientSystem::Execute(const std::vector<Entity*>& entities, float dt)
{
    m_HandleMsgs(entities, dt);
}

void ClientSystem::End()
{
    m_pUDPClient->Destroy();
}

void ClientSystem::m_HandleMsgs(const std::vector<Entity*>& entities, float dt)
{
    m_pUDPClient->ReadNewMsgs();

    std::vector<myUDP::sPacketData*> msgs;
    m_pUDPClient->GetNewMsgs(msgs);

    // Handle the new msgs
    for (myUDP::sPacketData* pPacket : msgs)
    {
        if (pPacket->dataType == "getid")
        {
            m_UpdatePlayerId(entities, pPacket->data);
            continue;
        }
        else if (pPacket->dataType == "userinput")
        {
            bool isHandled = m_HandleGameScene(entities, dt, pPacket->data);
            if (!isHandled)
            {
                printf("ClientSystem: error handling game scene from: \n");
                std::cout << "IP: " << inet_ntoa(pPacket->addr.sin_addr) << std::endl;
            }

            continue;
        }
    }

    m_pUDPClient->ResetMsgs();
}

void ClientSystem::m_SendUserInput(const std::vector<Entity*>& entities, float dt)
{
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    if (m_nextSendTime > currentTime)
    {
        return;
    }

    // 5Hz
    m_nextSendTime = currentTime + std::chrono::milliseconds(200);

    // Check if enough time has passed

    // Build user input proto

    // Send to server

    // Request id update
    m_nextRequestId += 1;
}

void ClientSystem::m_UpdatePlayerId(const std::vector<Entity*>& entities, 
						            const std::string& dataIn)
{
    // Search the entity id for this player
    // set owner to true and add player controller
}

bool ClientSystem::m_HandleGameScene(const std::vector<Entity*>& entities,
                                     float dt,
                                     const std::string& dataIn)
{
    // Go in each entity updating its position, orientation, velocity and state

    return true;
}
