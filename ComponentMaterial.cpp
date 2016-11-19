#include "Application.h"
#include "ComponentMaterial.h"
#include "imgui\imgui.h"
#include "Data.h"
#include "ResourceFileTexture.h"
ComponentMaterial::ComponentMaterial(ComponentType type, GameObject* game_object) : Component(type, game_object)
{}

ComponentMaterial::~ComponentMaterial()
{
	App->resource_manager->UnloadResource(file_path);
}

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
	ResourceFileTexture* rc_texture = (ResourceFileTexture*)App->resource_manager->LoadResource(file_path, ResourceFileType::RES_TEXTURE);
	rc_texture->Load();
	texture_id = rc_texture->GetTexture();
}
