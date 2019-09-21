#pragma once
#include <iostream>
#include <json/json.h>

#include "nav_error_code.h"
#include "navigation.h"
#include "task_flow.h"
#include "task.h"

using namespace std;
using namespace Json;

namespace nav{

class Scheme:public TaskFlow
{
public:
	Scheme(const string& name):TaskFlow(name){};
	virtual ~Scheme(){};

	ERR_CODE parse(Value& scheme_json);

};

}

