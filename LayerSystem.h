#ifndef __LAYER_H__
#define __LAYER_H__

#include <vector>
#include <string>
#include "Data.h"

#define MAX_LAYERS 30 //TODO: Change this to an editable option in the enigne.

class LayerSystem
{
public:
	LayerSystem();
	~LayerSystem();

	bool AddLayer(unsigned int index, const std::string& name);
	std::vector<std::string>* GetEditableLayerList();

	void Load(Data& data);
	void Save(Data& data)const;

private:
	std::vector<std::string> layers;
};

#endif // !__LAYER_H__
