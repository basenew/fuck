#pragma once
#include <iostream>
#include <json/json.h>

#include "flash_lamp.h"
#include "ppplay/propaganda.h"
#include "nav_error_code.h"
//#include "navigation.h"
#include "task_node.h"
#include "task_flow.h"
#include "gs_api.h"
#include "task.h"

using namespace std;
using namespace Json;

namespace nav{
using string_vct= vector<string>;

enum MODE{
	SINGLE = 1,
    LOOP   = 2,
    RANDOM = 3
};

struct OpBrocast{
	int         swh;
	int         md;
	string_vct  lst;
};

struct OpLamp{
	int  swh;
	bool white;
	bool rb;
	bool alarm;
};

struct OpPTZ{
	int swh;
	int h;
	int v;
	int z;
};

class NavPointTask:public TaskCB{
public:
	inline NavPointTask(const GsPos& pt, const string& path_name, const string& pt_name)
	:_name("Nav_" + path_name + "_" + pt_name), _pt(pt), _path(path_name){};
	
	void on_running(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_running" << endl;
	};

	void on_finished(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_finished" << endl;
	};

	void on_paused(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_paused" << endl;
	};

	void on_loop(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_loop" << endl;
	};

	inline const string& name(){return _name;};
	inline void name(const string& name){_name = name;};
private:
	GsPos       _pt;
	string      _path;	
	string      _name;
};

class PTZTask:public TaskCB{
public:
	inline PTZTask(const OpPTZ& ptz)
	:_name("PTZTask"), _ptz(ptz){}

	void on_running(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_running" << endl;
	};

	void on_finished(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_finished" << endl;
	};

	void on_paused(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_paused" << endl;
	};


	void on_loop(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_loop" << endl;
	};

	inline const string& name(){return _name;};
	inline void name(const string& name){_name = name;};
private:
	string _name;
	OpPTZ  _ptz;
};

class BrocastTask:public TaskCB{
public:
	inline BrocastTask(const OpBrocast& bc)
	:_name("BrocastTask"), _bc(bc){};

	void on_running(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_running" << endl;
		Propaganda::get_instance()->play(_bc.lst, true);
	};

	void on_finished(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_finished" << endl;
	    if(Propaganda::get_instance()->isPlaying())
			Propaganda::get_instance()->stop();
	};

	void on_paused(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_paused" << endl;
	};

	void on_loop(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_loop" << endl;
	};

	inline const string& name(){return _name;};
	inline void name(const string& name){_name = name;};

private:
	string    _name;
	OpBrocast _bc;
};

class LampTask:public TaskCB{
public:
	inline LampTask(const OpLamp lamp)
	:_name("LampTask"), _lamp(lamp){};

	void on_running(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		if (_lamp.white)
			FlashLamp::get_instance()->set_white_lamp_status(FL_ON);
		if (_lamp.rb)
			FlashLamp::get_instance()->set_red_blue_flash_status(FL_ON);
	};

	void on_finished(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		if (_lamp.white)
			FlashLamp::get_instance()->set_white_lamp_status(FL_OFF);
		if (_lamp.rb)
			FlashLamp::get_instance()->set_red_blue_flash_status(FL_OFF);
	};

	void on_paused(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_paused" << endl;
	};

	void on_loop(){
		log_info("%s %s...", _name.c_str(), __FUNCTION__);
		cout << _name << " on_loop" << endl;
	};

	inline const string& name(){return _name;};
	inline void name(const string& name){_name = name;};

private:
	string _name;
	OpLamp _lamp;
}; 

struct Operation{
	int       swh;
	OpPTZ     ptz;
	OpLamp    lamp;
	OpBrocast bc;
};

struct ObsPoint{
	string      name;
	Operation   op;
	GsPos       pt;
};

struct ObsPath{
	string            name;
	Operation         op;
	list<ObsPoint>    points;
};

enum SCHEME_SWITCH
{
	SW_DISABLE,
	SW_ENABLE,
	SW_PAUSE,
	SW_RESUME,
};

class Scheme:public Task{
public:
	enum SCH_STATUS{
		SCH_IDLE,
		SCH_RUNNING,
		SCH_PAUSED
	};

	Scheme(const string& name="")
	:Task("scheme_"+name)
	,_start_time(0)
	,_exe_times(1)
	,_merge_paths(false)
	,_sch_st(SCH_IDLE){};
	virtual ~Scheme(){};

	int parse(Value& scheme);

	inline void sch_switch(int swh){_swh = swh;};
	inline int  sch_switch(){return _swh;};
	inline int  sch_status(){return _sch_st;};
	inline bool is_running(){return _sch_st == SCH_RUNNING;};
	inline const GsNamePoint& return_point(){return _ret_pt;};     

	virtual void on_running();
	virtual void on_finished();
	virtual void on_paused();
private:
	int _check_json(Value& scheme);

	void _clear();

private:
	int          _sch_st;
	int          _swh;
	int          _start_time;	
	int          _exe_times;
	bool         _merge_paths;

	Operation       _sch_op;
	GsNamePoint     _ret_pt;
	list<ObsPath>   _paths;
	list<TaskCB*>   _tasks;
};


}

