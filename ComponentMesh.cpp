#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleMeshes.h"
#include "imgui\imgui.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

ComponentMesh::ComponentMesh(ComponentType type) : Component(type)
{
}

ComponentMesh::~ComponentMesh()
{
	delete mesh;
	mesh = nullptr;
}

void ComponentMesh::Update(float dt)
{
	ComponentTransform* trans = (ComponentTransform*)game_object->GetComponent(C_TRANSFORM);
	assert(trans);
	
	ComponentMaterial* material = (ComponentMaterial*)game_object->GetComponent(C_MATERIAL);
	uint texture_id = 0;

	if (material)
		texture_id = material->texture_id;


	App->renderer3D->Draw(*mesh, trans->GetTransformMatrix(), texture_id);	
}

void ComponentMesh::OnInspector()
{
	if (ImGui::CollapsingHeader("Geometry Mesh"))
	{
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
