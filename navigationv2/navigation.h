#pragma once
#include <iostream>
#include <json/json.h>
#include "gs_api.h"
#include "gs_nav.h"
#include "log/log.h"
#include "task_thread.h"
#include "state_machine.h"
#include "nav_error_code.h"

using namespace std;
using namespace Json;

namespace nav{

class Navigation:public StateMachine{
public:
	Navigation():_map_id(-1){};

	int load_map(const string& name);
	int locate(const GsPos& pos);	
	int to_point(const GsPos& pos);	
	int to_point_flow_path(const GsPos& pos, const GsPath& path);	
	int flow_path(const GsPath& path);	
	
	inline int cancel(){
		return StateMachine::stop();
	};

private:
	int  _status_cb(char *data);
	void _add_monitor();
	void _on_status(int code, const Value& json);

	void _on_pause();
	void _on_stopped();
	void _on_locate_proc(const int code);
	void _on_patrol_finished();
	void _on_localization_error();
	void _on_goal_point_not_safe();
	void _on_path_avoiding_obstacle();
	void _on_path_invalid();
	void _on_path_running(Value json);
	void _on_path_block();
	void _on_path_unreachable();
	void _on_path_to_start_point();
	void _on_path_finish();
	void _on_nav_navigating();
	void _on_nav_block();
	void _on_nav_planning();
	void _on_nav_unreachable();
	void _on_nav_unreached();
	void _on_nav_reached();

	bool _is_located();
	bool _is_loaded_map();

private:
    GsPath _path;
	string _map_name;

	int    _code;
	int    _map_id;
	int    _repeat_times;

    NavStateMonitor _nav_mnt;
};

}

