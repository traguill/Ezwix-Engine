#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "Material.h"
#include <string>
#include <map>

class ResourceFileTexture;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(ComponentType type, GameObject* game_object);
	~ComponentMaterial();

	void OnInspector();
	void Update();

	void Save(Data& file)const;
	void Load(Data& conf);

private:
	void PrintMaterialProperties();

public:
	std::string material_path;
	Material material;
	uint shader_id;
	uint texture_id = 0;
private:
	//Note: All materials must have model, view and projection uniforms. 
	std::map<string, ResourceFileTexture*> texture_list;

	bool change_material_enabled = false;

};
#endif // !__COMPONENT_MATERIAL_H__
