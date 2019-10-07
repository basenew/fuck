#ifndef RECOTASK_H
#define RECOTASK_H

#include "ubtvstask.h"

class UBTRecoTask : public UBTVSTask
{
public:
    UBTRecoTask(IUBTTaskMsgNotifyer* notifyer = nullptr);
    virtual ~UBTRecoTask();

    virtual int operate(TaskAction action);
    virtual int handle_msg(int msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam);

    inline static UBTTask* CreateTask(IUBTTaskMsgNotifyer* notifyer)
    {
        UBTUtil::print_msg("task CreateTask reco");
        return (UBTTask*)(new UBTRecoTask(notifyer));
    };

protected:
    virtual int parse_json(const char* json);

private:
    int _on_reco_err(int state);
};

#endif // RECOTASK_H
