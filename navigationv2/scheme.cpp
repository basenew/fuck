#include "scheme.h"

namespace nav{


ERR_CODE Scheme::check_json(Value& scheme)
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
}

ERR_CODE Scheme::parse(Value& scheme)
{
	ERR_CODE ret = check_json(scheme);
	if (ret != ERR_OK) return ret;

	_start_time = scheme["startTime"].asInt();
	//todo schudle patrol?
	
	//todo how to use switch on scheme scheme["switch"].asInt();
	//todo how to use work time if(scheme["workTime"].isNull())
	
	int h = scheme["cameraHAngle"].asInt();
	int v = scheme["cameraVAngle"].asInt();
	int z = scheme["focusLength"].asInt();
	PTZTask ptz_t1(START, h, v, z);
	
	
	bool white = scheme["floodLight"].asBool();
	bool rb = scheme["flashingLight"].asBool();
	bool alarm = scheme["alarmLight"].asBool();
	LampTask lamp_t(START, white, rb, alarm);
	
	int bc_sw = scheme["broadcastSwitch"].asInt();
	int mode = scheme["broadcastPlayMode"].asInt();
	int sz = scheme["broadcastList"].size();
	Json::Value js_lst = scheme["broadcastList"];
	vct_string lst;
	for(int i = 0; i < sz; i ++){
	    lst.push_back(js_lst[i].asString());
	}
	BrocastTask bc_t(bc_sw, static_cast<MODE>(mode), lst);//todo bc_sw may be START
	
	int path_sz = scheme["paths"].size();
	log_info("path size:%d", path_sz);
	Json::Value js_paths = scheme["paths"];
	
	
	_ret_pt.angle = scheme["returnPoint"]["angle"].asDouble();
	_ret_pt.x= scheme["returnPoint"]["x"].asInt();
	_ret_pt.y= scheme["returnPoint"]["y"].asInt();
	
	//std::string return_obs_name = scheme["returnPoint"].asString();
	for(int i = 0; i < path_sz; i ++){
		Json::Value& js_path = js_path;//todo maybe error?
	    if(js_path["path"].isNull() || js_path["pathType"].isNull()
	            || js_path["obsPoints"].isNull())
	        return ERR_INVALID_JSON;

		string path_name;
	    if(!js_path["path"].isNull())
	        path_name = js_path["path"].asString();
	
	    int pt_sz = js_path["obsPoints"].size();
	    Json::Value js_pts = js_path["obsPoints"];
	    for(int j = 0; j < pt_sz; j ++){
			Json::Value& js_pt = js_pt;
	        if(js_pt["name"].isNull() || js_pt["angle"].isNull()
	                || js_pt["x"].isNull() || js_pt["y"].isNull())
				return ERR_INVALID_JSON;

			GsNamePoint pt;
	        pt.name = js_pt["name"].asString();
	        pt.pos.angle = js_pt["angle"].asDouble();
	        pt.pos.x = js_pt["x"].asInt();
	        pt.pos.y = js_pt["y"].asInt();
			NavTask nav_t(pt, path_name);
	    }
	}
	//todo push task??
	return ERR_OK;
}

}
