#pragma once
#include <iostream>
#include <json/json.h>
#include "robot/robot.h"
#include "tiny_ros/ros/time.h"
#include "log/log.h"
#include "kv_db.h"
#include "gs_api.h"

#define DB_MOD_NAV_NAME            "nav"
#define DB_KEY_USING_MAP           "using_map"
#define GET_USING_MAP_FROM_DB()    _db.get(DB_KEY_USING_MAP)
#define SET_USING_MAP_TO_DB(name)  _db.set(DB_KEY_USING_MAP, name)

static const string MAP_UPLOAD_TMP_PATH("/userdata/tmp/maps");

using namespace std;
using namespace Json;
using namespace roslib_msgs;

namespace nav{

class MapManager
{
public:
	inline const string& get_using_map(){return _using_map;};
	inline bool is_set_using_map(){return !_using_map.empty();};
	inline bool init(){
	    rostopic::AgoraReceiveMessage_subscribe(boost::bind(&MapManager::_handle_msg, this, _1));
		_db.select(DB_MOD_NAV_NAME);
		_using_map = GET_USING_MAP_FROM_DB();
		Utils::createDir(MAP_UPLOAD_TMP_PATH);
		return true;
	};

    static MapManager& get_instance(){
        static MapManager singleton;
        return singleton;
    };
private:
	int _upload_map(Value &req, Value &resp, string& result, const SignalMessage &msg);
	int _download_map(Value &req, Value &resp, string& result, const SignalMessage &msg);
	int _del_map(Value &req, Value &resp, string& result);
	int _set_using_map(Value &req, Value &resp, string& result);
	int _get_maps(Value &req, Value &resp, string& result);
	int _rename_map(Value &req, Value &resp, string& result);
	int _load_map(const string map_name);
	int _is_exit(const string map_name);

	inline static int _upload_cb(void* name){
		log_info("upload %s to gs box", ((string*)name)->c_str());
		return GsApi::get_instance()->map_upload(*(string*)name);
	};
	inline static int _download_cb(void* name){
		log_info("download %s to client", ((string*)name)->c_str());
		return GsApi::get_instance()->map_download(*(string*)name, MAP_UPLOAD_TMP_PATH);
	};
	void _handle_msg(const SignalMessage &msg);
private:
	MapManager(){};

	string _using_map;
	KVDB   _db;
};

} 
