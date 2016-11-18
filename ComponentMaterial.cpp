#include "Application.h"
#include "ComponentMaterial.h"
#include "imgui\imgui.h"
#include "Data.h"

ComponentMaterial::ComponentMaterial(ComponentType type, GameObject* game_object) : Component(type, game_object)
{}

ComponentMaterial::~ComponentMaterial()
{}

void ComponentMaterial::OnInspector()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Text("Texture id: %d", texture_id);
		ImGui::Image((ImTextureID)texture_id, ImVec2(250, 250));

		if (ImGui::Button("Remove ###mat_rem"))
		{
			Remove();
		}
	}
}

void ComponentMaterial::Save(Data & file)const
{
	Data data;
	data.AppendInt("type", type);
	data.AppendUInt("UUID", uuid);
	data.AppendBool("active", active);
	data.AppendString("path", file_path.data());

	file.AppendArrayValue(data);
}

void ComponentMaterial::Load(Data & conf)
{
	uuid = conf.GetUInt("UUID");
	active = conf.GetBool("active");

	file_path = conf.GetString("path");
	//texture_id = App->meshes->LoadTexture(file_path.data()); //TODO: Change it for a resource method
}
