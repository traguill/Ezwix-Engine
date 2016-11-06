#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>
#include "MathGeoLib\include\MathGeoLib.h"

class Component;
enum ComponentType;
struct Mesh;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent);
	GameObject(const char* name, unsigned int uuid, GameObject* parent, bool active);
	~GameObject();

	void PreUpdate();
	void Update(float dt);

	bool AddChild(GameObject* child);
	bool RemoveChild(GameObject* child); //Breaks the link with the parent but does not delete the child.
	void RemoveAllChilds();
	GameObject* GetParent()const;
	const std::vector<GameObject*>* GetChilds();
	size_t ChildCount();

	bool IsActive();
	void SetActive(bool value);

	Component* AddComponent(ComponentType type);
	const std::vector<Component*>* GetComponents();
	void* GetComponent(ComponentType type)const;
	void RemoveComponent(Component* component);

	float4x4 GetGlobalMatrix()const;
	unsigned int GetUUID()const;

	void TransformModified();

	void Save(Data& file) const;

public:
	std::string name;
	Mesh* mesh_to_draw = nullptr; //Pointer to the mesh to draw in one frame
	AABB* bounding_box = nullptr;
	unsigned int texture_to_draw = 0; //Texture to draw in one frame

private:
	GameObject* parent = NULL;
	std::vector<GameObject*> childs;

	bool active = true;

	std::vector<Component*> components;
	std::vector<Component*> components_to_remove;

	float4x4* global_matrix = nullptr;

	unsigned int uuid = 0;
};

#endif // !__GAMEOBJECT_H__
