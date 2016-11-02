#ifndef __DATA_H__
#define __DATA_H__

#include "JSON\parson.h"

//Wrapper for Parson (JSON) library

class Data
{
public:
	Data();
	Data(const char* filename);
	Data(JSON_Object* root);
	~Data();

	Data GetJObject(const char* name)const;
	const char* GetString(const char* name)const;
	int GetInt(const char* name)const;

private:
	JSON_Value* root_value = nullptr;
	JSON_Object* root = nullptr;

};

#endif // !__DATA_H__
