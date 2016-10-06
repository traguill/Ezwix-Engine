#include "ComponentMaterial.h"
#include "imgui\imgui.h"

ComponentMaterial::ComponentMaterial(ComponentType type) : Component(type)
{}

ComponentMaterial::~ComponentMaterial()
{}

void ComponentMaterial::OnInspector()
{
	if (ImGui::CollapsingHeader("Material"))
	{

	}


}
