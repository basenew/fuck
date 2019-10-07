#include "ubttaskmgr.h"
#include "ubtstatemachine.h"
#include "ubtrecotask.h"
#include "ubtjsonparser.h"

#define ACTION_TASK_FILE  "action_task.json"
#define STATE_TABLE_FILE  "state_table.json"

UBTTaskMgr::UBTTaskMgr()
:_st_mch(*UBTStateMachine::GetInstance())
,_action_task_conf(STATE_MAX, nullptr)
{
}

UBTTaskMgr::~UBTTaskMgr()
{
    for (UBTTask* task:_readyq)
        SAFE_DELETE(task);

    for (UBTTask* task:_silenceq)
        SAFE_DELETE(task);

    for (ActionTaskConf* cnf:_action_task_conf)
        SAFE_DELETE(cnf);
}

UBTTask* UBTTaskMgr::_get_new_tasks(int state)
{
    UBTUtil::print_msg("task _get_new_tasks:%d :%s", state, _st_mch.state_int2str(state).c_str());
    ActionTaskConf* cnf = _get_action_task_conf(state);
    if (cnf != nullptr)
    {
        UBTActionTask* task = new UBTActionTask(_st_mch.state_int2str(state), state, 0, MODE_FG, this);
        task->set_cnf(*cnf);
        return task;
    }
    else
    {
        UBTUtil::print_msg("task create vs task");
        return UBTTaskFactory::GetInstance()->get_task(state, this);//todo
    }
}

bool UBTTaskMgr::_load_action_task(const string& file_name)
{
    UBTUtil::print_msg("task UBTTaskMgr::_load_action_task %s", file_name.c_str());
    cJSON* root = nullptr;
    do
    {
        std::string row_json;
        root = UbtJson::readJsonFile(file_name.c_str(), row_json);
        if (root == nullptr)
        {
            UBTUtil::print_msg("task load file:%s fail", file_name.c_str());
            break;
        }

        return true;
    }while (0);

    return false;
}

bool UBTTaskMgr::_exe_task_op_mode(const StateOp& state_op)
{
    UBTUtil::print_msg("task fg:%s before _exe_task_op_mode", _fg_task->name().c_str());
    if (!_st_mch.is_state_trans(state_op))
    {
        UBTUtil::print_msg("task fg:%s _exe_task_op_mode no trans", _fg_task->name().c_str());
        return true;
    }

    UBTUtil::print_msg("task state trans");
    UBTTask* next_task = nullptr;
    if (state_op.next_state != state_op.cur_state)
    {
        _get_new_tasks(state_op.next_state);
        if (next_task != nullptr)
        {
            next_task->switch_mode(state_op.next_task_op.mode);
            next_task->operate(state_op.next_task_op.action);
        }
        else
        {
            UBTUtil::print_msg("task get task by state:%d fail", state_op.next_state);
            return false;
        }
    }

    UBTTask* cur_task = _fg_task;
    if (cur_task != _idle_task)
    {
        cur_task->switch_mode(state_op.cur_task_op.mode);
        cur_task->operate(state_op.cur_task_op.action);
    }
    UBTUtil::print_msg("1111");

    if (next_task != nullptr)
    {
        if (!next_task->isexit())
        {
            switch (state_op.next_task_op.mode)
            {
            case MODE_BG:
                _readyq.push_front(next_task);
                break;
            case MODE_SILENCE:
                _silenceq.push_back(next_task);
                break;
            case MODE_FG:
                _fg_task = next_task;
                break;
            default:
                break;
            }
        }
        else
        {
            UBTUtil::print_msg("task %s deleted3", next_task->name().c_str());
            SAFE_DELETE(next_task);
        }
    }


    UBTUtil::print_msg("2222");
    if (!cur_task->isexit())
    {
        if (cur_task != _idle_task)
        {
            switch (state_op.cur_task_op.mode)
            {
            case MODE_SILENCE:
                _silenceq.push_back(cur_task);
                break;
            case MODE_BG:
                _readyq.push_front(cur_task);
                break;
            default:
                break;
            }
        }
    }
    else
    {
        if (_fg_task != cur_task)
        {
            UBTUtil::print_msg("task %s deleted2", cur_task->name().c_str());
            SAFE_DELETE(cur_task);
        }
        _find_fg_task();
    }

    UBTUtil::print_msg("task fg:%s after state trans", _fg_task->name().c_str());
    return true;
}

