#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>

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

	void TransformModified();

public:
	std::string name;

private:
	GameObject* parent = NULL;
	std::vector<GameObject*> childs;

	bool active = true;

	std::vector<Component*> components;

	std::vector<Component*> components_to_remove;
};

#endif // !__GAMEOBJECT_H__
