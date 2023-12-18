#pragma once

#include <common.h>

#include <system/component.h>

// HACK: Copy from shooter.proto, this should have a common way of state control
// without the need of duplicating the information
enum StatetType {
	NOT_ACTIVE,
	IS_ACTIVE,
	IS_CONNECTED,
	HAS_AMMO,
	IS_DEAD,
};

class Entity
{
public:
	Entity() { 
		//this->nextUniqueId = startinguniqueId;
		//std::cout << uniqueid << std::endl;
	}
	~Entity() {}

	StatetType state;
	std::string tag;
	int id; // player and bullet will have matching ids

	template<typename T>
	bool HasComponent() const
	{
		return m_Components.find(&typeid(T)) != m_Components.end();
	}

	template<typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		if (HasComponent<T>())
		{
			return GetComponent<T>();
		}

		T* newComponent = new T(std::forward<Args>(args)...);
		m_Components[&typeid(T)] = newComponent;
		//Entity::nextUniqueId++;
		return newComponent;
	}

	template <typename T>
	void RemoveComponent()
	{
		auto it = m_Components.find(&typeid(T));
		if (it != m_Components.end())
		{
			delete it->second;
			m_Components.erase(it);
		}
	}

	template <typename T>
	T* GetComponent() const
	{
		auto it = m_Components.find(&typeid(T));
		if (it != m_Components.end())
		{
			return dynamic_cast<T*>(it->second);
		}
		return nullptr;
	}

private:
	std::map<const std::type_info*, Component*> m_Components;
};
