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
			ImGui::Text("Number vertices %d", mesh->num_vertices);
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
