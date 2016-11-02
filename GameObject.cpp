#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ModuleMeshes.h"


GameObject::GameObject()
{
	name.resize(30);
	name = "Empty GameObject";
	uuid = App->rnd->RandomInt();
}

GameObject::GameObject(GameObject* parent) : parent(parent)
{
	name.resize(30);
	name = "Empty GameObject";
	uuid = App->rnd->RandomInt();
}

GameObject::~GameObject()
{
	global_matrix = nullptr;
	for (std::vector<Component*>::iterator component = components.begin(); component != components.end(); ++component)
	{
		delete (*component);
		(*component) = nullptr;
	}

	components.clear();
	components_to_remove.clear();
}

void GameObject::PreUpdate()
{
	//Reset elements to draw
	mesh_to_draw = nullptr;
	texture_to_draw = 0;
	bounding_box = nullptr;

	//Remove all components that need to be removed. Secure way.
	for (std::vector<Component*>::iterator component = components_to_remove.begin(); component != components_to_remove.end(); ++component)
	{
		for (std::vector<Component*>::iterator comp_remove = components.begin(); comp_remove != components.end(); ++comp_remove) //Remove the component from the components list
		{
			if ((*comp_remove) == (*component))
			{
				components.erase(comp_remove);
				break;
			}
		}
		delete (*component);
	}

	components_to_remove.clear();
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

GameObject* GameObject::GetParent()const
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
		if(GetComponent(type) == nullptr) //Only one transform compoenent for gameobject
			item = new ComponentTransform(type, this, &global_matrix);
		break;
	case C_MESH:
		if(GetComponent(C_TRANSFORM))
			item = new ComponentMesh(type, this);
		break;
	case C_MATERIAL:
		if (GetComponent(C_TRANSFORM) && GetComponent(C_MESH))
			item = new ComponentMaterial(type, this);
		break;
	case C_CAMERA:
		if (GetComponent(C_TRANSFORM))
			item = new ComponentCamera(type, this);
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

void* GameObject::GetComponent(ComponentType type)const
{
	std::vector<Component*>::const_iterator comp = components.begin();

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

void GameObject::RemoveComponent(Component * component)
{
	//Search if the component exists inside the GameObject

	std::vector<Component*>::iterator it = components.begin();

	for (it; it != components.end(); ++it)
	{
		if ((*it) == component)
		{
			components_to_remove.push_back(component);
			break;
		}
	}
}

float4x4 GameObject::GetGlobalMatrix() const
{
	return *global_matrix;
}

unsigned int GameObject::GetUUID() const
{
	return uuid;
}

void GameObject::TransformModified()
{
	if (global_matrix == nullptr)
		return;
	std::vector<Component*>::iterator component = components.begin();

	for (component; component != components.end(); component++)
	{
		(*component)->OnTransformModified();
	}
}
