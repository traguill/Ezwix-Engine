#include "Application.h"
#include "MaterialImporter.h"
#include "ModuleFileSystem.h"

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#pragma comment ( lib, "Devil/libx86/DevIL.lib" )
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

bool MaterialImporter::Import(const char* file, const char * path, std::string & output_file)
{
	bool ret = false;

	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(path);
	

	ILuint size;
	ILubyte *data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, NULL, 0);

	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			ret = App->file_system->SaveUnique(file, data, size, LIBRARY_TEXTURES_FOLDER, "dds", output_file);
		}
		delete[] data;
		data = nullptr;
	}

	return ret;
}
