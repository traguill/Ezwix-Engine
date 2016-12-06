#include "Application.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "imgui\imgui.h"
#include "Data.h"
#include "ResourceFileTexture.h"
ComponentMaterial::ComponentMaterial(ComponentType type, GameObject* game_object) : Component(type, game_object)
{}

ComponentMaterial::~ComponentMaterial()
{
	if (rc_texture)
	{
		rc_texture->Unload();
		rc_texture = nullptr;
	}
}

void ComponentMaterial::OnInspector()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		if (rc_texture)
		{
			ImGui::Text("Texture id: %d", rc_texture->GetTexture());
			ImGui::Image((ImTextureID)rc_texture->GetTexture(), ImVec2(250, 250));

			if (ImGui::Button("Remove ###mat_rem"))
			{
				Remove();
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Texture %s missing", file_path.data());
		}
	}
}

void ComponentMaterial::Update()
{

	game_object->texture_to_draw = (rc_texture) ? rc_texture->GetTexture() : 0;
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
	rc_texture = (ResourceFileTexture*)App->resource_manager->LoadResource(file_path, ResourceFileType::RES_TEXTURE);
	if (rc_texture)
	{
		game_object->texture_to_draw = rc_texture->GetTexture();
	}
	else
		game_object->texture_to_draw = 0;
	
}
