#include "navigation.h"
#include "gs_api.h"

#define TASK_QUEUE_NAME "atris_task_queue"
#define STATUS_REPEAT_TIMES 20 

namespace nav{
Navigation(){
}

ERR_CODE Navigation::load_map(const string& name){
	return ERR_OK;
}

ERR_CODE Navigation::locate(const GsPos& pos){
	return ERR_OK;
}
	
ERR_CODE Navigation::to_point(const GsPos& pos){
	return ERR_OK;
}

void Navigation::_add_monitor(){
	GsNav::Get_Instance()->del_state_monitor(nav_monitor.id);
    _nav_mnt.cb = boost::bind(&status_cb, this, _1);
    GsNav::Get_Instance()->add_state_monitor(_nav_mnt);
	_repeat_times = 0;
}
	
ERR_CODE Navigation::to_point_flow_path(const GsPos& pos, const GsPath& path){
    log_info("%s", __FUNCTION__);
    int ret = ERR_OK;
	if (GsApi::Get_instance()->is_initialize_success())
		return ERR_GS_NOT_INITIALIZE;
	
	_add_monitor();
	
    vector<Value> js_tasks;
    Value task = gsapi->nav_new_nav_path_task(_map_name, _path, pos);
    js_tasks.push_back(task);
    ret = gsapi->nav_task_new(TASK_QUEUE_NAME, _map_name, _map_id, js_tasks);
    if(ret != ERR_OK){
        //todo zy
        log_error("new task fail.");
        return ret;
    }

    ret = gsapi->nav_merge_path_task_start(TASK_QUEUE_NAME, task, _map_name, _map_id);
    if(ret != ERR_OK){
        //todo zy
        log_error("task start fail.");
        return ret;
    }

	return ERR_OK;
}
	
ERR_CODE Navigation::flow_path(const GsPath& path){
	return ERR_OK;
}

bool Navigation::_is_located(){
	return ERR_OK;
}

bool Navigation::_is_loadedmap(){
	return ERR_OK;
}

int Navigation::_status_cb(char *data)
{
    Json::Reader reader;
    Json::Value root;

	//todo mebay bug
    //if(get_patrol_state() == PATROL_STATE_IDLE)
    //    return 0;

    //log_info("%s:%s", __FUNCTION__, data);
    if(!reader.parse(data, root)){
        log_error("parse gs status js data fail.");
        return ERR_INVALID_JSON;
    }

    int code = root["statusCode"].asInt();
    //过滤掉相同的状态上报
    if(code == _code){
        _repeat_times ++;
        if(_repeat_times == STATUS_REPEAT_TIMES){
            _repeat_times = 0;
        }
        else
            return ERR_OK;
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
        if(code != 200 && code != 407 && (code < 800 || code > 900)){
            if(code == 306){
                if(nav_type != NAV_PATROL || nav_type != NAV_PATROL_AUTO)
                    report_pos_state(_code);
            }
            else
                report_pos_state(_code);
        }

    }

    _on_status(code, root);

    return ERR_OK;
}

void Navigation::_on_pause()
{
    log_info("%s", __FUNCTION__);
}

void Navigation::_on_stopped()
{
    log_info("%s", __FUNCTION__);

    patrol_state = PATROL_STATE_IDLE;
}

void Navigation::_on_locate_proc(const int code)
{
    if(code == 1006){
        patrol_state = PATROL_STATE_IDLE;
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

    patrol_state = PATROL_STATE_IDLE;
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
    patrol_state = PATROL_STATE_IDLE;
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
    patrol_state = PATROL_STATE_IDLE;
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
    patrol_state = PATROL_STATE_IDLE;
}

void Navigation::_on_nav_unreached()
{
    log_info("%s", __FUNCTION__);
    patrol_state = PATROL_STATE_IDLE;
}

void Navigation::_on_nav_reached()
{
    log_info("on_nav_reached");

    //when on return or point navigation, don't do anything.
    if(nav_type == NAV_RETURN || nav_type == NAV_NAV_TO){
        patrol_state = PATROL_STATE_IDLE;
        if(nav_type == NAV_RETURN)
            report_pos_state(STATE_CODE_9200);

        return;
    }

    if(task_thread != NULL){
        task_thread->interrupt();
        task_thread->join();
        delete task_thread;
    }

#if 0
    task_thread = new boost::thread(boost::bind(&Patrol::do_detect_point_action,
                this, patrol_scheme, patrol_scheme->current_obs_point));
    //执行监测点任务
    //do_detect_point_action(patrol_scheme, patrol_scheme->current_obs_point);
#endif
    if(nav_type == NAV_CHARGE){
        task_thread = new boost::thread(boost::bind(&Patrol::do_charge_action,
                    this, patrol_scheme));
        return;
    }

    task_thread = new boost::thread(boost::bind(&Patrol::do_detect_point_action,
                this, patrol_scheme, patrol_scheme->current_obs_point));
}

void Navigation::_on_status(int code, const Value& json)
{
    switch(code){
        case 100:
            on_pause();
            break;
        case 200:
            //on_stopped();
            break;
        case 300:
            on_path_invalid();
            break;
        case 301:
            on_path_to_start_point();//导航到轨道起点处
            break;
        case 302://轨道行走中
            on_path_running(json);
            break;
        case 303://轨道中行走，遇到障碍物，等待
            on_path_block();
            break;
        case 304:
            on_path_avoiding_obstacle();
            break;
        case 305://不能到达
            on_path_unreachable();
            break;
        case 306://轨道行走结束
            on_path_finish();
            break;
        case 401://localization error
            on_localization_error();
            break;
        case 402://goal not safe
            on_goal_point_not_safe();
            break;
        case 403://前方遇到障碍物
            on_nav_block();
            break;
        case 404:
            on_nav_unreached();
            break;
        case 405: //navigating
            on_nav_navigating();
            break;
        case 406: //planning
            on_nav_planning();
            break;
        case 407:
            on_nav_reached();
            break;
        case 408:
            on_nav_unreachable();
            break;
        case 1006:
            on_locate_proc(code);
            break;
        default:
            break;
    }
}

}

