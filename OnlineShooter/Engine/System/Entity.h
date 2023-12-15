#pragma once

#include <common.h>

#include <system/component.h>

class Entity
{
public:
	Entity() { }
	~Entity() {
		for (auto& pair : m_Components) {
			delete pair.second;
		}
	}

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
