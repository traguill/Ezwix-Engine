#include "Application.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "imgui\imgui.h"
#include "Data.h"
#include "ResourceFileTexture.h"
#include "Assets.h"
#include "ResourceFileMaterial.h"

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
		if (material_path.size() == 0)
		{
			ImGui::Text("Material: Default material");

			for (map<string, ResourceFileTexture*>::iterator it = texture_list.begin(); it != texture_list.end(); it++)
			{
				ImGui::Text("%s", (*it).first.data());
				ImGui::Image((ImTextureID)(*it).second->GetTexture(), ImVec2(50, 50));
			}
		}
		else
		{
			ImGui::Text("Material: %s", material_name.data());
			PrintMaterialProperties();
		}
		

		if (ImGui::Button("Change material"))
		{
			change_material_enabled = true;
		}

		if (change_material_enabled)
		{
			if (ImGui::BeginMenu("Select a material"))
			{
				vector<string> materials;
				App->editor->assets->GetAllFilesByType(FileType::MATERIAL, materials);

				for (vector<string>::iterator it = materials.begin(); it != materials.end(); ++it)
				{
					if (ImGui::MenuItem((*it).data()))
					{
						//TODO: UNLOAD ALL TEXTURES IN TEXTURE LIST

						change_material_enabled = false;
						material_name = (*it).data();
						if (rc_material)
						{
							rc_material->Unload();
						}
						material_path = App->resource_manager->FindFile(material_name);
						rc_material = (ResourceFileMaterial*)App->resource_manager->LoadResource(material_path, ResourceFileType::RES_MATERIAL);
					}
				}
				ImGui::EndMenu();
			}
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
		rc_material = (ResourceFileMaterial*)App->resource_manager->LoadResource(material_path, ResourceFileType::RES_MATERIAL);

		for (vector<Uniform*>::iterator uni = rc_material->material.uniforms.begin(); uni != rc_material->material.uniforms.end(); ++uni)
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

void ComponentMaterial::PrintMaterialProperties()
{
	for (vector<Uniform*>::iterator it = rc_material->material.uniforms.begin(); it != rc_material->material.uniforms.end(); ++it)
	{
		ImGui::Text("Variable: ");
		ImGui::SameLine();
		ImGui::Text("%s", (*it)->name.data());

		switch ((*it)->type)
		{
		case U_BOOL:
			ImGui::Checkbox("###bool_u", reinterpret_cast<bool*>((*it)->value));
			break;
		case U_INT:
			ImGui::InputInt("###int_u", reinterpret_cast<int*>((*it)->value));
			break;
		case U_FLOAT:
			ImGui::InputFloat("###float_m", reinterpret_cast<float*>((*it)->value));
			break;
		case U_VEC2:
			ImGui::InputFloat2("###float2_u", reinterpret_cast<float*>((*it)->value));
			break;
		case U_VEC3:
			ImGui::InputFloat2("###float2_u", reinterpret_cast<float*>((*it)->value));
			break;
		case U_MAT4X4:
			ImGui::Text("Matrices can be initalized only by code");
			break;
		case U_SAMPLER2D:
			/*if (ImGui::InputText("###sampler_u", u_sampler2d._Myptr(), u_sampler2d.capacity()))
			{
				if (content != nullptr)
					delete[] content;
				int sampler_size = u_sampler2d.size();
				content = new char[sizeof(int) + sampler_size * sizeof(char)];

				memcpy(content, &sampler_size, sizeof(int));
				char* pointer = content + sizeof(int);
				memcpy(pointer, u_sampler2d.c_str(), sizeof(char)* sampler_size);
			}*/
			ImGui::Text("Textures not visible right now");
			//TODO: Do it.
			break;
		}
		ImGui::Separator();
	}
}
