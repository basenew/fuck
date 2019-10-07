#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "ubttasktype.h"
#include "ubtmsg.h"

#include "zxLib.h"
#include "ubtjson.h"

#include <unordered_map>


using namespace std;

struct TaskOp
{
    TaskOp()
    :action(ACTION_NONE)
    ,mode(MODE_NONE)
    {};

    //int action;
    //int   mode;
    TaskAction action;
    TaskMode   mode;
};

struct StateOp
{
    StateOp():msg(UBTMSG_NONE),cur_state(STATE_NONE), next_state(STATE_NONE){}
    int msg;
    int cur_state;
    int next_state;
    TaskOp cur_task_op;
    TaskOp next_task_op;
};

class UBTStateMachine:public zxLib::zxSingletonT<UBTStateMachine>
{
    friend class zxLib::zxSingletonT<UBTStateMachine>;
public:
    UBTStateMachine();
    ~UBTStateMachine();

    bool load_config(const string& file_name);
    StateOp& get_sate_op(unsigned int state, unsigned int msg);
    inline const string& state_int2str(int state)
    {
        if (state < STATE_MAX)
            return s_int2str_state[state];
        return "";
    };

    inline int state_str2int(const string& state)
    {
        return s_str2int_state[state];
    };

    inline bool is_valid(const StateOp& state_op)const {return &state_op != &_none_state_op;};
    inline bool is_skip(const StateOp& state_op)const {return &state_op == &_none_state_op;};
    inline bool is_state_trans(const StateOp& state_op)
    {
        UBTUtil::print_msg("task is trans:%d", (&state_op != &_none_state_op));
        return &state_op != &_none_state_op;
    };

private:
    bool _json2struct(cJSON* root);
    bool _get_item_val(cJSON* root, const char* key, string& val);
    bool _get_action(const string& action, TaskAction& naction);
    bool _get_mode(const string& mode, TaskMode& nmode);
    bool _set_state_op(StateOp& state_op);

private:
    StateOp* _state_table[STATE_MAX][UBTMSG_MAX];
    StateOp  _none_state_op;
//    unsigned int _max_state;
//    unsigned int _max_msg;

    static unordered_map<int, string> s_int2str_state;
    static unordered_map<string, int> s_str2int_state;
};

#endif // STATEMACHINE_H
