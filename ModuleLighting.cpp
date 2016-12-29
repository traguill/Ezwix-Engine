#include "Application.h"
#include "ModuleLighting.h"

ModuleLighting::ModuleLighting(const char* name, bool start_enabled) : Module(name, start_enabled)
{}

ModuleLighting::~ModuleLighting()
{
}

bool ModuleLighting::Init(Data & config)
{
	ambient_intensity = config.GetFloat("ambient_intensity");
	ambient_color = config.GetFloat3("ambient_color");
	return true;
}

void ModuleLighting::SaveBeforeClosing(Data & data) const
{
	data.AppendFloat("ambient_intensity", ambient_intensity);
	data.AppendFloat3("ambient_color", ambient_color.ptr());
}
