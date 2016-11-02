#include "Data.h"

Data::Data()
{
	root_value = json_value_init_object();
	root = json_value_get_object(root_value);
}

Data::Data(const char * filename)
{
	root_value = json_parse_string(filename);
	root = json_value_get_object(root_value);
}

Data::~Data()
{
	json_value_free(root_value);
}

const char * Data::GetString(const char* name) const
{
	return json_object_get_string(root, name);
}
