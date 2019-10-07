#pragma once
#include <functional>

#include <json/json.h>

#include "transferfile/transferfile.h"
#include "robot/robot.h"
#include "log/log.h"
#include "tiny_ros/ros/time.h"

#define DOWNLOAD_PROGRESS_INTERVAL 1   // seconds

using namespace Json;
//using namespace Robot;
using namespace roslib_msgs;

namespace nav{

class TransNotifyer:public FileObject{
private:
    uint32_t last_sec_;

    void resp_progress(string result, string status, double progress){
        Value resp;
        int64_t now = (int64_t)tinyros::Time::now().toMSec();
        resp["id"] = origin.msgID;
        resp["url"] = remote_path;
        resp["name"] = name;
        resp["timestamp"] = Value(now);
        resp["progress"] = progress;
        resp["status"] = status;
        resp["result"] = result;
        Robot::responseResult(origin, resp, "response" + origin.title.substr(7));
    }

    virtual void progress(double progress){
        uint32_t now = tinyros::Time::now().sec;
        if ((now - last_sec_) >= DOWNLOAD_PROGRESS_INTERVAL){
            resp_progress("success", status_str, progress);
            last_sec_ = now;
        }

        if (now < last_sec_) {
            last_sec_ = now;
        }
    }

    virtual void notify(TransferStates st, string msg = "", int code = 0){
		state = st;
        if (state == TRANSFER_FILE_ERROR){
            resp_progress(result_fail, "", 0);
        }else if (state == TRANSFER_FILE_STARTED){
            resp_progress("success", "started", 0);
        }else if (state == TRANSFER_FILE_COMPLETED){
            if (cb)cb(param);
            resp_progress("success", result_success, 100);
        }
    }

public:
	typedef void (*ON_FINISHED)();
	TransNotifyer(bool download)
	:last_sec_(0)
	,status_str(download?"downloading":"uploading")
	,result_fail(download?"fail_download_error":"fail_upload_error")
	,result_success(download?"finished":"finish_upload")
	{};

	TransferStates state;
	string resp_str;
	string status_str;
	string result_fail;
	string result_success;
	string name;
	SignalMessage origin;
	void*  param;
	//ON_FINISHED   cb;
	std::function<int(void*)> cb;
};


}
