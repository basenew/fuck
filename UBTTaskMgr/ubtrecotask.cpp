#include "ubtrecotask.h"
#include "ubtjson.h"
#include "ubtjsonparser.h"

#include "UBTUIProxy/ubtuiproxy.h"
#include "UBTUIProxy/ubtuidefine.h"
#include "ubtmscmgr.h"
#include "ubtaudioconfig.h"

#define JKEY_STATUS                "rc"                        //回复码,0 表示正常
#define JKEY_ERROR                 "error"                     //错误码,rc非0时返回
#define JKET_SESSION               "session"                   //会话是否结束,开发者根据此字段是否继续交互。
#define JKEY_SERVICE               "service"                   //领域
#define JKEY_SPEAK_TIPS            "speak_tips"                //播报提示
#define JKEY_TIPS                  "tips"                      //展示提示
#define JKEY_DATA                  "data"                      //各领域数据,每个领域不一样。(旧方式)
#define JKEY_TEMPLATE              "template"                  //无屏幕模板内容(用来替代 data 功能)
#define JKEY_ITEM                  "listItems"                 //列表，数组
#define JKEY_SEMANTIC_JSON         "semantic_json"             //后台返回的语义 Json 数据
#define JKEY_SEMANTIC              "semantic"                  //后台返回的语义

#define JKEY_ERR_CODE              "code"                       //错误码
#define JKEY_ERR_MSG               "message"                    //错误消息

#define JKEY_OPERATION             "operation"
#define JKEY_QUERY                 "query"

#define JKEY_TEXTCONTENT           "textContent"
#define JKET_SESSION               "session"                   //会话是否结束,开发者根据此字段是否继续交互。

enum StatusErrorValue
{
    STATUS_OK = 0,
    STATUS_APPID_INVALID,   //appid无效
    STATUS_APPKEY_INVALID,  //appkey无效
    STATUS_APPQPS_WARNING,  //appqps前一天峰值警告
    STATUS_APPQPS_REFUSE,   //appqps拒绝
    STATUS_APPQPD_WARNING,  //appqpd前一天峰值警告
    STATUS_APPQPD_REFUSE,   //appqpd拒绝
    STATUS_SRVLST_EMPTY     //请求的服务列表未配置
};


UBTRecoTask::UBTRecoTask(IUBTTaskMsgNotifyer* notifyer)
:UBTVSTask(RECO, STATE_RECO, 0, MODE_FG, notifyer)
{

}

UBTRecoTask::~UBTRecoTask()
{

}

int UBTRecoTask::operate(TaskAction action)
{
    UBTUtil::print_msg("task RECO operate:%d", action);
    switch (action) {
    case ACTION_START:
        UBTUtil::print_msg("task RECO start");
        break;
    case ACTION_STOP:
        UBTUtil::print_msg("task RECO stop");
        _ui_proxy.hideWindow(UBTWINDOW_NAME_LISTENING);
        _exit = true;
        break;
    case ACTION_PAUSE:
    case ACTION_RESUME:
    default:
        UBTUtil::print_msg("task RECO invalid action:%d", action);
        break;
    }

    return 0;
}

int UBTRecoTask::_on_reco_err(int state)
{
    int ret = 0;
    do
    {
        UBTUtil::print_msg("task RECO _on_reco_err:%d", state);
        UBTMscMgr* pMscService = dynamic_cast<UBTMscMgr*>(UBTModuleManager::getModule("UBTMscMgr"));
        if (pMscService)
        {
            pMscService->pauseMscSrv(eUBTMscSrvRecogCmd);
            pMscService->resumeMscSrv(eUBTMscSrvWakeup);
            break;
        }
        else
        {
            UBTUtil::print_msg("task RECO getModule UBTMscMgr fail");
            ret = -1;
            break;
        }

        std::string aduio;
        UBTAudioConfig audio_cnf;
        if (state == eRecogcmdStopError)
            aduio = audio_cnf.getAudioNetworkError();
        else if (state == eRecogcmdStopTimeOut)
            aduio = audio_cnf.getAudioRecogTimeout();

        if (!aduio.empty())
        {
            _ui_proxy.stopCurrentExpression();
            _audio_proxy.SendPlayVoicePromptCmd(aduio.c_str());
        }
        else
            _ui_proxy.hideWindow(UBTWINDOW_NAME_LISTENING);

    }while(0);

    _exit = true;
    return ret;
}

int UBTRecoTask::handle_msg(int msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam)
{
    switch (msg) {
    case UBTMSG_MSC_WAKEUP:
        _exit = false;
        _ui_proxy.stopCurrentExpression();
        _ui_proxy.showWindow(UBTWINDOW_NAME_LISTENING);
        break;
    case UBTMSG_MSC_SEMANTIC_RESULT:
        return parse_json((const char*)lparam);
    case UBTMSG_MSC_RECOGCMD_NORESULT:
    case UBTMSG_MSC_RECOGCMD_ERROR:
        return _on_reco_err((int)lparam);
    default:
        UBTUtil::print_msg("task RECO ignore msg:%d", msg);
        break;
    }
    return 0;
}

int UBTRecoTask::parse_json(const char* json)
{
    int ret = -1;
    cJSON* root = nullptr;
    do
    {
        if (json == nullptr)
        {
            UBTUtil::print_msg("task RECO json is null");
            break;
        }

        root = cJSON_Parse(json);
        if (root == nullptr)
        {
            UBTUtil::print_msg("task RECO invalid json:%s", json);
            break;
        }

        cJSON* jstatus = cJSON_GetObjectItem(root, JKEY_STATUS);
        if (jstatus == nullptr)
        {
            UBTUtil::print_msg("task RECO status item is null");
            break;
        }

        if (jstatus->valueint != STATUS_OK)
        {
            string err_code, err_msg;
            UBTJsonParser::get_item_val(jstatus, JKEY_ERR_CODE, err_code);
            UBTJsonParser::get_item_val(jstatus, JKEY_ERR_MSG, err_msg);
            UBTUtil::print_msg("task RECO err code:%s msg:%s", err_code.c_str(), err_msg.c_str());
            break;
        }

        MscSrvMsg srv_msg;

        srv_msg.semantic_json = json;
        cJSON* jession = cJSON_GetObjectItem(root, JKET_SESSION);
        if (jession != nullptr) srv_msg.session = jession->valueint;

        UBTJsonParser::get_item_val(root, JKEY_SERVICE, srv_msg.service);
        UBTJsonParser::get_item_val(root, JKEY_OPERATION, srv_msg.operation);
        UBTJsonParser::get_item_val(root, JKEY_SPEAK_TIPS, srv_msg.tts);

        do
        {
            if (!srv_msg.tts.empty()) break;

            cJSON* jdirectives = cJSON_GetObjectItem(root, JKEY_DATA);
            if (jdirectives == nullptr) break;

            cJSON* jitems = cJSON_GetObjectItem(jdirectives, JKEY_ITEM);
            if(jitems == nullptr) break;

            int size = cJSON_GetArraySize(jitems);
            if (size <= 0) break;

            cJSON* jitem = cJSON_GetArrayItem(jitems, 0);
            if (jitem == nullptr) break;

            UBTJsonParser::get_item_val(jitem, JKEY_TEXTCONTENT, srv_msg.tts);
        }while(0);

        _notifyer->notify_msg(UBTMSG_MSC_SERVICE_CREATE, &srv_msg, 0);
        ret = 0;
    }while (0);

    SAFE_DELETE(root);
    _exit = true;
    return ret;
}
