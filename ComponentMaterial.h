#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "Material.h"
#include <string>
#include <map>

class ResourceFileTexture;
class ResourceFileMaterial;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(ComponentType type, GameObject* game_object);
	~ComponentMaterial();

	void OnInspector();
	void Update();

	void Save(Data& file)const;
	void Load(Data& conf);

	uint GetDiffuseId()const;
	///Returns 0 if doesn't have a normal map.
	uint GetNormalId()const;

private:
	void PrintMaterialProperties();

public:
	std::string material_path; //To Library. If is "" means that this component uses the default material.
	ResourceFileMaterial* rc_material = nullptr;

private:
	std::string material_name; //Assets path
	//Note: All materials must have model, view and projection uniforms. 
	std::map<string, ResourceFileTexture*> texture_list;

	bool change_material_enabled = false;

	uint diffuse_id = 0;
	uint normal_id = 0;

};
#endif // !__COMPONENT_MATERIAL_H__
