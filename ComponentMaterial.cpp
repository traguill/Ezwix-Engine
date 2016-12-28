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
	for (map<string, ResourceFileTexture*>::iterator it = texture_list.begin(); it != texture_list.end(); it++)
	{
		(*it).second->Unload();
	}
}

void ComponentMaterial::OnInspector()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		for (map<string, ResourceFileTexture*>::iterator it = texture_list.begin(); it != texture_list.end(); it++)
		{
			ImGui::Text("%s", (*it).first.data());
			ImGui::Image((ImTextureID)(*it).second->GetTexture(), ImVec2(50, 50));
		}

	}
}

void ComponentMaterial::Update()
{

	
}

void ComponentMaterial::Save(Data & file)const
{
	Data data;
	data.AppendInt("type", type);
	data.AppendUInt("UUID", uuid);
	data.AppendBool("active", active);
	data.AppendString("path", material_path.data());

	if (material_path.size() == 0)
	{
		data.AppendArray("textures");
		for (map<string, ResourceFileTexture*>::const_iterator it = texture_list.begin(); it != texture_list.end(); it++)
		{
			Data texture;
			texture.AppendString("path", (*it).first.data());
			data.AppendArrayValue(texture);
		}
	}

	file.AppendArrayValue(data);
}

void ComponentMaterial::Load(Data & conf)
{
	uuid = conf.GetUInt("UUID");
	active = conf.GetBool("active");
	material_path = conf.GetString("path");

	if (material_path.size() != 0)
	{
		material.Load(material_path.data());

		for (vector<Uniform*>::iterator uni = material.uniforms.begin(); uni != material.uniforms.end(); ++uni)
		{
			if ((*uni)->type == UniformType::U_SAMPLER2D)
			{
				string texture_path;
				int name_size = *reinterpret_cast<int*>((*uni)->value);
				texture_path.resize(name_size);
				memcpy(texture_path._Myptr(), (*uni)->value + sizeof(int), name_size);

				ResourceFileTexture* rc_tmp = (ResourceFileTexture*)App->resource_manager->LoadResource(texture_path, ResourceFileType::RES_TEXTURE);
				texture_list.insert(pair<string, ResourceFileTexture*>(texture_path, rc_tmp));
			}
		}
	}
	else
	{
		//Default shader
		shader_id = App->resource_manager->GetDefaultShaderId();

		Data texture;
		unsigned int tex_size = conf.GetArraySize("textures");
		for (int i = 0; i < tex_size; i++)
		{
			texture = conf.GetArray("textures", i);

			string tex_path = texture.GetString("path");
			ResourceFileTexture* rc_tmp = (ResourceFileTexture*)App->resource_manager->LoadResource(tex_path, ResourceFileType::RES_TEXTURE);
			texture_list.insert(pair<string, ResourceFileTexture*>(tex_path, rc_tmp));
			texture_id = rc_tmp->GetTexture();
		}

	}
	
}
