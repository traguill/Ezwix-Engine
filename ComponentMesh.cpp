#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleMeshes.h"
#include "imgui\imgui.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

ComponentMesh::ComponentMesh(ComponentType type, GameObject* game_object) : Component(type, game_object)
{
}

ComponentMesh::~ComponentMesh()
{
	delete mesh;
	mesh = nullptr;
}

void ComponentMesh::Update(float dt)
{
	//Component must be active to update
	if (!IsActive())
		return;

	ComponentTransform* trans = (ComponentTransform*)GetGameObject()->GetComponent(C_TRANSFORM);
	assert(trans);
	
	ComponentMaterial* material = (ComponentMaterial*)GetGameObject()->GetComponent(C_MATERIAL);
	uint texture_id = 0;

	if (material)
		texture_id = material->texture_id;


	App->renderer3D->Draw(*mesh, trans->GetTransformMatrix(), texture_id);	
}

void ComponentMesh::OnInspector()
{
	if (ImGui::CollapsingHeader("Geometry Mesh"))
	{
		//Active
		bool is_active = IsActive();
		if (ImGui::Checkbox("###activeMesh", &is_active))
		{
			SetActive(is_active);
		}

		if (mesh)
		{
			ImGui::Text("Number of vertices %d", mesh->num_vertices);
			ImGui::Text("Number of indices %d", mesh->num_indices);

			if (mesh->uvs != nullptr)
				ImGui::Text("Has UVs: yes");
			else
				ImGui::Text("Has UVs: no");

			if (mesh->normals != nullptr)
				ImGui::Text("Has Normals: yes");
			else
				ImGui::Text("Has Normals: no");

			if (mesh->colors != nullptr)
				ImGui::Text("Has Colors: yes");
			else
				ImGui::Text("Has Colors: no");
		}
		else
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "WARNING");
			ImGui::SameLine(); ImGui::Text("No mesh was loaded.");
		}
	}
}

bool ComponentMesh::SetMesh(Mesh * mesh)
{
	bool ret = false;
	if (mesh)
	{
		this->mesh = mesh;
		ret = true;
	}
		
	return ret;
}
