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
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum.SetPos(float3::zero);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	frustum.SetViewPlaneDistances(near_plane, far_plane);
	frustum.SetVerticalFovAndAspectRatio(DegToRad(fov), aspect_ratio);

	color = float3(0, 0, 0); //Black to clear the screen by default
}

ComponentCamera::~ComponentCamera()
{}

void ComponentCamera::Update()
{
	g_Debug->AddFrustum(frustum, 30.0f, g_Debug->blue, 2.0f);
}

void ComponentCamera::OnInspector()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		//Near plane
		ImGui::Text("Near Plane: ");
		float near_value = near_plane;
		if (ImGui::SliderFloat("##near_p", &near_value, 0, 1, "%.3f", 0.05f))
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

		ImGui::Text("Background color: "); ImGui::SameLine();
		float3 color = this->color;
		if (ImGui::ColorEdit3("", color.ptr()))
		{
			this->color = color;
		}

		if (ImGui::Button("###cam_rem Remove"))
		{
			Remove();
		}
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

float ComponentCamera::GetNearPlane() const
{
	return near_plane;
}

float ComponentCamera::GetFarPlane() const
{
	return far_plane;
}

float ComponentCamera::GetFOV() const
{
	return fov;
}

math::float3 ComponentCamera::GetFront() const
{
	return frustum.Front();
}

math::float3 ComponentCamera::GetUp() const
{
	return frustum.Up();
}

math::float4x4 ComponentCamera::GetProjectionMatrix() const
{
	math::float4x4 matrix = frustum.ProjectionMatrix();
	matrix.Transpose();
	return matrix;
}

math::float3 ComponentCamera::GetBackgroundColor() const
{
	return color;
}

math::float3 ComponentCamera::GetWorldRight() const
{
	return frustum.WorldRight();
}

void ComponentCamera::SetNearPlane(float value)
{
	if (value < far_plane && value > 0.0f)
		near_plane = value;
	frustum.SetViewPlaneDistances(near_plane, far_plane);

	properties_modified = true;
}

void ComponentCamera::SetFarPlane(float value)
{
	if (value > near_plane)
		far_plane = value;

	frustum.SetViewPlaneDistances(near_plane, far_plane);

	properties_modified = true;
}

void ComponentCamera::SetFOV(float value)
{
	//TODO: check for min -max values of fov
	fov = value;

	frustum.SetVerticalFovAndAspectRatio(DegToRad(fov), aspect_ratio);

	properties_modified = true;
}

void ComponentCamera::SetFront(const math::float3& value)
{
	frustum.SetFront(value);
}

void ComponentCamera::SetUp(const math::float3& value)
{
	frustum.SetUp(value);
}

void ComponentCamera::LookAt(const math::float3 & point)
{
	math::float3 look_direction = point - frustum.Pos();
	math::float3x3 matrix = math::float3x3::LookAt(frustum.Front(), look_direction, frustum.Up(), math::float3::unitY);

	frustum.SetFront(matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetFront(matrix.MulDir(frustum.Up()).Normalized());
}

void ComponentCamera::SetBackgroundColor(const math::float3 color)
{
	this->color = color;
}

bool ComponentCamera::IsVisible(const math::AABB & box)const
{
	bool ret = true;
	math::vec corners[8];
	box.GetCornerPoints(corners);

	math::Plane planes[6];
	frustum.GetPlanes(planes);

	for (int p = 0; p < 6; p++)
	{
		int count = 0;
		for (int i = 0; i < 8; i++)
		{
			if (planes[p].IsOnPositiveSide(corners[i]))
				count++;
		}

		if (count == 8)
		{
			ret = false;
			break;
		}
	}

	return ret;
}

math::float4x4 ComponentCamera::GetViewMatrix() const
{
	math::float4x4 matrix = frustum.ViewMatrix();
	return matrix.Transposed();
}

void ComponentCamera::Save(Data & file)const
{
	Data data;
	data.AppendInt("type", type);
	data.AppendUInt("UUID", uuid);
	data.AppendBool("active", active);
	
	data.AppendFloat("near_plane", near_plane);
	data.AppendFloat("far_plane", far_plane);
	data.AppendFloat("fov", fov);
	data.AppendFloat("aspect_ratio", aspect_ratio);
	data.AppendFloat3("color", color.ptr());

	file.AppendArrayValue(data);
}

void ComponentCamera::Load(Data & conf)
{
	uuid = conf.GetUInt("UUID");
	active = conf.GetBool("active");

	near_plane = conf.GetFloat("near_plane");
	far_plane = conf.GetFloat("far_plane");
	fov = conf.GetFloat("fov");
	aspect_ratio = conf.GetFloat("aspect_ratio");
	color = conf.GetFloat3("color");

	//Init frustrum
	float vertical_fov = DegToRad(fov);
	float horizontal_fov = 2.0f*atanf(tanf(vertical_fov / 2.0f) * aspect_ratio);

	frustum.SetPerspective(horizontal_fov, vertical_fov);
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum.SetPos(float3::zero);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	frustum.SetViewPlaneDistances(near_plane, far_plane);
	frustum.SetVerticalFovAndAspectRatio(DegToRad(fov), aspect_ratio);

	OnTransformModified();
}

math::Ray ComponentCamera::CastCameraRay(math::float2 screen_pos)
{
	//TODO: Change this for the actual screen widht and height
	float2 pos = screen_pos;

	pos.x = 2.0f * pos.x / SCREEN_WIDTH - 1.0f;
	pos.y = 1.0f - 2.0f * pos.y / SCREEN_HEIGHT;
	
	Ray ray = frustum.UnProjectFromNearPlane(pos.x, pos.y);
 
	return ray;
}
