#include "navigation.h"
#include "map_manager.h"
#include "scheme_manager.h"
#include "transferfile/transferfile.h"
#include "trans_notifyer.h"

#define PARSE_JSON()    if(!reader.parse(msg.msg, req)){\
                            log_error("parse json fail.");\
                            ret = ERR_INVALID_JSON;\
                            break;\
                        }

namespace nav{

void SchemeManager::_handle_msg(const roslib_msgs::SignalMessage &msg){
    Json::Reader reader;
    Json::Value req, resp;

    int ret = ERR_OK;
    std::string result = "success";
    resp["id"] = msg.msgID;
    resp["timestamp"] = msg.timestamp;

    log_info("handle %s", msg.title.c_str());
    do{
		if(msg.title == "request_sync_patrol_scheme"){
			if (_sch.is_running()){
				ret = ERR_ALREADY_RUNNING;
				break;
			}

            PARSE_JSON(); 
            if(req["content"]["url"].isNull() || req["content"]["mapname"].isNull()){
                log_error("no patrol scheme url");
                result = "fail_invalid_data";
                ret = ERR_INVALID_FIELD;
                break;
            }

			string sch_name = req["content"]["mapname"].asString();
			string url = req["content"]["url"].asString();
			ret = download(msg, sch_name, url);
			if (ret == ERR_OK)return;
        }
		else if(msg.title == "request_set_scheme"){
            PARSE_JSON(); 
            if(req["content"]["mapname"].isNull() || req["content"]["schemeName"].isNull()){
                result = "fail_invalid_data";
                ret = ERR_INVALID_FIELD;
                break;
            }

			if (MapManager::get_instance().is_set_using_map()){
				ret = ERR_MAP_NOT_USING;
				break;
			}

            std::string map_name = req["content"]["mapname"].asString();
			if (map_name != MapManager::get_instance().get_using_map()){
                result = "fail_invalid_data";
				ret = ERR_MAP_MUST_EQ_USING;
				break;
			}
	
            string sch_name = req["content"]["schemeName"].asString();
    		string sch_path_name = Config::get_instance()->patrol_scheme_path + "/" + sch_name;
			ret = parse(sch_path_name);
        } 
        else if(msg.title == "request_get_scheme"){
            PARSE_JSON(); 
            if(req["content"]["mapname"].isNull()){
                result = "fail_invalid_data";
                ret = ERR_INVALID_FIELD;
                break;
            }
			string sch_name = req["content"]["mapname"].asString();
			ret = upload(msg, sch_name, resp);
        }
		else if(msg.title == "request_del_scheme") {//test case del in use scheme
            PARSE_JSON(); 
            if(req["content"]["mapname"].isNull()){
                result = "fail_invalid_data";
                ret = ERR_INVALID_FIELD;
                break;
            }

			//todo 
        }
		else if (msg.title == "request_switch_patrol") {
			if (_sch_parse_ret != ERR_OK){
				ret = _sch_parse_ret;
				break;
			}

            PARSE_JSON(); 
            int sw = req["content"]["switch"].asInt();
            switch(sw)
			{
			case SW_ENABLE:
                ret = _sch.start();
				break;
           	case SW_DISABLE: 
                ret = _sch.stop();
				break;
			case SW_PAUSE:
                ret = _sch.pause();
				break;
			default:
				ret = ERR_INVALID_VALUE;
				break;
            }

            resp["switch"] = Value(sw);
        }
        else if(msg.title == "request_get_patrol_status"){
            PARSE_JSON(); 
            resp["switch"] = _sch.sch_status();
        }
		else if(msg.title == "request_return") {
            PARSE_JSON(); 
			ret = _to_return_point(req, resp, result);			
		}
        else if(msg.title == "request_auto_patrol_enable"){
            PARSE_JSON(); 
            //todo auto_patrol_enable = req["enable"].asInt();
        }
		else return;
	} while (0);

    resp["result"] = result;
    resp["result_code"] = ret;
    Robot::responseResult(msg, resp, "response" + msg.title.substr(7));
}

int SchemeManager::download(const SignalMessage &origin, const string &sch_name, const string &url){
    log_info("%s", __FUNCTION__);
	if (sch_name.empty() || url.empty()) return ERR_INVALID_PARAM;

    string file_path = Config::get_instance()->patrol_scheme_path + "/" + sch_name;
    log_info("download %s url:%s", file_path.c_str(), url.c_str());

    boost::shared_ptr<TransNotifyer> obj = boost::shared_ptr<TransNotifyer> (new TransNotifyer(true));

    obj->origin = origin;
    obj->remote_path = url;
    obj->local_path = file_path;
    obj->cb = boost::bind(&SchemeManager::parse, this, file_path);

    TransferFile::download(obj);

    return ERR_OK;
}

string SchemeManager::_gen_upload_url(const string &path_name){
    char md5buf[128] = {0};
    Utils::get_instance()->gen_md5(path_name.c_str(), md5buf);
    string file_md5 = md5buf;
    return QINIU_BUCKET_NAME + file_md5;

}

int SchemeManager::upload(const SignalMessage &origin, const string &sch_name, Value& resp){
	if (sch_name.empty()) return ERR_INVALID_PARAM;

    string file_path = Config::get_instance()->patrol_scheme_path + "/" + sch_name;
	string url = _gen_upload_url(file_path);
    if(TransferFile::uploaded(url)){
        log_info("already upload, don't upload again.");
        resp["result"] = "success";
        resp["url"] = Json::Value(url);
        Robot::responseResult(origin, resp, "response_get_scheme");
        return ERR_OK;
    }
	
    boost::shared_ptr<TransNotifyer> obj = boost::shared_ptr<TransNotifyer> (new TransNotifyer(false));
	
    obj->origin = origin;
    obj->remote_path = url;
    obj->local_path = file_path;

    TransferFile::upload(obj);

    return ERR_OK;
}

int SchemeManager::_to_point(Value &req, Value &resp, string& result, GsPos pos){
	int ret = Navigation::get_instance().is_located();
	if (ret != ERR_OK)
		return ret;

    int sw = req["content"]["switch"].asInt();
    resp["switch"] = Json::Value(sw);

	//todo not impl
    switch(sw){
	case 1:{
        //ret = Navigation::get_instance()->to_point(pos);
		break;
    }
    case 0:{
        //ret = Navigation::get_instance()->cancel();
		break;
    }
    case 2:{
        //ret = Navigation::get_instance()->pause();
		break;
    }
	default:
		break;
	}
	return ret;
}

int SchemeManager::_to_return_point(Value &req, Value &resp, string& result){
	if (!_is_standby()){
        log_error("scheme not ready err:%d", _sch_parse_ret);
		return _sch_parse_ret;
	}

	return _to_point(req, resp, result, _sch.return_point().pos);
}
int SchemeManager::parse(const string& scheme_file){
    Reader reader;
    Value  root;
	int    ret = ERR_OK;

	_sch_parse_ret = ERR_PARSE_SCHEME_FAIL;
    FILE *file = nullptr;
    char *js_scheme_file = nullptr;
	do{
    	log_info("parse %s", scheme_file.c_str());
    	file = fopen(scheme_file.c_str(), "r");
    	if(file == NULL){
    	    log_error("open %s fail", scheme_file.c_str());
    	    ret = ERR_OPEN_FILE_FAIL;
			break;
    	}

    	fseek(file, 0 , SEEK_END);
    	int len = ftell(file);
    	rewind(file);

    	js_scheme_file = new char[len];
		assert(js_scheme_file);
    	int ret = fread(js_scheme_file, 1, len, file);
    	if(ret != len){
    	    log_error("read %s fail", scheme_file.c_str());
    	    ret = ERR_READ_FILE_FAIL;
			break;
    	}


    	if(!reader.parse(js_scheme_file, root)){
    	    log_error("parse json file %s fail", scheme_file.c_str());
    		ret = ERR_INVALID_JSON;
			break;
    	}

	    if(root["mapName"].isNull() || root["schemes"].isNull()){
    	    log_error("map name or schemes is null");
    		ret = ERR_INVALID_JSON;
			break;
		}

		Value js_schemes = root["schemes"], js_scheme;
    	int sz = js_schemes.size();

    	string scheme_use = root["schemeUse"].asString();
    	log_info("scheme using:%s", scheme_use.c_str());
		ret = ERR_NONE_SCHEME;
    	for(int i = 0; i < sz; i++){
    	    js_scheme = js_schemes[i];
    	    string sch_name = js_scheme["schemeName"].asString();
    	    if(sch_name.compare(scheme_use) != 0)
    	        continue;

    	    log_info("load scheme:%s", sch_name.c_str());
			ret = _sch.parse(js_scheme);
			break;
		}

	}while(0);

	if (js_scheme_file) delete js_scheme_file;

	if (file) fclose(file);
	
	return ret;
}

}


