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
	void OnTransformModified();

	bool SetMesh(Mesh* mesh);
	void RecalculateBoundingBox();

	void Save(Data& file)const;

private:
	Mesh* mesh = nullptr;
	math::AABB aabb; //Local one
	math::AABB bounding_box; //In the world position
};


#endif // !__COMPONENTMESH_H__
