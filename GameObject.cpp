#include "GameObject.h"
#include "Application.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject()
{
	name.resize(30);
	name = "Empty GameObject";
}

GameObject::GameObject(GameObject* parent) : parent(parent)
{
	name.resize(30);
	name = "Empty GameObject";
}

GameObject::~GameObject()
{
	for (std::vector<Component*>::iterator component = components.begin(); component != components.end(); ++component)
	{
		delete (*component);
		(*component) = nullptr;
	}

	components.clear();
}

void GameObject::Update(float dt)
{
	std::vector<Component*>::iterator comp = components.begin();

	for (comp; comp != components.end(); comp++)
	{
		(*comp)->Update(dt);
	}
}

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

bool GameObject::IsActive()
{
	return active;
}

void GameObject::SetActive(bool value)
{
	if (value == true && parent->IsActive() == false)
		return;

	if ((value == true && active == false) || (value == false && active == true))
	{
		active = value;

		for (std::vector<GameObject*>::iterator child = childs.begin(); child != childs.end(); ++child)
			(*child)->SetActive(value);
	}
}

Component* GameObject::AddComponent(ComponentType type)
{
	Component* item = nullptr;

	switch (type)
	{
	case C_TRANSFORM:
		item = new ComponentTransform(type, this);
		break;
	case C_MESH:
		item = new ComponentMesh(type, this);
		break;
	case C_MATERIAL:
		item = new ComponentMaterial(type, this);
		break;
	default:
		break;
	}

	if (item != nullptr)
	{
		components.push_back(item);
	}
	else
	{
		LOG("Error while adding component to %s", this->name);
	}
		

	return item;
}

const std::vector<Component*>* GameObject::GetComponents()
{
	return &components;
}

void* GameObject::GetComponent(ComponentType type)
{
	std::vector<Component*>::iterator comp = components.begin();

	while (comp != components.end())
	{
		if ((*comp)->GetType() == type)
		{
			return (*comp);
		}
		++comp;
	}

	return NULL;
}