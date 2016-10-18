#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"

struct Mesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(ComponentType type, GameObject* game_object);
	~ComponentMesh();

	void Update(float dt);

	void OnInspector();

	bool SetMesh(Mesh* mesh);
	void RecalculateBoundingBox();

private:
	Mesh* mesh = nullptr;
	math::AABB bounding_box;
};


#endif // !__COMPONENTMESH_H__
