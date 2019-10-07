#pragma once
#include <iostream>
#include <json/json.h>

#include "gs_api.h"
#include "gs_nav.h"
#include "log/log.h"
#include "task.h"
#include "task_thread.h"
#include "wsclient.h"
#include "map_manager.h"
#include "../navigation/nav_error_code.h"

using namespace std;
using namespace Json;
using namespace roslib_msgs;

#define LOCATOR_TASK  "Locator"

namespace nav{

class Locator:public Task{
public:
	Locator():Task(LOCATOR_TASK){};

	inline int stop(){
		int ret;
		if ((ret = Task::stop()) == ERR_OK)
			return GsApi::get_instance()->nav_task_stop_all();
		return ret;	
	};
	void on_running();
	void on_loop();
	void response(int ret, string result = "");

public:
	string map;
	string point;
	int    x,y;
	int    times;
	float  angle;

	SignalMessage  org;
	GsLocateType   type; 
};

class Navigation{//todo :public Task{
	enum NAV_MODE{
		MD_NONE,
		MD_LOCATE,
		MD_NAVIGATE
	};
public:
	bool init(){
	    rostopic::AgoraReceiveMessage_subscribe(boost::bind(&Navigation::_handle_msg, this, _1));
		_thd.start();
		_connect_ws_status();
		_connect_ws_health();
		return MapManager::get_instance().init();
	};

	inline bool is_locating(){
		return (_cur_task && _md == MD_LOCATE);
	};

	inline bool is_navigating(){
		return (_cur_task && _md == MD_NAVIGATE);
	};

	inline bool is_idle(){
		return (_cur_task == nullptr || _cur_task->is_idle());
	};

	inline bool is_running(){
		return (is_locating() || is_navigating());
	};


	int is_located();
	int to_point(const GsNamePoint &pos);	
	int to_point_flow_path(const GsPos &pos, const GsPath &path);	

	int publish_pos_status(int task_type, int status_code);
	void publish_status(int task_type, const std::string &reason, int status);

	static Navigation& get_instance(){
		static Navigation nav;
		return nav;
	};

private:
	inline Navigation():_md(MD_NONE), _cur_task(nullptr), _task_name("atris_task_queue"){};
	void _handle_msg(const roslib_msgs::SignalMessage &msg);
	//int  _status_cb(char *data);
	void _status_cb(const string &data);
	void _health_cb(const string &data);
	void _add_monitor();
	void _on_status(int code, const Value &json);
	void _on_health(int code, const Value &json);
	void _connect_ws_status();
	void _connect_ws_health();

	int _get_position(Value &req, Value &resp, string &result);
	int _hdl_to_point(Value &req, Value &resp, string &result, const SignalMessage &msg);
	int _to_point(Value &req, Value &resp, string &result);
	int _hdl_locate(Value &req, Value &resp, string &result, const SignalMessage &msg);
	int _locate(Value &req, Value &resp, string &result, const SignalMessage &msg);
	int _locate_stop(Value &req, Value &resp, string &result, const SignalMessage &msg);


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

private:
	string _using_map;
    GsPath _path;
	string _map_name;
	string _task_name;
	string _map_id;

	//int    _st;	
	int    _code;
	int    _repeat_times;
	int    _md;

    int    lidar_master_state;
	int    lidar_slave_state;
    int    rgbd_state;
	int    gpsstatus;
	int    gyro_state;
	int    gyro_state_data;
	int    odom_delta_spd;

	WsClient        _ws_status;
	WsClient        _ws_health;
	TaskThread      _thd;
    NavStateMonitor _nav_mnt;
	//Locator         _locator;
	Task           *_cur_task;
};

}

