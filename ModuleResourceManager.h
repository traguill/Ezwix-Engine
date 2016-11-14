#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#include "Module.h"

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(const char* name, bool start_enabled = true);
	~ModuleResourceManager();

private:

};
#endif // !__MODULE_RESOURCE_MANAGER_H__
