#include "NetworkServer.h"

#include <System/Input.h>
#include <System/Components/RigidBody.h>
#include <System/Components/Transform.h>
#include <System/Components/Network.h>
#include <shooter.pb.h>
#include <UDP/utils/common.h>

bool ServerSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
    m_pUDPServer = new UDPServer();
    bool udpInitialized = m_pUDPServer->Initialize(INADDR_ANY, DEFAULT_PORT);
    if (!udpInitialized)
    {
        printf("ServerSystem: Failed to initialize UDP server!\n");
        return false;
    }

    bool isListening = m_pUDPServer->StartListening();
    if (!isListening)
    {
        printf("ServerSystem: Failed to start listening!\n");
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

    m_pUDPServer->ResetMsgs();
}

void ServerSystem::m_BroadcastGameScene(const std::vector<Entity*>& entities, float dt)
{
    // Build game scene proto
    shooter::GameScene gamescene;
    gamescene.set_requestid(m_nextRequestId);

    for (int i = 0; i < entities.size(); i++)
    {
        Entity* pEntity = entities[i];

        if (!pEntity->HasComponent<NetworkComponent>())
        {
            continue;
        }
        TransformComponent* pTransform = pEntity->GetComponent<TransformComponent>();
        NetworkComponent* pNetwork = pEntity->GetComponent<NetworkComponent>();

        // Setup entity
        shooter::Entity* entity = gamescene.add_entities();
        entity->set_entityid(i);
        entity->set_state(pNetwork->state);

        // Setup Position
        shooter::Vector3* position = entity->mutable_position();
        position->set_x(pTransform->position.x);
        position->set_y(pTransform->position.y);
        position->set_z(pTransform->position.z);

        // Setup orientation
        shooter::Vector3* orientation = entity->mutable_orientation();
        orientation->set_x(pTransform->orientation.x);
        orientation->set_y(pTransform->orientation.y);
        orientation->set_z(pTransform->orientation.z);

        // Setup velocity
        // Velocity should not be mandatory but proto2 is bugged with optional fields
        glm::vec3 enttVel = glm::vec3(0);
        if (pEntity->HasComponent<RigidBodyComponent>())
        {
            RigidBodyComponent* pRigidbody = pEntity->GetComponent<RigidBodyComponent>();
            enttVel = pRigidbody->velocity;
        }

        shooter::Vector3* velocity = entity->mutable_velocity();
        velocity->set_x(enttVel.x);
        velocity->set_y(enttVel.y);
        velocity->set_z(enttVel.z);
    }

    if (gamescene.entities_size() == 0)
    {
        return;
    }

    std::string gsSerialized;
    gamescene.SerializeToString(&gsSerialized);

    // Iterate over each client and send the updated gamescene
    std::vector<sClientInfo*> clients;
    m_pUDPServer->GetConnectedClients(clients);
    for (sClientInfo* pClient : clients)
    {
        m_pUDPServer->SendRequest("gamescene", gsSerialized, pClient->addr, pClient->addrLen);
    }

    // Request id update
    m_nextRequestId += 1;
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
