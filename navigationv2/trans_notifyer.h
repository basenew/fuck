class TransNotifyer: public FileObject {
    private:
        uint32_t last_sec_;
        Json::Value values;

        void resp_progress(std::string result, std::string status, double progress ) {
            Json::Value response;
            int64_t now = (int64_t)tinyros::Time::now().toMSec();
            response["id"] = origin.msgID;
            response["timestamp"] = Json::Value(now);
            response["progress"] = progress;
            response["status"] = status;
            response["result"] = result;
            Robot::responseResult(origin, response, "response_sync_patrol_scheme");
        }

        virtual void progress(double progress)  {
            uint32_t now = tinyros::Time::now().sec;
            if ((now - last_sec_) >= DOWNLOAD_PROGRESS_INTERVAL) {
                resp_progress("success", "downloading", progress);
                last_sec_ = now;
            }

            if (now < last_sec_) {
                last_sec_ = now;
            }
        }

        virtual void notify(TransferStates state, std::string msg = "", int code = 0)  {
            if (state == TRANSFER_FILE_ERROR) {
                resp_progress("fail_download_error", "", 0);
            } else if (state == TRANSFER_FILE_STARTED) {
                resp_download_progress("success", "started", 0);
            } else if (state == TRANSFER_FILE_COMPLETED) {
                Json::Reader reader;
                Json::Value root;
                std::string mapname = "";
                reader.parse(origin.msg, root);
                if (!root["content"].isNull() && !root["content"]["mapname"].isNull()) {
                    mapname = root["content"]["mapname"].asString();
                } else {
                    log_error("%s mapname is empty!", __FUNCTION__);
                }

                patrol_sync_callback();
                patrol_update_callback(mapname);
                resp_download_progress("success", "finished", 100);
            }
        }

    public:
		TransNotifyer(bool download):last_sec_(0){};
        roslib_msgs::SignalMessage origin;
        boost::function<void ()> patrol_sync_callback;
        boost::function<void (std::string map_name)> patrol_update_callback;
		std::string result_fail;
};

