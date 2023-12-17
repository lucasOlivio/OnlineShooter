#include "NetworkServer.h"

#include <System/Input.h>
#include <System/components.h>
#include <Engine/Engine.h>
#include <shooter.pb.h>
#include <UDP/utils/common.h>

bool ServerSystem::Start(const std::vector<Entity*>& entities, int argc, char** argv)
{
    m_availablePlayerIds = { 0, 1, 2, 3 };

    m_nextSendTime = std::chrono::high_resolution_clock::now();

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
    sockaddr_in addr;
    int addrLen = sizeof(addr);
    int clientId;

    int result = m_pUDPServer->ReadNewMsgs(addr, addrLen, clientId);
    if (result == WSAECONNRESET)
    {
        // Client disconnected
        m_RemovePlayer(entities, addr, addrLen, clientId);

        return;
    }

    std::vector<myUDP::sPacketData*> msgs;
    m_pUDPServer->GetNewMsgs(msgs);

    // Handle the new msgs
    for (myUDP::sPacketData* pPacket : msgs)
    {
        if (pPacket->dataType == "getid")
        {
            int nextPlayerId = m_AddPlayer(entities, pPacket->addr, pPacket->addrLen, clientId);
            m_SendNextId(entities, nextPlayerId, pPacket->addr, pPacket->addrLen);
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
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    if (m_nextSendTime > currentTime)
    {
        return;
    }

    // 5Hz
    m_nextSendTime = currentTime + std::chrono::milliseconds(200);

    // Build game scene proto
    shooter::GameScene gamescene;

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

        // We have one requestid follow for each player
        entity->set_requestid(m_nextRequestIds[i]);

        entity->set_state((shooter::Entity::StatetType)pEntity->state);

        // Setup Position
        shooter::Vector3* position = entity->mutable_position();
        position->set_x(pTransform->position.x);
        position->set_y(pTransform->position.y);
        position->set_z(pTransform->position.z);

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

        // Setup orientation
        shooter::Vector3* orientation = entity->mutable_orientation();
        orientation->set_x(pTransform->orientation.x);
        orientation->set_y(pTransform->orientation.y);
        orientation->set_z(pTransform->orientation.z);
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
}

int ServerSystem::m_AddPlayer(const std::vector<Entity*>& entities, sockaddr_in& addr, 
                              int& addrLen, int clientId)
{
    if (m_availablePlayerIds.size() <= 0)
    {
        // Max players reached, no more connections allowed
        m_pUDPServer->Removeclient(addr, addrLen);
        return -1;
    }

    int nextPlayerId = m_availablePlayerIds.front();

    // Update entity state
    entities[nextPlayerId]->state = StatetType::IS_CONNECTED;

    // Update cache control of clienid->playerid
    m_clientplayer[clientId] = nextPlayerId;

    // Initializes the request id for this player
    m_nextRequestIds[nextPlayerId] = 0;

    // Update next player id available
    m_availablePlayerIds.pop_front();

    return nextPlayerId;
}

void ServerSystem::m_RemovePlayer(const std::vector<Entity*>& entities, sockaddr_in addr, 
                                  int addrLen, int clientId)
{
    // Return the player id to the list of available ids
    int availableId = m_clientplayer[clientId];
    Entity* pEntity = entities[availableId];

    if (pEntity->state == StatetType::NOT_ACTIVE)
    {
        // Player already removed;
        return;
    }

    // Update entity state
    entities[availableId]->state = StatetType::NOT_ACTIVE;

    m_availablePlayerIds.push_back(availableId);

    // Remove client from cache client-player
    m_clientplayer.erase(clientId);

    return;
}

void ServerSystem::m_SendNextId(const std::vector<Entity*>& entities, int nextPlayerId, 
                                sockaddr_in addrIn, int addrLenIn)
{
    shooter::GetId getid;
    std::string responseSerialized;

    getid.set_playerid(nextPlayerId);
  
    getid.SerializeToString(&responseSerialized);

    // Send player id to client
    m_pUDPServer->SendRequest("getid", responseSerialized, addrIn, addrLenIn);

    if (nextPlayerId < 0)
    {
        return;
    }

    // Update entity state
    entities[nextPlayerId]->state = StatetType::IS_ACTIVE;

    return;
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

    int playerId = newInput.playerid();
    shooter::UserInput::InputType playerInput = newInput.input();

    std::vector<Entity*> entities;
    GetEngine().GetEntityManager()->GetEntities(entities);

    // For every input received we set the flag to the correspondent entity
    Entity* pEntity = entities[playerId];
    PlayerControllerComponent* pPlayer = pEntity->GetComponent<PlayerControllerComponent>();

    if (playerInput == shooter::UserInput::FORWARD)
    {
        pPlayer->moveForward = newInput.ispressed();
    }
    else if (playerInput == shooter::UserInput::BACKWARD)
    {
        pPlayer->moveBackward = newInput.ispressed();
    }
    else if (playerInput == shooter::UserInput::TURN_LEFT)
    {
        pPlayer->moveLeft = newInput.ispressed();
    }
    else if (playerInput == shooter::UserInput::TURN_RIGHT)
    {
        pPlayer->moveRight = newInput.ispressed();
    }
    else if (playerInput == shooter::UserInput::FIRE)
    {
        pPlayer->shoot = newInput.ispressed();
    }

    // Update next request id for this player
    m_nextRequestIds[playerId] = newInput.requestid();

    printf("Next request id for player %d: %d", playerId, m_nextRequestIds[playerId]);

    return true;
}
