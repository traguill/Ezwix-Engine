#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"

struct Mesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(ComponentType type, GameObject* game_object);
	~ComponentMesh();

	void Update(float dt);

	void OnInspector();

	bool SetMesh(Mesh* mesh);

private:
	Mesh* mesh = nullptr;
};


#endif // !__COMPONENTMESH_H__
