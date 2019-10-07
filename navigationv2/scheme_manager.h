#pragma once
#include <json/json.h>

#include "robot/robot.h"
#include "tiny_ros/ros/time.h"
#include "log/log.h"
#include "../navigation/nav_error_code.h"
#include "scheme.h"

using namespace std;
using namespace Json;
using namespace roslib_msgs;

namespace nav{

class SchemeManager{
public:
	bool init(){
	    rostopic::AgoraReceiveMessage_subscribe(boost::bind(&SchemeManager::_handle_msg, this, _1));
		return true;
	};

	int download(const SignalMessage &origin, const string &sch_name, const string &url);
	int upload(const SignalMessage &origin, const string &sch_name, Value& resp);
	int parse(const string& scheme_file);
	Scheme& get_using_scheme(){return _sch;};
    static SchemeManager& get_instance(){
        static SchemeManager singleton;
        return singleton;
    };

private:
	SchemeManager():_sch_parse_ret(ERR_PARSE_SCHEME_FAIL){
	};
	
	string _gen_upload_url(const string &path_name);
	bool _is_standby(){return _sch_parse_ret == ERR_OK;};
	int  _to_point(Value &req, Value &resp, string& result, GsPos pos);
	int  _to_return_point(Value &req, Value &resp, string& result);
	void _handle_msg(const roslib_msgs::SignalMessage &msg);
private:
	Scheme _sch;
	int    _sch_parse_ret;
};

} 
