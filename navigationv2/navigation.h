#pragma once
#include <iostream>
#include "nav_error_code.h"
#include "log/log.h"
#include "libwebsockets.h"
#include "gs_api.h"
#include "task_thread.h"

using namespace std;

namespace nav{

class Navigation
{
public:
	Navigation();

	ERR_CODE load_map(const string& name);
	ERR_CODE locate(const GsPos& pos);	
	ERR_CODE to_position(const GsPos& pos);	
	ERR_CODE to_position_flow_path(const GsPos& pos, const GsPath& path);	
	ERR_CODE flow_path(const GsPath& path);	

	virtual void execute();

private:
	bool _is_located();
	bool _is_loadedmap();

private:
	GsPos _pos;

};

class NavigationTask:public Navigation
{
};
}

