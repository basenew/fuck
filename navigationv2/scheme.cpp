#include "scheme.h"

namespace nav{


int Scheme::_check_json(Value& scheme)
{
	if(scheme["startTime"].isNull() 
	|| scheme["repeatType"].isNull()
	|| scheme["period"].isNull() 
	|| scheme["switch"].isNull()
	|| scheme["cameraHAngle"].isNull() 
	|| scheme["cameraVAngle"].isNull()
	|| scheme["focusLength"].isNull() 
	|| scheme["floodLight"].isNull()
	|| scheme["flashingLight"].isNull() 
	|| scheme["alarmLight"].isNull()
	|| scheme["broadcastPlayMode"].isNull() 
	|| scheme["broadcastSwitch"].isNull()
	|| scheme["broadcastList"].isNull() 
	|| scheme["paths"].isNull()
	|| scheme["returnPoint"].isNull()
	|| scheme["returnPoint"]["angle"].isNull() 
	|| scheme["returnPoint"]["x"].isNull()
	|| scheme["returnPoint"]["y"].isNull())
	    return ERR_INVALID_JSON;
	return ERR_OK;
}

int Scheme::parse(Value& scheme)
{
	_clear();

	int ret = _check_json(scheme);
	if (ret != ERR_OK) return ret;

	_start_time = scheme["startTime"].asInt();
	//todo schudle patrol?
	
	//todo how to use switch on scheme scheme["switch"].asInt();
	//todo how to use work time if(scheme["workTime"].isNull())
	
	Operation op;
	op.ptz.h = scheme["cameraHAngle"].asInt();
	op.ptz.v = scheme["cameraVAngle"].asInt();
	op.ptz.z = scheme["focusLength"].asInt();
	
	
	op.lamp.rb    = scheme["flashingLight"].asBool();
	op.lamp.white = scheme["floodLight"].asBool();
	op.lamp.alarm = scheme["alarmLight"].asBool();
	
	op.bc.swh = scheme["broadcastSwitch"].asInt();
	op.bc.md  = scheme["broadcastPlayMode"].asInt();

	int sz = scheme["broadcastList"].size();
	Value js_lst = scheme["broadcastList"];
	for(int i = 0; i < sz; i ++){
	    op.bc.lst.push_back(js_lst[i].asString());
	}
	
	int path_sz = scheme["paths"].size();
	log_info("path size:%d", path_sz);
	Value js_paths = scheme["paths"];
	
	_ret_pt.pos.angle = scheme["returnPoint"]["angle"].asDouble();
	_ret_pt.pos.x= scheme["returnPoint"]["x"].asInt();
	_ret_pt.pos.y= scheme["returnPoint"]["y"].asInt();

	//std::string return_obs_name = scheme["returnPoint"].asString();
	for(int i = 0; i < path_sz; i ++){
		Value js_path = js_path;//todo maybe error?
	    if(js_path["path"].isNull() || js_path["pathType"].isNull()
	            || js_path["obsPoints"].isNull())
	        return ERR_INVALID_JSON;

		ObsPath path;
	    if(!js_path["path"].isNull()){
	        path.name = js_path["path"].asString();
		}
	
	    int pt_sz = js_path["obsPoints"].size();
	    Value js_pts = js_path["obsPoints"];
	    for(int j = 0; j < pt_sz; j ++){
			Value js_pt = js_pt;
	        if(js_pt["name"].isNull() || js_pt["angle"].isNull()
	                || js_pt["x"].isNull() || js_pt["y"].isNull())
				return ERR_INVALID_JSON;

			ObsPoint pt;
	        pt.name = js_pt["name"].asString();
	        pt.pt.angle = js_pt["angle"].asDouble();
	        pt.pt.x = js_pt["x"].asInt();
	        pt.pt.y = js_pt["y"].asInt();
			path.points.push_back(pt);
	    }
		_paths.push_back(path);
	}
	//todo push task??
	return ERR_OK;
}

void Scheme::on_running(){
	if (_paths.empty()){
		stop();
		return;
	}

	TaskFlow tf(_name);
	TaskNode *pre_t = nullptr, *cur_t = nullptr;
	for (auto path:_paths)
	{
		cout << _name << "path:" << path.name << endl;
		for (auto pt:path.points)
		{
			cout << _name << "point:" << pt.name << endl;
			NavPointTask* nav_t = new NavPointTask(pt.pt, path.name, pt.name);	
			_tasks.push_back(nav_t);
			cur_t = tf.push(nav_t->name(), nav_t);
			if (pre_t)
				cur_t->behind(pre_t);

			pre_t = cur_t;
		}
	}

	if (tf.empty()) return;

	TaskNode *first_tn = tf[0], *second_tn = tf[1], *bc_tn = nullptr, *lamp_tn = nullptr, *ptz_tn = nullptr;
	if (_sch_op.swh == SW_ENABLE){
		if (_sch_op.bc.swh == SW_ENABLE){
			BrocastTask* bc_t = new BrocastTask(_sch_op.bc);
			_tasks.push_back(bc_t);
			bc_tn = tf.push(bc_t->name(), bc_t);
			first_tn->front(bc_tn);
		}

		if (_sch_op.lamp.swh == SW_ENABLE){
			LampTask* lamp_t = new LampTask(_sch_op.lamp);
			_tasks.push_back(lamp_t);
			lamp_tn = tf.push(lamp_t->name(), lamp_t);
			first_tn->front(lamp_tn);
		}

		if (_sch_op.ptz.swh == SW_ENABLE){
			PTZTask* ptz_t = new PTZTask(_sch_op.ptz);
			_tasks.push_back(ptz_t);
			ptz_tn = tf.push(ptz_t->name(), ptz_t);
			first_tn->front(ptz_tn);
			if (second_tn)
				second_tn->behind(ptz_tn);
		}
	}

	_sch_st = SCH_RUNNING;
	tf.run();
}

void Scheme::on_finished()
{
	cout << _name << "on_finished" << endl;
	_sch_st = SCH_IDLE;
}

void Scheme::on_paused()
{
	cout << _name << "on_finished" << endl;
	_sch_st = SCH_PAUSED;
}

void Scheme::_clear(){
	_start_time = 0;
	_exe_times = 1;
	_merge_paths = false;
	_sch_op = {};
	_ret_pt = {};
	_paths.clear();

	for (auto t:_tasks)
	  if (t) delete t;

	_tasks.clear();
}

}
