#include "ComponentMaterial.h"
#include "imgui\imgui.h"

ComponentMaterial::ComponentMaterial(ComponentType type, GameObject* game_object) : Component(type, game_object)
{}

ComponentMaterial::~ComponentMaterial()
{}

void ComponentMaterial::OnInspector()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Image((ImTextureID)texture_id, ImVec2(250, 250));
	}


}
