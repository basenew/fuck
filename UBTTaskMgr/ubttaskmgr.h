#ifndef UBTTASKMGR_H
#define UBTTASKMGR_H

#include "ubttaskmgr_global.h"
#include "ubtstatemachine.h"
#include "ubtmsg.h"
#include "ubtmsgmapping.h"
#include "ubttask.h"
#include "ubtactiontask.h"
#include "ubttaskfactory.h"

#include "ubtservice.h"
#include "ubttimer.h"

#include "iubthandlemessage.h"
#include "MsgListQueue.h"

#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

using TaskQ = list<UBTTask*>;
using TaskQIt = TaskQ::iterator;

//using ActionTaskMap = unordered_map<int, UBTActionTask*>;

using MsgQ = MsgListQueue<TaskMsg*>;


class UBTTASKMGRSHARED_EXPORT UBTTaskMgr: public UBTService, public IUBTHandleMessage,
        public zxLib::zxSingletonT<UBTTaskMgr>, public IUBTTaskMsgNotifyer
{
    friend class zxLib::zxSingletonT<UBTTaskMgr>;
public:
    ~UBTTaskMgr();
    bool initService();
    bool handleMessage(const zxLib::UBTMsgType& msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam);
    bool notify_msg(UBTMSG msg, void* param_data, int data_len);

private:
    UBTTaskMgr();
    bool serviceMain();
    bool _exe_task_op_mode(const StateOp& state_op);
    bool _load_action_task(const string& file_name);
    void _handle_task_msg(const TaskMsg& msg);
    void _find_fg_task();

    inline ActionTaskConf* _get_action_task_conf(int state)
    {
        if (state < _action_task_conf.size())
            return _action_task_conf[state];
        return nullptr;
    };

    inline bool _is_eixt(int ret_code)
    {
        return ret_code == -1;
    }

    UBTTask* _get_new_tasks(int state);

private:
    TaskQ    _readyq;
    TaskQ    _silenceq;

    int      _cur_state;
    MsgQ     _msgq;

    UBTTask*       _fg_task;
    UBTActionTask* _idle_task;

    //bool            _action_task_state[STATE_MAX];
    VecATCnf _action_task_conf;

    UBTStateMachine& _st_mch;
};

#endif // UBTTASKMGR_H
