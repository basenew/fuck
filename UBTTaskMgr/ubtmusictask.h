#ifndef UBTMUSICTASK_H
#define UBTMUSICTASK_H

#include "ubttasktype.h"
#include "ubtvstask.h"

class UBTMusicTask : public UBTVSTask
{
public:
    UBTMusicTask(IUBTTaskMsgNotifyer* notifyer = nullptr);
    virtual ~UBTMusicTask();

    virtual int operate(TaskAction action);
    virtual int handle_msg(int msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam);

    inline static UBTTask* CreateTask(IUBTTaskMsgNotifyer* notifyer)
    {
        UBTUtil::print_msg("task CreateTask MUSIC");
        return dynamic_cast<UBTTask*>(new UBTMusicTask(notifyer));
    };

protected:
    virtual int parse_json(const char* json){return 0;};

};

#endif // UBTMUSICTASK_H
