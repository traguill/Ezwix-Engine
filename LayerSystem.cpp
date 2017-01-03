#include "LayerSystem.h"
#include "Globals.h"

LayerSystem::LayerSystem()
{
	layers.resize(MAX_LAYERS);
	layers[0] = "Default";
}

LayerSystem::~LayerSystem()
{
}

bool LayerSystem::AddLayer(unsigned int index, const std::string& name)
{
	if (index >= MAX_LAYERS || index == 0)
		return false;

	layers[index] = name;
	return true;
}

std::vector<std::string>* LayerSystem::GetEditableLayerList() 
{
	return &layers;
}

void LayerSystem::Load(Data & data)
{
	
	if (data.GetArray("layers", 0).IsNull())
	{
		LOG("Layers configuration could not be loaded.");
		return;
	}

	for (int i = 0; i < MAX_LAYERS; ++i)
	{
		if (i == 0)
			continue;
		Data layer_data;
		layer_data = data.GetArray("layers", i);
		layers[i] = layer_data.GetString("name");
	}
}

void LayerSystem::Save(Data & data) const
{
	for (int i = 0; i < MAX_LAYERS; ++i)
	{
		Data layer;
		layer.AppendString("name", layers[i].data());
		data.AppendArrayValue(layer);
	}
}