void UBTTaskMgr::_find_fg_task()
{
    while (_fg_task->isexit() || _fg_task->mode() != MODE_FG)//cur fg task exit by self
    {
        if (_fg_task->isexit())
        {
            UBTUtil::print_msg("task %s deleted1", _fg_task->name().c_str());
            SAFE_DELETE(_fg_task);
        }

        UBTUtil::print_msg("task 1111");
        if (_readyq.empty())
        {
            UBTUtil::print_msg("task 2222");
            _fg_task = _idle_task;
        }
        else
        {
            UBTUtil::print_msg("task 3333");
            _fg_task = _readyq.front();
            _readyq.pop_front();
            if (_fg_task == nullptr)
            {
                UBTUtil::print_msg("task invalid task from readyq");
                _fg_task = _idle_task;
            }
        }

        UBTUtil::print_msg("task 4444");
        _fg_task->switch_mode(MODE_FG);
        _fg_task->operate(ACTION_RESUME);
    }
    UBTUtil::print_msg("task 5555");
    _cur_state = _fg_task->type();
}

void UBTTaskMgr::_handle_task_msg(const TaskMsg& msg)
{
    UBTUtil::print_msg("task fg:%s _handle_task_msg:%s", _fg_task->name().c_str(), msg.str_msg.c_str());
    if (_fg_task == _idle_task)
    {
        UBTUtil::print_msg("task fg:idle not deal msg");
        return;
    }

    UBTUtil::print_msg("44444");
    _fg_task->handle_msg(msg.msg, msg.ctx, msg.wparam, msg.lparam);
    for (TaskQIt it = _readyq.begin(); it != _readyq.end();++it)
    {
        (*it)->handle_msg(msg.msg, msg.ctx, msg.wparam, msg.lparam);
        if ((*it)->isexit())
            _readyq.erase(it);
    }

    UBTUtil::print_msg("55555");
    for (TaskQIt it = _silenceq.begin(); it != _silenceq.end();++it)
    {
        (*it)->handle_msg(msg.msg, msg.ctx, msg.wparam, msg.lparam);
        if ((*it)->isexit())
            _silenceq.erase(it);
    }

    UBTUtil::print_msg("66666");
    _find_fg_task();
    UBTUtil::print_msg("task fg:%s after hdl msg", _fg_task->name().c_str());
}

bool UBTTaskMgr::initService()
{
    if (UBTService::initService())
    {
        string cur_dir = UBTUtil::getCurrentAppDir();
        string cnf = cur_dir + STATE_TABLE_FILE;
        if (_st_mch.load_config(cnf))
        {
            cnf = cur_dir + ACTION_TASK_FILE;
            if (UBTJsonParser::parse_action_task_conf(cnf, _action_task_conf))
            {
                ActionTaskConf* cnf =  _action_task_conf[STATE_IDLE];
                if (cnf != nullptr)
                {
                    _idle_task = new UBTActionTask(_st_mch.state_int2str(STATE_IDLE), STATE_IDLE);
                    _idle_task->set_cnf(*cnf);
                    _idle_task->operate(ACTION_START);
                    _fg_task = _idle_task;
                    _cur_state = STATE_IDLE;
                    m_bSrvLoop = true;
                    UBTUtil::print_msg("task UBTTaskMgr::initService ok");
                    return true;
                }
            }
        }
    }
    UBTUtil::print_msg("task UBTTaskMgr::initService fail");
    return false;
}

bool UBTTaskMgr::handleMessage(const zxLib::UBTMsgType& msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam)
{
    UBTUtil::print_msg("task post msg:%s", msg.c_str());
    TaskMsg& task_msg = *(new TaskMsg);
    task_msg.str_msg = msg;
    task_msg.msg = UBTMsgMapping::msg_str2int(msg);
    task_msg.ctx = ctx;
    task_msg.wparam = wparam;
    task_msg.lparam = lparam;
    _msgq.push_msg(&task_msg);
    return true;
}

bool UBTTaskMgr::notify_msg(UBTMSG msg, void* param_data, int data_len)
{
    return true;
}

bool UBTTaskMgr::serviceMain()
{
    TaskMsg* msg;
    if(_msgq.get_msg_from_list(msg))
    {
        UBTUtil::print_msg("task fg:%s hdl msg:%s", _fg_task->name().c_str(), msg->str_msg.c_str());
        StateOp& state_op = _st_mch.get_sate_op(_cur_state, msg->msg);

        _exe_task_op_mode(state_op);
        _handle_task_msg(*msg);

        //todo stop big sence in readyq by other big sence??
        UBTUtil::print_msg("task hdl msg:%s end", msg->str_msg.c_str());
        SAFE_DELETE(msg);
    }
    else
        UBTUtil::print_msg("task get_msg_from_list fail");
}
