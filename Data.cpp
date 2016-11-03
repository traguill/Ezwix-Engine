#include "Data.h"

Data::Data()
{
	root_value = json_value_init_object();
	root = json_value_get_object(root_value);
}

Data::Data(const char * filename)
{
	root_value = json_value_init_string(filename);
	root = json_value_get_object(root_value);
}

Data::Data(JSON_Object* root) : root(root)
{}

Data::~Data()
{
	json_value_free(root_value);
}

bool Data::AppendArray(const char * name)
{
	JSON_Value* value = json_value_init_array();
	array = json_value_get_array(value);
	return json_object_set_value(root, name, value) == JSONSuccess;
}

bool Data::AppendArrayValue(const Data & data)
{
	if (!array)
		return false;
	return json_array_append_value(array, json_value_deep_copy(data.root_value)) == JSONSuccess;
}

bool Data::AppendString(const char * name, const char * string)
{
	return json_object_set_string(root, name, string) == JSONSuccess;
}

bool Data::AppendBool(const char * name, bool value)
{
	return json_object_set_boolean(root, name, value) == JSONSuccess;
}

bool Data::AppendInt(const char * name, int value)
{
	return json_object_set_number(root, name, value) == JSONSuccess;
}

bool Data::AppendMatrix(const char * name, const math::float4x4 & matrix)
{
	JSON_Value* value = json_value_init_array();
	JSON_Array* array = json_value_get_array(value);
	const float* matrix_array = *matrix.v;

	for (int i = 0; i < 16; i++)
		json_array_append_number(array, matrix_array[i]);

	return json_object_set_value(root, name, value) == JSONSuccess;
}

bool Data::AppendFloat(const char * name, float value)
{
	return json_object_set_number(root, name, value) == JSONSuccess;
}

bool Data::AppendFloat3(const char * name,const float * value)
{
	JSON_Value* j_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(j_value);

	for (int i = 0; i < 3; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(root, name, j_value) == JSONSuccess;
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

size_t Data::Serialize(char ** buffer)
{
	size_t size = json_serialization_size_pretty(root_value); //TODO: Change for normal serialization. Pretty is enabled now for testing.
	*buffer = new char[size];
	json_serialize_to_buffer_pretty(root_value, *buffer, size);
	return size;
}
