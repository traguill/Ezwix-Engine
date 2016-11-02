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

Data::Data(JSON_Object* root) : root(root)
{}

Data::~Data()
{
	json_value_free(root_value);
}

Data Data::GetJObject(const char* name)const
{
	return Data(json_object_get_object(root, name));
}

const char * Data::GetString(const char* name) const
{
	return json_object_get_string(root, name);
}

int Data::GetInt(const char* name)const
{
	return (int) json_object_get_number(root, name);
}
