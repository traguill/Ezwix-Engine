#include "Application.h"
#include "Assets.h"
#include "MaterialCreatorWindow.h"
#include <vector>
#include "ShaderComplier.h"

MaterialCreatorWindow::MaterialCreatorWindow()
{
	save_path.resize(512);
}

MaterialCreatorWindow::~MaterialCreatorWindow()
{}

void MaterialCreatorWindow::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Material Creator", &active);

	ImGui::Text("Save path: "); ImGui::SameLine();
	ImGui::InputText("###save_path", save_path._Myptr(), save_path.capacity());
	
	ImGui::Text("Vertex program: "); ImGui::SameLine();
	string vertex_name = vertex + "###vertex_title";
	if(ImGui::BeginMenu(vertex_name.data()))
	{
		vector<string> vertex_list;
		App->editor->assets->GetAllFilesByType(FileType::VERTEX, vertex_list);

		for (vector<string>::iterator ver = vertex_list.begin(); ver != vertex_list.end(); ver++)
		{
			if (ImGui::MenuItem((*ver).data()))
			{
				vertex = (*ver);
				material.vertex_path = vertex;
			}
		}

		ImGui::EndMenu();
	}

	ImGui::Text("Fragment program: "); ImGui::SameLine();
	string frag_name = fragment + "###frag_title";
	if (ImGui::BeginMenu(frag_name.data()))
	{
		vector<string> fragment_list;
		App->editor->assets->GetAllFilesByType(FileType::FRAGMENT, fragment_list);

		for (vector<string>::iterator frag = fragment_list.begin(); frag != fragment_list.end(); frag++)
		{
			if (ImGui::MenuItem((*frag).data()))
			{
				fragment = (*frag);
				material.fragment_path = fragment;
			}
		}

		ImGui::EndMenu();
	}

	SetUniforms();

	if (ImGui::Button("Create Shader"))
	{
		if (ShaderCompiler::TryCompileShader(vertex.data(), fragment.data()))
		{
			compilation_result = "Shader creation success!";
			App->resource_manager->SaveMaterial(material, save_path.data());
		}
		else
			compilation_result = "Compilation failed";
	}

	ImGui::Text("Result: %s", compilation_result.data());

	ImGui::End();
}

void MaterialCreatorWindow::LoadToEdit(const char * path)
{
	material.Load(path);
	vertex = material.vertex_path.data();
	fragment = material.fragment_path.data();
	save_path = "Write again the save path";
}

void MaterialCreatorWindow::SetUniforms()
{
	ImGui::Separator();
	ImGui::Text("Variable name: "); ImGui::SameLine();
	ImGui::InputText("###variable_name", uniform_name._Myptr(), uniform_name.capacity());
	ImGui::Text("Type: "); ImGui::SameLine();
	if (ImGui::BeginMenu(uniform_type_char.data()))
	{
		if (ImGui::MenuItem("Bool"))
		{
			uniform_type_char = "Bool";
			uniform_type = UniformType::U_BOOL;
		}

		if (ImGui::MenuItem("Int"))
		{
			uniform_type_char = "Int";
			uniform_type = UniformType::U_INT;
		}

		if (ImGui::MenuItem("Float"))
		{
			uniform_type_char = "Float";
			uniform_type = UniformType::U_FLOAT;
		}

		if (ImGui::MenuItem("Vec2"))
		{
			uniform_type_char = "Vec2";
			uniform_type = UniformType::U_VEC2;
		}

		if (ImGui::MenuItem("Vec3"))
		{
			uniform_type_char = "Vec3";
			uniform_type = UniformType::U_VEC3;
		}

		if (ImGui::MenuItem("Mat4x4"))
		{
			uniform_type_char = "Mat4x4";
			uniform_type = UniformType::U_MAT4X4;
		}

		if (ImGui::MenuItem("Sampler2D"))
		{
			uniform_type_char = "Sampler2D";
			uniform_type = UniformType::U_SAMPLER2D;
		}
		ImGui::EndMenu();
	}

	SetUniformValue();

	if (ImGui::Button("Add uniform"))
	{
		material.AddUniform(uniform_name, uniform_type, content);
	}

	PrintUniforms();
	
}

void MaterialCreatorWindow::SetUniformValue()
{
	switch (uniform_type)
	{
	case U_BOOL:
		if (ImGui::Checkbox("###bool_u", &u_bool))
		{
			if (content != nullptr)
				delete[] content;

			content = new char[sizeof(bool)];
			memcpy(content, &u_bool, sizeof(bool));
		}
		break;
	case U_INT:
		if (ImGui::InputInt("###int_u", &u_int))
		{
			if (content != nullptr)
				delete[] content;
			content = new char[sizeof(int)];
			memcpy(content, &u_int, sizeof(int));
		}
		break;
	case U_FLOAT:
		ImGui::Text("error");
		break;
	case U_VEC2:
		ImGui::Text("error");
		break;
	case U_VEC3:
		ImGui::Text("error");
		break;
	case U_MAT4X4:
		ImGui::Text("error");
		break;
	case U_SAMPLER2D:

		break;
	}
}

void MaterialCreatorWindow::PrintUniforms()
{
	bool uni_bool;
	for (vector<Uniform*>::iterator it = material.uniforms.begin(); it != material.uniforms.end(); ++it)
	{
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "Variable: ");
		ImGui::SameLine();
		ImGui::Text("%s", (*it)->name.data());

		switch ((*it)->type)
		{
		case U_BOOL:
		{
			ImGui::Text("Type: bool");
			bool uni_bool = *(reinterpret_cast<bool*>((*it)->value));
			if (uni_bool)
				ImGui::Text("Value: True");
			else
				ImGui::Text("Value: False");

			break;
		}
		case U_INT:
		{
			ImGui::Text("Type: int");
			int uni_int = *(reinterpret_cast<int*>((*it)->value));
			ImGui::Text("Value: %i", uni_int);
			break;
		}
		case U_FLOAT:
			ImGui::Text("Type: float");
			break;
		case U_VEC2:
			ImGui::Text("Type: vec2");
			break;
		case U_VEC3:
			ImGui::Text("Type: vec3");
			break;
		case U_MAT4X4:
			ImGui::Text("Type: mat4x4");
			break;
		case U_SAMPLER2D:
			ImGui::Text("Type: sampler2D");
			break;
		}
		ImGui::Separator();
	}
}

