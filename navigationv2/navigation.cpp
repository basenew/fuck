#include "robot/robot.h"
#include "map_manager.h"
#include "navigation.h"
#include "gs_api.h"

#define STATUS_REPEAT_TIMES 20 

#define PARSE_JSON()    if(!reader.parse(msg.msg, req)){\
                            log_error("parse json fail.");\
                            ret = ERR_INVALID_JSON;\
                            break;\
                        }
namespace nav{

void Locator::on_running(){
    int ret = GsApi::get_instance()->nav_relocate(x, y, angle, type, map, point);

    if (org.account.empty()) {
        //todo msg.account = Robot::pc;
    }

    if(ret != ERR_OK){
		response(ret);
		stop();
    }
}

void Locator::response(int ret, string result){
	if (result.empty()) result = get_err_msg(ret);

	Value resp;	
    resp["id"] = org.msgID;
    resp["state"] = Json::Value(0);
    resp["result"] = result;
    resp["switch"] = Json::Value(1);
    resp["timestamp"] = org.timestamp;
    resp["result_code"] = ret;
    Robot::responseResult(org, resp, "response_locate");
}

void Locator::on_loop(){
	if (times > 0){
		--times;
		int ret;
		if (GsApi::get_instance()->nav_check_relocate_state(ret) != ERR_OK)
    	    return;

    	if(ret == 1){
    	    log_info("locate finished........");
			int ret = Navigation::get_instance().is_located();
			response(ret);
			stop();
    	}
    	else if(ret == 0){
    	    log_info("on locating....");
    	}
    	else{
    	    log_error("locate fail");
			response(ERR_LOCATE_FAIL, "locate_fail");
			stop();
    	}
	}
	else {
		response(ERR_LOCATE_FAIL, "locate_fail");
		stop();
	}
}

#define WS_NAV_STATUS_PROTOCOL_NAME "gs-state-protocol"
#define WS_DEV_HEALTH_PROTOCOL_NAME "gs-health-state-protocol"

#define WS_NAV_STATUS_PATH "/gs-robot/notice/status"
#define WS_DEV_HEALTH_PATH "/gs-robot/notice/system_health_status"

void Navigation::_connect_ws_status()
{
    wsClientParam param;
    param.path = WS_NAV_STATUS_PATH;
    param.protocolsName = WS_NAV_STATUS_PROTOCOL_NAME;
    param.addr = Config::get_instance()->gs_ip;
    param.port = Config::get_instance()->gs_ws_port;

    _ws_status.setParams(param);
    _ws_status.setSsl(false);
    _ws_status.setReadCallback(boost::bind(&Navigation::_status_cb, this, _1));
    _ws_status.start();
}

void Navigation::_connect_ws_health()
{
    wsClientParam param;
    param.path = WS_DEV_HEALTH_PATH;
    param.protocolsName = WS_DEV_HEALTH_PROTOCOL_NAME;
    param.addr = Config::get_instance()->gs_ip;
    param.port = Config::get_instance()->gs_ws_port;

    _ws_health.setParams(param);
    _ws_health.setSsl(false);
    _ws_health.setReadCallback(boost::bind(&Navigation::_health_cb, this, _1));
    _ws_health.start();
}
void Navigation::_handle_msg(const roslib_msgs::SignalMessage &msg){
    Reader reader;
    Value  req, resp;
    string rsp_title;
    string result;
    int    ret = ERR_OK;

    do
    {
        if (msg.title == "request_robot_pose"){
            PARSE_JSON();
			ret = _get_position(req, resp, result);
        }
		else if(msg.title == "request_locate"){
            PARSE_JSON(); 
			ret = _hdl_locate(req, resp, result, msg);
        }
		else if(msg.title == "request_nav_to") {
            PARSE_JSON(); 
			ret = _hdl_to_point(req, resp, result, msg);
        }
		else if (msg.title == "request_nav_switch_with_shttpd") { 
            PARSE_JSON(); 
            if(!req["content"]["switch"].isNull() && _cur_task) {
                int state = req["content"]["switch"].asInt();
                if (state == 0) { // pause
                    _cur_task->pause();
                } else if (state == 1) {// resume
                    _cur_task->resume();
                } else if (state == 2) { // stop
                    _cur_task->stop();
                }
            } else {
                result = "fail_invalid_data";
                ret = ERR_INVALID_FIELD;
            }
		}
		else return;
    }while(0);

	if (result.empty())
		result = ret == ERR_OK ? "success":get_err_msg(ret);

    resp["id"] = req["content"]["id"];
    resp["timestamp"] = req["content"]["timestamp"];
    resp["result"] = result;
    resp["result_code"] = ret;
    Robot::responseResult(msg, resp, "response" + msg.title.substr(7));
}

int Navigation::_get_position(Value &req, Value &resp, string& result){
    GsPos pos;
    int ret = GsApi::get_instance()->nav_get_pos(pos);
    if (ret == ERR_OK){
        resp["x"] = Value(pos.x);
        resp["y"] = Value(pos.y);
        resp["theta"] = Value(pos.angle);
    }
	return ret;
}

int Navigation::_hdl_to_point(Value &req, Value &resp, string& result, const SignalMessage &msg){
	int ret = ERR_OK;

	if (is_locating()){
		ret = ERR_LOCATING;
		return ret;
	}

    int sw = req["content"]["switch"].asInt();
	switch(sw){
	case 0:
		if (is_navigating()){
			ret = _cur_task->stop();	
		}
		else{
			ret = ERR_NOT_RUNNING;
		}
		break;
	case 1:
		if (is_idle()){
			GsNamePoint pt;
    		pt.name = req["content"]["name"].asString();
    		pt.pos.angle = req["content"]["angle"].asFloat();
    		pt.pos.x = req["content"]["x"].asInt();
    		pt.pos.y = req["content"]["y"].asInt();
			ret = to_point(pt);
		}
		else {
			ret = ERR_ALREADY_RUNNING;
		}
		break;
	case 2:
		if (is_navigating()){
			ret = _cur_task->pause();	
		}
		else{
			ret = ERR_NOT_RUNNING;
		}
		break;
	default:
		ret = ERR_INVALID_VALUE;
		break;
	}

	return ret;
}

int Navigation::_hdl_locate(Value &req, Value &resp, string& result, const SignalMessage &msg){
	int ret = ERR_OK;
	if (is_navigating()){
		ret = ERR_NAVIGATING;
		return ret;
	}

    int sw = req["content"]["switch"].asInt();
    if(sw == 1){
		if (is_idle()){
			string using_map = MapManager::get_instance().get_using_map();
			if (using_map.empty()){
				ret = ERR_MAP_NOT_USING;
			}
			else{
				ret = _locate(req, resp, result, msg);
			}
		}
		else{
			ret = ERR_ALREADY_RUNNING;
		}
    }
    else if(sw == 0){
		if (is_running()){
			ret = _cur_task->stop();
		}
		else{
			ret = ERR_NOT_RUNNING;
		}
    }

    resp["switch"] = Json::Value(sw);
	return ret;
}

int Navigation::_locate(Value &req, Value &resp, string& result, const SignalMessage &msg){
	//todo thread safe
	//todo judge nav not running
	if (_cur_task)delete _cur_task;
	_cur_task = new Locator();

	Locator& locator = *(Locator*)_cur_task;
    locator.point = req["content"]["name"].asString();
    locator.angle = req["content"]["angle"].asFloat();
    locator.x = req["content"]["x"].asInt();
    locator.y = req["content"]["y"].asInt();

#ifdef _CHASSIS_MARSHELL_
    log_info("locate not turn around");
	locator.type = GS_LOCATE_DIRECT_CUSTMOIZED;
#else
    log_info("locate turn around");
    locator.type = GS_LOCATE_CUSTMOIZED;
#endif
	_thd.push(_cur_task);
	return locator.start();
}

int Navigation::is_located(){
	string using_map = MapManager::get_instance().get_using_map(); 
	if (using_map.empty()){
		return ERR_MAP_NOT_USING;
	}

	int ret = GsApi::get_instance()->get_init_status(using_map);
    if (ERR_OK != ret){
        log_error("robot is not located");
    }

	return ret;
}

int Navigation::to_point(const GsNamePoint& pt){
    log_info("%s", __FUNCTION__);
    int ret = is_located();
	if (ret != ERR_OK)
		return ret;
	
	//todo _add_monitor();
	
    vector<Value> js_tasks;
    Value task = GsApi::get_instance()->nav_new_nav_path_task(_map_name, _path, pt.pos);
    js_tasks.push_back(task);
    ret = GsApi::get_instance()->nav_task_new(_task_name, _map_name, _map_id, js_tasks);
    if(ret != ERR_OK){
        //todo zy
        log_error("new task fail.");
        return ret;
    }

    ret = GsApi::get_instance()->nav_merge_path_task_start(_task_name, task, _map_name, _map_id);
    if(ret != ERR_OK){
        //todo zy
        log_error("task start fail.");
        return ret;
    }

	return ERR_OK;
}
	

void Navigation::_status_cb(const string& data)
{
    Json::Reader reader;
    Json::Value root;

	//todo mebay bug
    //if(get_patrol_state() == PATROL_STATE_IDLE)
    //    return 0;

    //log_info("%s:%s", __FUNCTION__, data);
    if(!reader.parse(data.c_str(), root)){
        log_error("parse gs status js data fail.");
        return;// ERR_INVALID_JSON;
    }

    int code = root["statusCode"].asInt();
    //过滤掉相同的状态上报
    if(code == _code){
        _repeat_times ++;
        if(_repeat_times == STATUS_REPEAT_TIMES){
            _repeat_times = 0;
        }
        else
            return;// ERR_OK;
    }
    else{
        _repeat_times = 0;
    }

    _code = code;
    //不保留检测到障碍物的状态码
    //if(code != 701)
    //_code = code;

    log_info("nav code:%d", code);
    {
#if 0
		//todo add by
        if(code != 200 && code != 407 && (code < 800 || code > 900)){
            if(code == 306){
                if(nav_type != NAV_PATROL || nav_type != NAV_PATROL_AUTO)
                    publish_pos_status(_code);
            }
            else
                publish_pos_status(_code);
        }
#endif
    }

    _on_status(code, root);

    //return ERR_OK;
}

void Navigation::_on_pause()
{
    log_info("%s", __FUNCTION__);
}

void Navigation::_on_stopped()
{
    log_info("%s", __FUNCTION__);

}

void Navigation::_on_locate_proc(const int code)
{
    if(code == 1006){
    }
}

void Navigation::_on_patrol_finished()
{
    log_info("%s", __FUNCTION__);

    //Charger::get_instance()->start_charge();
}

void Navigation::_on_localization_error()
{
    log_info("%s", __FUNCTION__);

}

void Navigation::_on_goal_point_not_safe()
{
    log_info("%s", __FUNCTION__);

    //patrol_state = PATROL_STATE_IDLE;
}

void Navigation::_on_path_avoiding_obstacle()
{
    log_info("%s", __FUNCTION__);

}

void Navigation::_on_path_invalid()
{
    log_info("%s", __FUNCTION__);
}

void Navigation::_on_path_running(Json::Value json)
{
    log_info("%s", __FUNCTION__);
}

void Navigation::_on_path_block()
{
    log_info("%s", __FUNCTION__);
}

void Navigation::_on_path_unreachable()
{
    log_info("%s", __FUNCTION__);
}

void Navigation::_on_path_to_start_point()
{
    log_info("%s", __FUNCTION__);
}

void Navigation::_on_path_finish()
{
    log_info("%s", __FUNCTION__);
}

void Navigation::_on_nav_navigating()
{
    log_info("on_nav_navigating");
}

void Navigation::_on_nav_block()
{
    log_info("on_nav_block");
}

void Navigation::_on_nav_planning()
{
    log_info("on_nav_planning");
}

void Navigation::_on_nav_unreachable()
{
    log_info("%s", __FUNCTION__);
}

void Navigation::_on_nav_unreached()
{
    log_info("%s", __FUNCTION__);
}

void Navigation::_on_nav_reached(){
    log_info("on_nav_reached");
}

void Navigation::_health_cb(const string& data){
    Json::Reader reader;
    Json::Value root, rbtInfoValue;
    Json::FastWriter fw;
    std::string strRbtInfo = "";
    roslib_msgs::RobotInfo rbtInfo;

    if(!reader.parse(data.c_str(), root)){
        log_error("parse device health state fail.");
        return ;
    }

    if(!root["laserTopic"].isNull()){
        lidar_master_state = root["laserTopic"].asBool() ? 0 : 1;
        rbtInfoValue["robot_info"]["main_lidar"]["error"] = lidar_master_state;
    }

    if(!root["laser2Topic"].isNull()){
        lidar_slave_state = root["laser2Topic"].asBool() ? 0 : 1;
        rbtInfoValue["robot_info"]["slave_lidar"]["error"] = lidar_slave_state;
    }

    if(!root["imuTopic"].isNull()) {
        rbtInfoValue["robot_info"]["gyro"]["error"] = root["imuTopic"].asBool() ? 0 : 1;
    }

    if(!root["odomDeltaSpeed"].isNull()){
        odom_delta_spd = root["odomDeltaSpeed"].asBool() ? 0 : 1;
        rbtInfoValue["robot_info"]["odom"]["error"] = odom_delta_spd;
    }

    if(!root["pointcloudTopic"].isNull()){
        rgbd_state = root["pointcloudTopic"].asBool() ? 0 : 1;
        rbtInfoValue["robot_info"]["rgbd"]["error"] = rgbd_state;
    }

    rbtInfoValue["robot_info"]["gaussian_status"] = root;

    strRbtInfo = fw.write(rbtInfoValue);
    snprintf(rbtInfo.json, sizeof(rbtInfo.json), "%s", strRbtInfo.c_str());
    rostopic::RobotInfo_publish(rbtInfo);
}

void Navigation::_on_status(int code, const Value& json){

    switch(code){
        case 100:
            _on_pause();
            break;
        case 200:
            //on_stopped();
            break;
        case 300:
            _on_path_invalid();
            break;
        case 301:
            _on_path_to_start_point();//导航到轨道起点处
            break;
        case 302://轨道行走中
            _on_path_running(json);
            break;
        case 303://轨道中行走，遇到障碍物，等待
            _on_path_block();
            break;
        case 304:
            _on_path_avoiding_obstacle();
            break;
        case 305://不能到达
            _on_path_unreachable();
            break;
        case 306://轨道行走结束
            _on_path_finish();
            break;
        case 401://localization error
            _on_localization_error();
            break;
        case 402://goal not safe
            _on_goal_point_not_safe();
            break;
        case 403://前方遇到障碍物
            _on_nav_block();
            break;
        case 404:
            _on_nav_unreached();
            break;
        case 405: //navigating
            _on_nav_navigating();
            break;
        case 406: //planning
            _on_nav_planning();
            break;
        case 407:
            _on_nav_reached();
            break;
        case 408:
            _on_nav_unreachable();
            break;
        case 1006:
            _on_locate_proc(code);
            break;
        default:
            break;
    }
}

int Navigation::publish_pos_status(int task_type, int status_code)
{
    log_info("report pos and status");
    //report state and position
	GsPos cur_pos;
    int ret = GsApi::get_instance()->nav_get_pos(cur_pos);
    if(ret != ERR_OK)
        return ret;

#if 0
    int type = 0;
    if(nav_type == NAV_PATROL)
        type = 1;
    else if(nav_type == NAV_NAV_TO)
        type = 2;
    else if(nav_type == NAV_RETURN)
        type = 3;
    else if(nav_type == NAV_LOW_BAT_RETURN)
        type = 4;
    else if(nav_type == NAV_PATROL_AUTO)
        type = 5;
    else if(nav_type == NAV_CHARGE)
        type = 6;
    else if(nav_type == NAV_LOCATE)
        type = 7;
#endif

    Json::Value root;

    root["title"] = "response_nav_state";
    root["content"]["x"] = Json::Value(cur_pos.x);
    root["content"]["y"] = Json::Value(cur_pos.y);
    root["content"]["navtype"] = Json::Value(task_type);
    root["content"]["angle"] = Json::Value((float)(cur_pos.angle));
    root["content"]["state"] = Json::Value(status_code);

    int64_t now = (int64_t)tinyros::Time::now().toMSec();

    Json::FastWriter fw;
    std::string js_data = fw.write(root);
    roslib_msgs::SignalMessage resp;
    resp.account = Robot::pc.c_str();
    resp.msgID = std::to_string(now).c_str();
    resp.msg = js_data.c_str();
    rostopic::AgoraSendMessage_publish(resp);
	return ERR_OK;
}

void Navigation::publish_status(int task_type, const std::string& reason, int status)
{
    log_info("report state");


#if 0
    int type = 0;
    if(nav_type == NAV_PATROL)
        type = 1;
    else if(nav_type == NAV_RETURN)
        type = 3;
    else if(nav_type == NAV_NAV_TO)
        type = 2;
    else if(nav_type == NAV_LOW_BAT_RETURN)
        type = 4;
    else if(nav_type == NAV_PATROL_AUTO)
        type = 5;
    else if(nav_type == NAV_CHARGE)
        type = 6;
    else if(nav_type == NAV_LOCATE)
        type = 7;
#endif

    Json::Value root;
    root["title"] = "response_state";
    root["content"]["navtype"] = Json::Value(task_type);
    root["content"]["reason"] = Json::Value(reason);
    root["content"]["state"] = Json::Value(status);

    int64_t now = (int64_t)tinyros::Time::now().toMSec();

    Json::FastWriter fw;
    std::string js_data = fw.write(root);
    roslib_msgs::SignalMessage resp;
    resp.account = Robot::pc.c_str();
    resp.msgID = std::to_string(now).c_str();
    resp.msg = js_data.c_str();
    rostopic::AgoraSendMessage_publish(resp);
}

}

