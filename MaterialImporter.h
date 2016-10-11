#ifndef __MATERIAL_IMPORTER_H_
#define __MATERIAL_IMPORTER_H_

#include <string>

namespace MaterialImporter
{
	bool Import(const char* file, const char* path, std::string& output_file);
}


#endif // !__MATERIAL_IMPORTER_H_
