#ifndef __MODULELIGHTING_H__
#define __MODULELIGHTING_H__

#include "Module.h"
#include "MathGeoLib\include\MathGeoLib.h"

class ModuleLighting : public Module
{
public:
	ModuleLighting(const char* name, bool start_enabled = true);
	~ModuleLighting();

	bool Init(Data& config);
	void SaveBeforeClosing(Data& data)const;

public:
	float3 ambient_color = float3::one;
	float ambient_intensity = 0.1f;

};

#endif // !__MODULELIGHTING_H__
