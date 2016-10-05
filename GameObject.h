#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent);
	~GameObject();

	bool AddChild(GameObject* child);
	bool RemoveChild(GameObject* child);
	void RemoveAllChilds();
	GameObject* GetParent();
	const std::vector<GameObject*>* GetChilds();
	size_t ChildCount();


public:
	std::string name = "Empty GameObject";

private:
	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;
};

#endif // !__GAMEOBJECT_H__
