#include "Application.h"
#include "MaterialImporter.h"
#include "ModuleFileSystem.h"

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#pragma comment ( lib, "Devil/libx86/DevIL.lib" )
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

bool MaterialImporter::Import(const char* file, const char * path)
{
	bool ret = false;

	char* buffer = nullptr;
	unsigned int size = App->file_system->Load(path, &buffer);

	if (size > 0)
	{
		ILuint id;
		ilGenImages(1, &id);
		ilBindImage(id);

		if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
		{
			ILuint il_size;
			ILubyte *data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
			il_size = ilSaveL(IL_DDS, NULL, 0);
			if (il_size > 0)
			{
				data = new ILubyte[il_size];
				if (ilSaveL(IL_DDS, data, il_size) > 0)
				{
					ret = App->file_system->Save(file, data, il_size);
				}
				delete[] data;
				ilDeleteImages(1, &id);
			}
		}
	}

	delete[] buffer;

	return ret;
}
