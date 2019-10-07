#include "gs_api.h"
#include "robot/robot.h"
#include "map_manager.h"
#include "trans_notifyer.h"
#include "nav_error_code.h"

namespace nav{

#define PARSE_JSON()    if(!reader.parse(msg.msg, req)){\
                            log_error("parse json fail.");\
                            ret = ERR_INVALID_JSON;\
                            break;\
                        }


void MapManager::_handle_msg(const SignalMessage &msg)
{
    Reader reader;
    Value  req, rsp;
    string result;
    int    ret = ERR_OK;

    do
    {
        if (msg.title == "request_sync_map"){
            PARSE_JSON();
			ret = _upload_map(req, rsp, result, msg);
			if (ret == ERR_OK) return;
        }
        else if (msg.title == "request_del_map"){
            PARSE_JSON();
			ret = _del_map(req, rsp, result);
        }
        else if (msg.title == "request_rename_map"){
            PARSE_JSON();
			ret = _rename_map(req, rsp, result);
        }
        else if (msg.title == "request_set_map"){
            PARSE_JSON();
			ret = _set_using_map(req, rsp, result);
        }
        else if (msg.title == "request_get_maps"){
            PARSE_JSON();
			ret = _get_maps(req, rsp, result);
        }
        else if(msg.title == "request_load_map"){
            PARSE_JSON();
			ret = _download_map(req, rsp, result, msg);
            //todo if (ret == ERR_OK) return;
        }
        else
            return;

    }while(0);

	if (result.empty())	result = get_err_msg(ret);
    rsp["id"] = req["content"]["id"];
    rsp["timestamp"] = req["content"]["timestamp"];
    rsp["result"] = result;
    rsp["result_code"] = ret;
    Robot::responseResult(msg, rsp, "response" + msg.title.substr(7));
}

int MapManager::_upload_map(Value &req, Value &rsp, string& result, const SignalMessage &msg){
    int sz = 0;
    if (req["content"]["maps"].isNull() || !req["content"]["maps"].isArray()
        || (sz = req["content"]["maps"].size()) <= 0) {
        result = "fail_invalid_data";
        log_error("maps field not set or not array");
        return ERR_INVALID_FIELD;
    }

    for(int i = 0; i < sz; i ++){
        Value value = req["content"]["maps"][i];
        boost::shared_ptr<TransNotifyer> obj = boost::shared_ptr<TransNotifyer> (new TransNotifyer(true));
        string m = value["name"].asString() + ".tar.gz";
        obj->local_path = Config::get_instance()->maps_dir + m;
        obj->remote_path = value["url"].asString();
        obj->name = value["name"].asString();
        obj->origin = msg;
        obj->param = &obj->name;
		obj->cb = &MapManager::_upload_cb;
        TransferFile::download(obj);
    }
	return ERR_OK;
}

int MapManager::_download_map(Value &req, Value &resp, string& result, const SignalMessage &msg){
    int size = 0;
    if (req["content"]["maps"].isNull() || !req["content"]["maps"].isArray()
        || (size = req["content"]["maps"].size()) <= 0) {
        result = "fail_invalid_data";
        log_error("maps field not set or not array");
        return ERR_INVALID_FIELD;
    }
	
    vector<GsMap> maps;
    int ret = GsApi::get_instance()->map_get_list(maps);
    if(ret != ERR_OK)
        return ret;

    if(maps.size() <= 0)
		return ERR_NONE_MAP;

	string map_path = MAP_UPLOAD_TMP_PATH;  
	Utils::createDir(map_path);
	bool map_not_found = true;
	Value js_maps = req["content"]["maps"];
	int sz = js_maps.size();
	for (int i = 0; i < sz; i++){
		string map_name = js_maps[i].asString();
		for (auto m:maps){
			if (m.name == map_name){
				string map_path_name = map_path + map_name + ".tar.gz";
				log_info("start upload map:%s", map_path_name.c_str());

        		resp["status"]   = Value("start_download");
        		resp["result"]   = Value("success");
        		resp["progress"] = Value("0.0");
        		Robot::responseResult(msg, resp, "response_load_map");
				
				ret =GsApi::get_instance()->map_download(map_name, map_path);
				if (ret != ERR_OK)
        		{
					log_info("download map:%s from gs err:%d", map_path_name.c_str(), ret);
        		    //resp["status"]   = Value("finish_download");
        		    //resp["result"]   = Value("success");
        		    //resp["progress"] = Value("100.0");
        		    //Robot::responseResult(msg, resp, "response_load_map");
        		    continue;
        		}

        		resp["status"]   = Value("finish_download");
        		resp["result"]   = Value("success");
        		resp["progress"] = Value("100.0");
        		Robot::responseResult(msg, resp, "response_load_map");

        		char md5[128] = {0};
        		Utils::get_instance()->gen_md5(map_path_name.c_str(), md5);
				string map_md5(md5);
				map_md5 += ".tar.gz";

				log_info("md5 map:%s", map_md5.c_str());

        		if(TransferFile::uploaded(map_md5)){
        		    log_info("file exist, don't upload again.");
        		    resp["url"]      = QINIU_BUCKET_NAME + map_md5;
        		    resp["name"]     = map_name;
        		    resp["progress"] = Value("100.0");
        		    resp["status"]   = "finish_upload";
        		    resp["result"]   = Value("success");
        		    Robot::responseResult(msg, resp, "response_load_map");

        		    std::string cmd = "/bin/rm " + map_path_name;
        		    system(cmd.c_str());
					map_not_found = false;
        		    continue;
        		}

        		//upload file to cloud server
        		boost::shared_ptr<TransNotifyer> upload_file = boost::shared_ptr<TransNotifyer> (new TransNotifyer(false));
        		upload_file->local_path = map_path_name;
        		upload_file->remote_path = QINIU_BUCKET_NAME + map_md5;
        		upload_file->deleteDays = 1;
        		upload_file->name = map_name;
        		upload_file->origin = msg;
        		TransferFile::upload(upload_file);
				map_not_found = false;

				Value mm;
				while(true){
        		    if(upload_file->state == TRANSFER_FILE_COMPLETED){
        		        mm["name"] = Value(map_name);
        		        mm["url"] = Value(upload_file->remote_path);
        		        resp["maps"].append(mm);
        		        log_info("success upload map:%s", map_name.c_str());
        		        result = "success";
        		        std::string cmd = "/bin/rm " + map_path_name;
        		        system(cmd.c_str());
        		        break;
        		    }
        		    else if(upload_file->state == TRANSFER_FILE_ERROR){
        		        log_error("upload log files fail");
        		        ret = ERR_TRANSFER_FILE_FAIL;
        		        break;
        		    }
        		    usleep(500*1000);
        		}
				break;
			}
		}
	}

	return map_not_found? ERR_MAP_NOT_FOUND:ERR_OK;
}

