#ifndef UBTTTSTASK_H
#define UBTTTSTASK_H
#include "ubtvstask.h"
#include "ubttasktype.h"
#include "ubtmscmgr.h"
#include "UBTMscProxy/ubtmscmgrproxy.h"

class UBTTTSTask : public UBTVSTask
{
public:
    UBTTTSTask(IUBTTaskMsgNotifyer* notifyer = nullptr);
    virtual ~UBTTTSTask();

    virtual int operate(TaskAction action);
    virtual int handle_msg(int msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam);

    inline static UBTTask* CreateTask(IUBTTaskMsgNotifyer* notifyer)
    {
        UBTUtil::print_msg("task CreateTask TTS");
        return dynamic_cast<UBTTask*>(new UBTTTSTask(notifyer));
    };

protected:
    virtual int parse_json(const char* json){return 0;};

private:
    string _get_weather_type(int nWeatherType);
    string _get_exp_type(int type);
    string _get_chat_face_type(int type);
    int _show_window(int ctx, zxLib::WPARAM domain, zxLib::LPARAM lparam);
    int _show_weather(const WearherMsg *msg);
    int _show_idiom(IdiomMsg* msg);
    int _show_reminder(int session, REMINDERLIST_INFO *msg);
    int _show_alarm(int session, ALARMLIST_INFO *msg);
    int _show_experssion(ExpMsg* msg);
    int _show_holiday(string* msg);

private:
    int _ctx;
    zxLib::WPARAM _domain;
    zxLib::LPARAM _lparam;
};

#endif // UBTTTSTASK_H
