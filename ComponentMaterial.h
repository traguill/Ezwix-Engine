#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include <string>

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

public:
	std::string file_path; 
	ResourceFileTexture* rc_texture = nullptr;

};
#endif // !__COMPONENT_MATERIAL_H__
