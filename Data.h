#ifndef __DATA_H__
#define __DATA_H__

#include "JSON\parson.h"
#include "MathGeoLib\include\MathGeoLib.h"

//Wrapper for Parson (JSON) library

class Data
{
public:
	Data();
	Data(const char* filename);
	Data(JSON_Object* root);
	~Data();

	//Append
	bool AppendArray(const char* name);
	bool AppendArrayValue(const Data& data);
	bool AppendString(const char* name, const char* string);
	bool AppendBool(const char* name, bool value);
	bool AppendInt(const char* name, int value);
	bool AppendMatrix(const char* name, const math::float4x4& matrix);
	bool AppendFloat(const char* name, float value);
	bool AppendFloat3(const char* name,const float* value);

	//Gets
	Data GetJObject(const char* name)const;
	const char* GetString(const char* name)const;
	int GetInt(const char* name)const;

	//Save
	size_t Serialize(char** buffer);

private:
	JSON_Value* root_value = nullptr;
	JSON_Object* root = nullptr;
	JSON_Array* array = nullptr;

};

#endif // !__DATA_H__
