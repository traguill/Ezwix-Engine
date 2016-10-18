#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(ComponentType type, GameObject* game_object);
	~ComponentCamera();

	void OnInspector();

	void SetNearPlane(float value);
	void SetFarPlane(float value);
	void SetFOV(float value);

private:
	float near_plane = 0.3f;
	float far_plane = 1000.0f;
	float fov = 60;
	float ratio_x;
	float ratio_y;
	float aspect_ratio;

};
#endif // !__COMPONENT_MATERIAL_H__