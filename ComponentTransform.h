#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(ComponentType type, GameObject* game_object);
	~ComponentTransform();

	void Update(float dt);

	void OnInspector();

	void SetPosition(math::float3 pos);
	void SetRotation(math::float3 rot_euler);
	void SetRotation(math::Quat rot);
	void SetScale(math::float3 scale);

	math::float3 GetPosition();
	math::float3 GetRotationEuler();
	math::Quat GetRotation();
	math::float3 GetScale();

	// Returns the final transformation matrix. Not the local one!
	math::float4x4 GetTransformMatrix();

private:

	void CalculateFinalTransform();

private:
	math::float3 position = math::float3::zero;
	math::Quat rotation = math::Quat::identity;
	math::float3 scale = math::float3::one;

	math::float3 rotation_euler = math::float3::zero;

	math::float4x4 transform_matrix = math::float4x4::identity;
	math::float4x4 final_transform_matrix = math::float4x4::identity;
};

#endif // !__COMPONENT_TRANSFORM_H__
