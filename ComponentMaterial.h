#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include <string>

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(ComponentType type, GameObject* game_object);
	~ComponentMaterial();

	void OnInspector();

	void Save(Data& file)const;
	void Load(Data& conf);

public:
	size_t texture_id = 0;
	std::string file_path; 
};
#endif // !__COMPONENT_MATERIAL_H__
