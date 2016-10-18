#include "ComponentCamera.h"
#include "imgui\imgui.h"

ComponentCamera::ComponentCamera(ComponentType type, GameObject* game_object) : Component(type, game_object)
{}

ComponentCamera::~ComponentCamera()
{}

void ComponentCamera::OnInspector()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		//Near plane
		ImGui::Text("Near Plane: ");
		float near_value = near_plane;
		if (ImGui::SliderFloat("##near_p", &near_value, 0, 2000))
			SetNearPlane(near_value);

		//Far plane
		ImGui::Text("Far Plane: ");
		float far_value = far_plane;
		if (ImGui::SliderFloat("##far_p", &far_value, 0, 2000))
			SetFarPlane(far_value);

		//Field of view
		ImGui::Text("Field of view: ");
		float fov_value = fov;
		if (ImGui::SliderFloat("##fov", &fov_value, 0, 180))
			SetFOV(fov_value);
	}
}

void ComponentCamera::SetNearPlane(float value)
{
	if (value < far_plane)
		near_plane = value;
}

void ComponentCamera::SetFarPlane(float value)
{
	if (value > near_plane)
		far_plane = value;
}

void ComponentCamera::SetFOV(float value)
{
	//TODO: check for min -max values of fov
	fov = value;
}
