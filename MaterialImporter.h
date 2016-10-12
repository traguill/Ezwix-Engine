#ifndef __MATERIAL_IMPORTER_H_
#define __MATERIAL_IMPORTER_H_

#include <string>

namespace MaterialImporter
{
	//File is the name of the final file. Path includes the file with it's extension.
	bool Import(const char* file, const char* path, std::string& output_file);

}




#endif // !__MATERIAL_IMPORTER_H_
