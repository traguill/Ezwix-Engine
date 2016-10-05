#include "GameObject.h"
#include "Application.h"

GameObject::GameObject()
{}

GameObject::GameObject(GameObject* parent) : parent(parent)
{}

GameObject::~GameObject()
{}

bool GameObject::AddChild(GameObject* child)
{
	bool ret = false;
	if (child)
	{
		childs.push_back(child);
		ret = true;
	}

	return ret;
}

bool GameObject::RemoveChild(GameObject* child)
{
	bool ret = false;

	if (child)
	{
		std::vector<GameObject*>::iterator item = childs.begin();

		while (item != childs.end())
		{
			if (*item == child)
			{
				childs.erase(item);
				ret = true;
				break;
			}
			++item;
		}
	}

	return ret;
}

void GameObject::RemoveAllChilds()
{
	for (int i = 0; i < childs.size(); i++)
	{
		App->go_manager->RemoveGameObject(childs[i]);
	}

	childs.clear();
}

GameObject* GameObject::GetParent()
{
	return parent;
}

const std::vector<GameObject*>* GameObject::GetChilds()
{
	return &childs;
}

size_t GameObject::ChildCount()
{
	return childs.size();
}