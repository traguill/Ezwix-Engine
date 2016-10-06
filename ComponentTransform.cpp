#include "ComponentTransform.h"
#include "GameObject.h"
#include "imgui\imgui.h"


ComponentTransform::ComponentTransform(ComponentType type) : Component(type)
{
	CalculateFinalTransform();
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::OnInspector()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImVec4 white = ImVec4(1, 1, 1, 1);
	
		//Position
		ImGui::TextColored(white, "Position: ");
		ImGui::SameLine();

		float3 position = this->position;
		if (ImGui::InputFloat3("##pos", position.ptr()))
		{
			SetPosition(position);
		}

		//Rotation
		ImGui::TextColored(white, "Rotation: ");
		ImGui::SameLine();

		float3 rotation = this->rotation_euler;
		if (ImGui::InputFloat3("##rot", rotation_euler.ptr()))
		{
			SetRotation(rotation_euler);
		}

		//Scale
		ImGui::TextColored(white, "Scale: ");
		ImGui::SameLine();

		float3 scale = this->scale;
		if (ImGui::InputFloat3("##scale", scale.ptr()))
		{
			SetScale(scale);
		}

		ImGui::Text("%0.2f %0.2f %0.2f %0.2f", transform_matrix.v[0][0], transform_matrix.v[0][1], transform_matrix.v[0][2], transform_matrix.v[0][3]);
		ImGui::Text("%0.2f %0.2f %0.2f %0.2f", transform_matrix.v[1][0], transform_matrix.v[1][1], transform_matrix.v[1][2], transform_matrix.v[1][3]);
		ImGui::Text("%0.2f %0.2f %0.2f %0.2f", transform_matrix.v[2][0], transform_matrix.v[2][1], transform_matrix.v[2][2], transform_matrix.v[2][3]);
		ImGui::Text("%0.2f %0.2f %0.2f %0.2f", transform_matrix.v[3][0], transform_matrix.v[3][1], transform_matrix.v[3][2], transform_matrix.v[3][3]);
	}
}

void ComponentTransform::SetPosition(math::float3 pos)
{
	position = pos;

	transform_matrix.v[3][0] = position.x;
	transform_matrix.v[3][1] = position.y;
	transform_matrix.v[3][2] = position.z;

	CalculateFinalTransform();
}

void ComponentTransform::SetRotation(math::float3 rot_euler)
{
	rotation_euler = rot_euler;
	
	float3 rot_deg = DegToRad(rot_euler);

	rotation = rotation.FromEulerXYZ(rot_deg.z, rot_deg.y, rot_deg.x);

	transform_matrix.SetRotatePart(rotation);

	CalculateFinalTransform();
}

void ComponentTransform::SetRotation(math::Quat rot)
{
	rotation = rot;

	transform_matrix.SetRotatePart(rotation);

	rotation_euler = RadToDeg(rotation.ToEulerXYZ());

	CalculateFinalTransform();
}

void ComponentTransform::SetScale(math::float3 scale)
{
	this->scale = scale;

	transform_matrix.v[0][0] = scale.x;
	transform_matrix.v[1][1] = scale.y;
	transform_matrix.v[2][2] = scale.z;

	CalculateFinalTransform();
}

math::float3 ComponentTransform::GetPosition()
{
	return position;
}

math::float3 ComponentTransform::GetRotationEuler()
{
	return rotation_euler;
}

math::Quat ComponentTransform::GetRotation()
{
	return rotation;
}

math::float3 ComponentTransform::GetScale()
{
	return scale;
}

math::float4x4 ComponentTransform::GetTransformMatrix()
{
	return final_transform_matrix;
}

void ComponentTransform::CalculateFinalTransform()
{
	if (parent)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)parent->GetComponent(C_TRANSFORM);
		assert(parent_transform);
		
		final_transform_matrix = parent_transform->final_transform_matrix * transform_matrix;
	}
	else
	{
		final_transform_matrix = transform_matrix;
	}
}