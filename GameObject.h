#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "MathGeoLib\include\MathGeoLib.h"

class Component;
enum ComponentType;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent);
	~GameObject();

	void PreUpdate();
	void Update(float dt);

	bool AddChild(GameObject* child);
	bool RemoveChild(GameObject* child);
	void RemoveAllChilds();
	GameObject* GetParent();
	const std::vector<GameObject*>* GetChilds();
	size_t ChildCount();

	bool IsActive();
	void SetActive(bool value);

	Component* AddComponent(ComponentType type);
	const std::vector<Component*>* GetComponents();
	void* GetComponent(ComponentType type);
	void RemoveComponent(Component* component);

	float4x4 GetGlobalMatrix()const;

	void TransformModified();

public:
	std::string name;

private:
	GameObject* parent = NULL;
	std::vector<GameObject*> childs;

	bool active = true;

	std::vector<Component*> components;
	std::vector<Component*> components_to_remove;

	float4x4* global_matrix = nullptr;
};

#endif // !__GAMEOBJECT_H__
