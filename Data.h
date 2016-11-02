#ifndef __DATA_H__
#define __DATA_H__

#include "JSON\parson.h"

//Wrapper for Parson (JSON) library

class Data
{
public:
	Data();
	Data(const char* filename);
	~Data();

	const char* GetString(const char* name)const;

private:
	JSON_Value* root_value;
	JSON_Object* root;

};

#endif // !__DATA_H__
