#include "ComponentTransform.h"
#include "GameObject.h"
#include "Globals.h"
#include "imgui\imgui.h"


ComponentTransform::ComponentTransform(ComponentType type, GameObject* game_object) : Component(type, game_object)
{
	CalculateFinalTransform();
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update(float dt)
{}

void ComponentTransform::OnInspector()
{
	if (ImGui::CollapsingHeader("Transform"))
	{

		ImVec4 white = ImVec4(1, 1, 1, 1);
	
		//Position
		ImGui::TextColored(white, "Position: ");
		ImGui::SameLine();

		float3 position = this->position;
		if (ImGui::DragFloat3("##pos", position.ptr()))
		{
			SetPosition(position);
		}

		//Rotation
		ImGui::TextColored(white, "Rotation: ");
		ImGui::SameLine();

		float3 rotation = this->rotation_euler;
		if (ImGui::DragFloat3("##rot", rotation_euler.ptr(), 1.0f, -360.0f, 360.0f))
		{
			SetRotation(rotation_euler);
		}

		//Scale
		ImGui::TextColored(white, "Scale: ");
		ImGui::SameLine();

		float3 scale = this->scale;
		if (ImGui::DragFloat3("##scale", scale.ptr()))
		{
			SetScale(scale);
		}

		//Local Matrix
		ImGui::Text("%0.2f %0.2f %0.2f %0.2f", transform_matrix.v[0][0], transform_matrix.v[0][1], transform_matrix.v[0][2], transform_matrix.v[0][3]);
		ImGui::Text("%0.2f %0.2f %0.2f %0.2f", transform_matrix.v[1][0], transform_matrix.v[1][1], transform_matrix.v[1][2], transform_matrix.v[1][3]);
		ImGui::Text("%0.2f %0.2f %0.2f %0.2f", transform_matrix.v[2][0], transform_matrix.v[2][1], transform_matrix.v[2][2], transform_matrix.v[2][3]);
		ImGui::Text("%0.2f %0.2f %0.2f %0.2f", transform_matrix.v[3][0], transform_matrix.v[3][1], transform_matrix.v[3][2], transform_matrix.v[3][3]);
	}
}

void ComponentTransform::SetPosition(math::float3 pos)
{
	position = pos;

	transform_matrix = transform_matrix.FromTRS(position, rotation, scale);

	CalculateFinalTransform();
}

void ComponentTransform::SetRotation(math::float3 rot_euler)
{
	rotation_euler = rot_euler;
	
	float3 rot_deg = DegToRad(rot_euler);

	rotation = rotation.FromEulerXYZ(rot_deg.z, rot_deg.y, rot_deg.x);

	transform_matrix = transform_matrix.FromTRS(position, rotation, scale);

	CalculateFinalTransform();
}

void ComponentTransform::SetRotation(math::Quat rot)
{
	rotation = rot;

	transform_matrix = transform_matrix.FromTRS(position, rotation, scale);

	rotation_euler = RadToDeg(rotation.ToEulerXYZ());

	CalculateFinalTransform();
}

void ComponentTransform::SetScale(math::float3 scale)
{
	this->scale = scale;

	transform_matrix = transform_matrix.FromTRS(position, rotation, this->scale);

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
	return final_transform_matrix.Transposed();
}

void ComponentTransform::CalculateFinalTransform()
{
	GameObject* game_object = GetGameObject();
	if (game_object)
	{
		if (game_object->GetParent())
		{
			ComponentTransform* parent_transform = (ComponentTransform*)game_object->GetParent()->GetComponent(C_TRANSFORM);
			assert(parent_transform);

			final_transform_matrix = parent_transform->final_transform_matrix * transform_matrix;

			std::vector<GameObject*>::const_iterator go_childs = game_object->GetChilds()->begin();
			for (go_childs; go_childs != game_object->GetChilds()->end(); ++go_childs)
			{
				ComponentTransform* transform = (ComponentTransform*)(*go_childs)->GetComponent(C_TRANSFORM);
				if (transform)
				{
					transform->CalculateFinalTransform();
				}
			}
		}
		else
		{
			final_transform_matrix = transform_matrix;
		}	
	}
	else
	{	
		LOG("Error: Component created but not attached to any gameobject");
	}
}