int MapManager::_del_map(Value &req, Value &rsp, string& result)
{

    Value map_list;
    map_list = req["content"]["name"];
    Value del_map_list;
    int size = map_list.size();
    if(size <= 0)
    {
        result = "fail_invalid_data";
        return ERR_INVALID_FIELD;
    }
    else{
		int ret;
        for(int i = 0; i < size; i ++){
            string map = map_list[i].asString();
			if (_using_map == map){//todo test case
    		    log_error("can not rename using map:%s", map.c_str());
				ret = ERR_REMOVE_USING_MAP;
				continue;
			}
            ret = GsApi::get_instance()->map_del(map); //删除地图
            if(ret == ERR_OK)
                rsp["name"].append(Value(map));
        }
		return rsp["name"].size()?ERR_OK:ret;
	}
}

int MapManager::_rename_map(Value &req, Value &rsp, string& result)
{
    if(req["content"]["name"].isNull() || req["content"]["newname"].isNull()){
        result = "fail_invalid_data";
        return ERR_INVALID_FIELD;
    }

    string name = req["content"]["name"].asString();
    string newname = req["content"]["newname"].asString();
	if (name.empty()||newname.empty()){
        result = "fail_invalid_data";
        return ERR_INVALID_VALUE;
	}

	if (_using_map == name){//todo test case
        log_error("can not rename using map:%s", name.c_str());
		return ERR_RENAME_USING_MAP;
	}

    return GsApi::get_instance()->map_rename(name, newname);
}

int MapManager::_is_exit(const string map_name){
    vector<GsMap> maps;
    int ret = GsApi::get_instance()->map_get_list(maps);
    if(ret != ERR_OK)
        return ret;

    if(maps.size() <= 0)
		return ERR_NONE_MAP;

	for (auto m:maps){
		if (m.name == map_name)
			return ERR_OK;
	}

	return ERR_MAP_NOT_FOUND;
}

int MapManager::_load_map(const string map_name){
	if (_using_map == map_name)
		return ERR_OK;

	int ret = _is_exit(map_name);//todo test case
	if (ret != ERR_OK)
		return ret;

	if (ERR_OK != GsApi::get_instance()->get_init_status(map_name)){//todo test case
    	log_info("load %s replace %s", map_name.c_str(), _using_map.c_str());
    	ret = GsApi::get_instance()->map_load(map_name);
    	if(ret != ERR_OK){
    	    //todo zy
    	    log_error("load %s fail", map_name.c_str());
    	    return ret;
    	}
	}

	_using_map = map_name;
   	SET_USING_MAP_TO_DB(_using_map);
	//todo merge paths
	return ERR_OK;
}

int MapManager::_set_using_map(Value &req, Value &rsp, string& result)
{
    if(req["content"]["name"].isNull()){
        result = "fail_invalid_data";
        return ERR_INVALID_FIELD;
    }

    string map_name = req["content"]["name"].asString();

	return _load_map(map_name);
}

int MapManager::_get_maps(Value &req, Value &rsp, string& result){
    vector<GsMap> maps;
    int ret = GsApi::get_instance()->map_get_list(maps);
    if(ret != ERR_OK)
        return ret;

    if(maps.empty())
        rsp["maps"] = "";
    else{
        for(auto m:maps)
            rsp["maps"].append(Value(m.name));
    }

    rsp["map_using"] = _using_map;//todo
	return ERR_OK;
}

}

