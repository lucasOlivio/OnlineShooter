#include "NetworkClient.h"

#include <System/Input.h>
#include <System/Components/RigidBody.h>
#include <System/Components/Transform.h>
#include <System/Components/Network.h>
#include <System/Components/PlayerController.h>
#include <System/Components/BulletController.h>
#include <shooter.pb.h>
#include <UDP/utils/common.h>
#include <Engine/Engine.h>

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
    shooter::GetId getid;
    getid.set_playerid(m_playerId);

    sockaddr_in addrServer = m_pUDPClient->GetInfo();
    std::string getidSerialized;
    getid.SerializeToString(&getidSerialized);

    m_pUDPClient->SendRequest("getid", getidSerialized, 
                              addrServer, sizeof(addrServer));
    printf("waiting for id...\n");

    return true;
}

void ClientSystem::Execute(const std::vector<Entity*>& entities, float dt)
{
    m_HandleMsgs(entities, dt);
    m_SendUserInput(entities, dt);
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
        else if (pPacket->dataType == "gamescene")
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
    // Check if enough time has passed
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    if (m_nextSendTime > currentTime)
    {
        return;
    }

    // 5Hz
    m_nextSendTime = currentTime + std::chrono::milliseconds(200);

    if (m_playerId < 0 || m_playerId > entities.size())
    {
        return;
    }

    Entity* pEntity = entities[m_playerId];
    
    if (!pEntity->HasComponent<PlayerControllerComponent>())
    {
        return;
    }

    PlayerControllerComponent* pPlayer = pEntity->GetComponent<PlayerControllerComponent>();

    // Request id update
    m_nextRequestId += 1;

    std::vector<std::string> inputsToSend = {};

    // HACK: We should keep a before state of the inputs so we could send only the changes,
    // but no time now
  
    // Build forward input
    std::string serializedInput;
    m_GetSerializedUserInputProto(m_nextRequestId, m_playerId,
                                  shooter::UserInput::FORWARD, pPlayer->moveForward, 
                                  serializedInput);
    inputsToSend.push_back(serializedInput);

    // Build backward input
    m_GetSerializedUserInputProto(m_nextRequestId, m_playerId,
                                  shooter::UserInput::BACKWARD, pPlayer->moveBackward,
                                  serializedInput);

    inputsToSend.push_back(serializedInput);

    // Build left input
    m_GetSerializedUserInputProto(m_nextRequestId, m_playerId,
                                  shooter::UserInput::TURN_LEFT, pPlayer->moveLeft,
                                  serializedInput);

    inputsToSend.push_back(serializedInput);

    // Build right input
    m_GetSerializedUserInputProto(m_nextRequestId, m_playerId,
        shooter::UserInput::TURN_RIGHT, pPlayer->moveRight,
        serializedInput);

    inputsToSend.push_back(serializedInput);

    // Build shoot input
    m_GetSerializedUserInputProto(m_nextRequestId, m_playerId,
        shooter::UserInput::FIRE, pPlayer->shoot,
        serializedInput);

    inputsToSend.push_back(serializedInput);

    if (inputsToSend.size() == 0)
    {
        // No inputs to send
        // reset request id 
        m_nextRequestId -= 1;
        return;
    }

    // Send to server
    sockaddr_in addrServer = m_pUDPClient->GetInfo();
    int addrLen = sizeof(addrServer);
    for (std::string serializedInput : inputsToSend)
    {
        m_pUDPClient->SendRequest("userinput", serializedInput,
                                  addrServer, addrLen);
    }
}

void ClientSystem::m_UpdatePlayerId(const std::vector<Entity*>& entities, 
						            const std::string& dataIn)
{
    shooter::GetId getit;
    bool isDeserialized = getit.ParseFromString(dataIn);
    if (!isDeserialized)
    {
        printf("ClientSystem: Error deserializing getid!");
        return;
    }

    m_playerId = getit.playerid();

    if (m_playerId < 0)
    {
        printf("Server full! Try again later!\n");
        GetEngine().SetRunning(false);
        return;
    }

    printf("Received id %d, setting playable entity...\n", m_playerId);

    // Search the entity id for this player
    Entity* pEntity = entities[m_playerId];
    
    // set owner to true and add player controller
    pEntity->AddComponent<NetworkComponent>(true);
    pEntity->AddComponent<PlayerControllerComponent>();
}

bool ClientSystem::m_HandleGameScene(const std::vector<Entity*>& entities,
                                     float dt,
                                     std::string& data)
{
    shooter::GameScene gamescene;
    bool isDeserialized = gamescene.ParseFromString(data);
    if (!isDeserialized)
    {
        printf("ClientSystem: Error deserializing gamescene!\n");
        return false;
    }

    // Go in each entity updating its position, 
    // orientation, velocity and state to match the server
    for (shooter::Entity entity : gamescene.entities())
    {
        int entityId = entity.entityid();
        int requestId = entity.requestid();
        int state = entity.state();

        Entity* pLocalEntity = entities[entityId];

        // Update state
        pLocalEntity->state = (StatetType)state;

        // Reconciliation
        // only update trivial info if the requestid is not previous (already calculated)
        if (requestId <= m_nextRequestId)
        {
            continue;
        }

        glm::vec3 position = glm::vec3(entity.position().x(),
                                       entity.position().y(),
                                       entity.position().z());
        glm::quat orientation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
        orientation.x = entity.orientation().x();
        orientation.y = entity.orientation().y();
        orientation.z = entity.orientation().z();
        orientation.w = entity.orientation().w();
        glm::vec3 velocity = glm::vec3(entity.velocity().x(),
                                       entity.velocity().y(),
                                       entity.velocity().z());

        // Update transform
        TransformComponent* pTransform = pLocalEntity->GetComponent<TransformComponent>();
        pTransform->position = position;
        pTransform->orientation = orientation;
        
        // Update RigidBody
        if (!pLocalEntity->HasComponent<RigidBodyComponent>())
        {
            continue;
        }
        RigidBodyComponent* pBody = pLocalEntity->GetComponent<RigidBodyComponent>();
        pBody->velocity = velocity;
    }

    return true;
}

void ClientSystem::m_GetSerializedUserInputProto(int requestid, int playerid, 
                                                 int input, bool isPressed,
                                                 std::string& serializedOut)
{
    shooter::UserInput userinput;
    userinput.set_playerid(m_playerId);
    userinput.set_requestid(m_nextRequestId);
    userinput.set_input((shooter::UserInput::InputType)input);
    userinput.set_ispressed(isPressed);

    userinput.SerializeToString(&serializedOut);

    return;
}
