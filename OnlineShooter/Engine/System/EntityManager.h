#pragma once

#include <common.h>

#include <system/component.h>
#include <system/entity.h>

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    void GetEntities(std::vector<Entity*>& entities) const;

    Entity* CreateEntity();
    void RemoveEntity(Entity* entity);

private:
    std::vector<Entity*> m_Entities = {};
};
