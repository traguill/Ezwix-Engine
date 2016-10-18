#include "Application.h"
#include "ComponentCamera.h"
#include "imgui\imgui.h"
#include "DebugDraw.h"
#include "GameObject.h"
#include "ComponentTransform.h"

ComponentCamera::ComponentCamera(ComponentType type, GameObject* game_object) : Component(type, game_object)
{
	//Init frustrum
	float vertical_fov = DegToRad(fov);
	float horizontal_fov = 2.0f*atanf(tanf(vertical_fov / 2.0f) * aspect_ratio);

	frustum.SetPerspective(horizontal_fov, vertical_fov);

	frustum.SetPos(float3::zero);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	frustum.SetViewPlaneDistances(near_plane, far_plane);
	frustum.SetVerticalFovAndAspectRatio(DegToRad(fov), aspect_ratio);

	color = float3(0, 0, 0); //Black to clear the screen by default
}

ComponentCamera::~ComponentCamera()
{}

void ComponentCamera::Update(float dt)
{
	//Draw Frustrum
	float planes_dist = 5.0f; //Note: the far plane position is faked to apreciate the depth of the camera. A better implementation would be to use the near-far relation to modify this value.
	float3 near_pos = frustum.Pos() + frustum.Front() * frustum.NearPlaneDistance();
	g_Debug->AddRect(near_pos, frustum.Front(), float2(frustum.NearPlaneWidth(), frustum.NearPlaneHeight()), g_Debug->blue);

	float3 far_pos = frustum.Pos() + frustum.Front() * planes_dist;
	g_Debug->AddRect(far_pos, frustum.Front(), float2(frustum.NearPlaneWidth()*planes_dist, frustum.NearPlaneHeight()*planes_dist), g_Debug->blue);

}

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

		ImGui::Text("Aspect ratio: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 0, 1, 1), "%d:%d", ratio_x, ratio_y);
	}
}

void ComponentCamera::OnTransformModified()
{
	GameObject* game_object = GetGameObject();

	if (game_object)
	{
		ComponentTransform* trans = (ComponentTransform*)game_object->GetComponent(C_TRANSFORM);
		
		float4x4 matrix = trans->GetGlobalMatrix();

		frustum.SetPos(matrix.TranslatePart());
		frustum.SetFront(matrix.WorldZ());
		frustum.SetUp(matrix.WorldY());
	}
	else
		LOG("Error: Component Camera is trying to update it's matrix but it is not attached to any game object.");

}

void ComponentCamera::SetNearPlane(float value)
{
	if (value < far_plane && value > 0.0f)
		near_plane = value;
	frustum.SetViewPlaneDistances(near_plane, far_plane);
}

void ComponentCamera::SetFarPlane(float value)
{
	if (value > near_plane)
		far_plane = value;

	frustum.SetViewPlaneDistances(near_plane, far_plane);
}

void ComponentCamera::SetFOV(float value)
{
	//TODO: check for min -max values of fov
	fov = value;

	frustum.SetVerticalFovAndAspectRatio(DegToRad(fov), aspect_ratio);
}
