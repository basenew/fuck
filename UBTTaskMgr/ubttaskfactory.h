#ifndef UBTTASKFACTORY_H
#define UBTTASKFACTORY_H

#include "ubttasktype.h"
#include "zxSingletonT.h"
#include "ubtutil.h"

class UBTTask;

class UBTTaskFactory:public zxLib::zxSingletonT<UBTTaskFactory>
{
    friend class zxLib::zxSingletonT<UBTTaskFactory>;
public:
    typedef UBTTask* (*CreateTask)(IUBTTaskMsgNotifyer* notifyer);

    ~UBTTaskFactory(){SAFE_DELETE_ARRAY(_fn_arr);};

    UBTTask* get_task(int type, IUBTTaskMsgNotifyer* notifyer)
    {
        if (type < _max_task_cnt)
        {
           CreateTask fn = _fn_arr[type];
           if (fn != nullptr)
               return fn(notifyer);
           else
               UBTUtil::print_msg("task get_task create task:%d fn is null", type);
        }
        else
            UBTUtil::print_msg("task type:%d over max", type);

        return nullptr;
    };

    bool register_task_creator(int type, CreateTask fn)
    {
        if (fn != nullptr && type > 0 && type < _max_task_cnt)
        {
            UBTUtil::print_msg("task register_task_creator type:%d", type);
            _fn_arr[type] = fn;
            return true;
        }
        UBTUtil::print_msg("task register_task_creator type:%d fn:%p fail", type, fn);
        return false;
    };

    bool unregister_task_creator(int type)
    {
        if (type > 0 && type < _max_task_cnt)
        {
            UBTUtil::print_msg("task unregister_task_creator type:%d", type);
            _fn_arr[type] = nullptr;
            return true;
        }
        UBTUtil::print_msg("task unregister_task_creator type:%d fail", type);
        return false;
    };

protected:
    UBTTaskFactory()
    :_max_task_cnt(STATE_MAX)
    ,_fn_arr(new CreateTask[STATE_MAX])
    {
    };

private:
    int _max_task_cnt;
    CreateTask* _fn_arr;
};

#endif // UBTTASKFACTORY_H
