#ifndef UBTACTIONTASK_H
#define UBTACTIONTASK_H
#include "ubttask.h"



class UBTActionTask : public UBTTask
{
public:
    UBTActionTask(const string& name, int type, int id = 0, TaskMode mode = MODE_FG, IUBTTaskMsgNotifyer* notifyer = nullptr);
    virtual ~UBTActionTask();

    virtual int operate(TaskAction action);
    virtual int handle_msg(int msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam);

    inline bool set_cnf(const ActionTaskConf& conf)
    {
        _audio = conf.audio;
        _ui = conf.ui;
        _audio_type = conf.audio_type;
        _ui_type = conf.ui_type;
    };

private:
    string _audio;
    string _ui;
    int    _audio_type;
    int    _ui_type;
};

#endif // UBTACTIONTASK_H
