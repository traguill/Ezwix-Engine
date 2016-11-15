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
			string file_name = LIBRARY_FOLDER;
			file_name += file;
			file_name += "/";
			App->file_system->GenerateDirectory(file_name.data());
			file_name += file;
			file_name += ".dds";
			ret = App->file_system->Save(file_name.data(), data, size);
		}
		delete[] data;
		data = nullptr;
	}

	return ret;
}
