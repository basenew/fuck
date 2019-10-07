#ifndef UBTTASK_H
#define UBTTASK_H

#include "ubttasktype.h"
#include "ubtmsg.h"

#include "UBTUIProxy/ubtuiproxy.h"
#include "UBTAudioProxy/ubtaudioproxy.h"
#include "ubttaskfactory.h"

using namespace std;
using namespace Alpha1E_SDK;

class UBTTask
{
public:
    UBTTask(const string& name, int type, int id, TaskMode mode, IUBTTaskMsgNotifyer* notifyer)
    :_id(id)
    ,_name(name)
    ,_type(type)
    ,_mode(mode)
    ,_exit(false)
    ,_notifyer(notifyer)
    ,_ui_proxy(nullptr)
    ,_audio_proxy(nullptr)
    {};
    virtual ~UBTTask() = 0;

    virtual int operate(TaskAction action) = 0;
    virtual int handle_msg(int msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam) = 0;
    inline virtual bool isexit(){return _exit;};
    inline virtual int switch_mode(TaskMode mode){_mode = mode;};

    inline const string& name(){return _name;};
    inline int type(){return _type;};
    inline int id(){return _id;};
    inline int mode(){return _mode;};

protected:
    int      _type;
    int      _id;
    bool     _exit;
    string   _name;
    TaskMode _mode;

    UBTUIProxy           _ui_proxy;
    UBTAudioProxy        _audio_proxy;
    IUBTTaskMsgNotifyer* _notifyer;
};

#endif // UBTTASK_H
