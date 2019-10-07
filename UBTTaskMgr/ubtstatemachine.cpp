#include "ubtstatemachine.h"
#include "ubtmsgmapping.h"
#include "ubtjsonparser.h"

#include <vector>
#include <algorithm>

#define JKEY_STATE_MSG_TABLE "state_msg_table"
#define JKEY_CUR_STATE       "cur_state"
#define JKEY_NEXT_STATE      "next_state"
#define JKEY_MSG             "msg"
#define JKEY_CUR_TASK        "cur_task"
#define JKEY_NEXT_TASK       "next_task"
#define JKEY_ACTION          "action"
#define JKEY_MODE            "mode"

#define JVAL_STATE_IDEL      "idle"
#define JVAL_STATE_WAKEUP    "wakeup"
#define JVAL_STATE_RECO      "reco"
#define JVAL_STATE_MUSIC     "music"
#define JVAL_STATE_FM        "fm"
#define JVAL_STATE_NEWS      "news"
#define JVAL_STATE_JOKE      "joke"
#define JVAL_STATE_IDIOM     "idiom"
#define JVAL_STATE_TRANSLATE "translate"

#define JVAL_ACTION_NONE     ""
#define JVAL_ACTION_START    "START"
#define JVAL_ACTION_STOP     "STOP"
#define JVAL_ACTION_PAUSE    "PAUSE"
#define JVAL_ACTION_RESUME   "RESUME"

#define JVAL_MODE_NONE       ""
#define JVAL_MODE_FG         "FG"
#define JVAL_MODE_BG         "BG"
#define JVAL_MODE_SILENCE    "SILENCE"


#define JVAL_MSG_WAKEUP      "UBTEVENT_MSC_MSG_WAKEUP"

struct JTASK_OP
{
    string action;
    string mode;
};


struct JSTATE_OP
{
    string cur_state;
    string next_state;
    string msg;
    JTASK_OP cur_task;
    JTASK_OP next_task;
};


using namespace std;

unordered_map<string, int> UBTStateMachine::s_str2int_state = {
    {"",STATE_NONE},
    {FIRST_POWERON_WAKEUP_VIDEO,STATE_FIRST_POWERON_WAKEUP_VIDEO},
    {FIRST_POWERON_QR_CODE,STATE_FIRST_POWERON_QR_CODE},
    {FIRST_POWERON_SELF_INTRODUCTION,STATE_FIRST_POWERON_SELF_INTRODUCTION},
    {POWERON,STATE_POWERON},
    {POWEROFF,STATE_POWEROFF},
    {IDLE,STATE_IDLE},
    {RECO,STATE_RECO},
    {MUTE,STATE_MUTE},
    {DISCONNECT,STATE_DISCONNECT},
    {NETWORK_CONFIG,STATE_NETWORK_CONFIG},
    {OTA_DOWNLOADING,STATE_OTA_DOWNLOADING},
    {BATTERY_CHARGING,STATE_BATTERY_CHARGING},
    {LOW_POWER,STATE_LOW_POWER},
    {DEEP_SLEEP,STATE_DEEP_SLEEP},
    {TTS,STATE_TTS},
    {OFFLINE_QUERY,STATE_OFFLINE_QUERY},
    {IDIOM,STATE_IDIOM},
    {TRANSLATE,STATE_TRANSLATE},
    {ONGOING_DIALOGUE,STATE_ONGOING_DIALOGUE},
    {MUSIC,STATE_MUSIC},
    {FM,STATE_FM},
    {NEWS,STATE_NEWS},
    {JOKE,STATE_JOKE},
    {PAUSED_MUSIC,STATE_PAUSED_MUSIC},
    {PAUSED_FM,STATE_PAUSED_FM},
    {PAUSED_NEWS,STATE_PAUSED_NEWS},
    {PAUSED_JOKE,STATE_PAUSED_JOKE},
    {ALERT,STATE_ALERT},
    {REMIND,STATE_REMIND},
    {QUERY_ALERT,STATE_QUERY_ALERT},
    {QUERY_REMIND,STATE_QUERY_REMIND},
    {BT_PLAYING,STATE_BT_PLAYING},
    {TOUCH_EXPERSSION,STATE_TOUCH_EXPERSSION},
    {MOVE_EXPERSSION,STATE_MOVE_EXPERSSION},
    {COIN,STATE_COIN},
};

unordered_map<int, string> UBTStateMachine::s_int2str_state = {
    {STATE_FIRST_POWERON_WAKEUP_VIDEO,FIRST_POWERON_WAKEUP_VIDEO},
    {STATE_FIRST_POWERON_QR_CODE,FIRST_POWERON_QR_CODE},
    {STATE_FIRST_POWERON_SELF_INTRODUCTION,FIRST_POWERON_SELF_INTRODUCTION},
    {STATE_POWERON,POWERON},
    {STATE_POWEROFF,POWEROFF},
    {STATE_IDLE,IDLE},
    {STATE_RECO,RECO},
    {STATE_MUTE,MUTE},
    {STATE_DISCONNECT,DISCONNECT},
    {STATE_NETWORK_CONFIG,NETWORK_CONFIG},
    {STATE_OTA_DOWNLOADING,OTA_DOWNLOADING},
    {STATE_BATTERY_CHARGING,BATTERY_CHARGING},
    {STATE_LOW_POWER,LOW_POWER},
    {STATE_DEEP_SLEEP,DEEP_SLEEP},
    {STATE_TTS,TTS},
    {STATE_OFFLINE_QUERY,OFFLINE_QUERY},
    {STATE_IDIOM,IDIOM},
    {STATE_TRANSLATE,TRANSLATE},
    {STATE_ONGOING_DIALOGUE,ONGOING_DIALOGUE},
    {STATE_MUSIC,MUSIC},
    {STATE_FM,FM},
    {STATE_NEWS,NEWS},
    {STATE_JOKE,JOKE},
    {STATE_PAUSED_MUSIC,PAUSED_MUSIC},
    {STATE_PAUSED_FM,PAUSED_FM},
    {STATE_PAUSED_NEWS,PAUSED_NEWS},
    {STATE_PAUSED_JOKE,PAUSED_JOKE},
    {STATE_ALERT,ALERT},
    {STATE_REMIND,REMIND},
    {STATE_QUERY_ALERT,QUERY_ALERT},
    {STATE_QUERY_REMIND,QUERY_REMIND},
    {STATE_BT_PLAYING,BT_PLAYING},
    {STATE_TOUCH_EXPERSSION,TOUCH_EXPERSSION},
    {STATE_MOVE_EXPERSSION,MOVE_EXPERSSION},
    {STATE_COIN,COIN}
};

UBTStateMachine::UBTStateMachine()
//:_state_table(nullptr)
//,_max_state(0)
//,_max_msg(0)
{
    for (int i = 0; i < STATE_MAX; i++)
    {
        for (int j = 0; j < UBTMSG_MAX; j++)
        {
            _state_table[i][j] = &_none_state_op;
        }
    }
}

UBTStateMachine::~UBTStateMachine()
{
//    SAFE_DELETE_ARRAY(_state_table);
    for (int i = 0; i < STATE_MAX; i++)
    {
        for (int j = 0; j < UBTMSG_MAX; j++)
        {
            if (_state_table[i][j] != &_none_state_op)
                SAFE_DELETE(_state_table[i][j]);
        }
    }
}

bool UBTStateMachine::_json2struct(cJSON* root)
{
    return true;
}

bool UBTStateMachine::_get_item_val(cJSON* root, const char* key, string& val)
{
    cJSON* item = cJSON_GetObjectItem(root, key);
    if(item && item->type == cJSON_String && item->valuestring)
    {
        val = item->valuestring;
        return true;
    }

    UBTUtil::print_msg("task %s not exist or not string", key);
    return false;
}

bool UBTStateMachine::_get_action(const string& action, TaskAction& naction)
{
    if (action == JVAL_ACTION_NONE)
        naction = ACTION_NONE;
    else if (action == JVAL_ACTION_START)
        naction = ACTION_START;
    else if (action == JVAL_ACTION_STOP)
        naction = ACTION_STOP;
    else if (action == JVAL_ACTION_PAUSE)
        naction = ACTION_PAUSE;
    else if (action == JVAL_ACTION_RESUME)
        naction = ACTION_RESUME;
    else
    {
        UBTUtil::print_msg("task invalid action:%s", action.c_str());
        return false;
    }

    return true;
}

bool UBTStateMachine::_get_mode(const string& mode, TaskMode& nmode)
{
    if (mode == JVAL_MODE_NONE)
        nmode = MODE_NONE;
    else if (mode == JVAL_MODE_FG)
        nmode = MODE_FG;
    else if (mode == JVAL_MODE_BG)
        nmode = MODE_BG;
    else if (mode == JVAL_MODE_SILENCE)
        nmode = MODE_SILENCE;
    else
    {
        UBTUtil::print_msg("task invalid mode:%s", mode.c_str());
        return false;
    }

    return true;
}

bool UBTStateMachine::load_config(const string& file_name)
{
    UBTUtil::print_msg("task UBTStateMachine::load_sate_config %s", file_name.c_str());
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

        //if (!_json2struct(root))break;

        vector<JSTATE_OP> vstate_op;
        cJSON* jstate_table = cJSON_GetObjectItem(root, JKEY_STATE_MSG_TABLE);
        if (jstate_table == nullptr)
        {
            UBTUtil::print_msg("task key:%s not exist", JKEY_STATE_MSG_TABLE);
            break;
        }

        UBTUtil::print_msg("task jstate_table:%p type:%d", jstate_table, jstate_table->type);
        int size = cJSON_GetArraySize(jstate_table);
        UBTUtil::print_msg("task size:%d", size);
        for(int i = 0; i < size; i++)
        {
            JSTATE_OP state_op;
            cJSON* jstate_op = cJSON_GetArrayItem(jstate_table, i);
            UBTUtil::print_msg("task jstate_op:%p type:%d", jstate_op, jstate_op->type);
            UBTJsonParser::get_item_val(jstate_op, JKEY_MSG, state_op.msg);
            UBTJsonParser::get_item_val(jstate_op, JKEY_CUR_STATE, state_op.cur_state);
            UBTJsonParser::get_item_val(jstate_op, JKEY_NEXT_STATE, state_op.next_state);

            cJSON* jcur_task = cJSON_GetObjectItem(jstate_op, JKEY_CUR_TASK);
            UBTJsonParser::get_item_val(jcur_task, JKEY_ACTION, state_op.cur_task.action);
            UBTJsonParser::get_item_val(jcur_task, JKEY_MODE, state_op.cur_task.mode);

            cJSON* jnext_task = cJSON_GetObjectItem(jstate_op, JKEY_NEXT_TASK);
            UBTJsonParser::get_item_val(jnext_task, JKEY_ACTION, state_op.next_task.action);
            UBTJsonParser::get_item_val(jnext_task, JKEY_MODE, state_op.next_task.mode);

            transform(state_op.cur_state.begin(), state_op.cur_state.end(), state_op.cur_state.begin(), ::toupper);
            transform(state_op.next_state.begin(), state_op.next_state.end(), state_op.next_state.begin(), ::toupper);
            transform(state_op.cur_task.action.begin(), state_op.cur_task.action.end(), state_op.cur_task.action.begin(), ::toupper);
            transform(state_op.cur_task.mode.begin(), state_op.cur_task.mode.end(), state_op.cur_task.mode.begin(), ::toupper);
            transform(state_op.next_task.action.begin(), state_op.next_task.action.end(), state_op.next_task.action.begin(), ::toupper);
            transform(state_op.next_task.mode.begin(), state_op.next_task.mode.end(), state_op.next_task.mode.begin(), ::toupper);
            UBTUtil::print_msg("task msg:%s [%s|%s|%s] [%s|%s|%s]", state_op.msg.c_str(),
                               state_op.cur_state.c_str(), state_op.cur_task.action.c_str(), state_op.cur_task.mode.c_str(),
                               state_op.next_state.c_str(), state_op.next_task.action.c_str(), state_op.next_task.mode.c_str());
            vstate_op.push_back(state_op);
        }

        for (JSTATE_OP jstate_op:vstate_op)
        {
            StateOp& state_op = *(new StateOp);
            state_op.msg = UBTMsgMapping::msg_str2int(jstate_op.msg);
            state_op.cur_state = s_str2int_state[jstate_op.cur_state];
            state_op.next_state = s_str2int_state[jstate_op.next_state];

            _get_action(jstate_op.cur_task.action, state_op.cur_task_op.action);
            _get_mode(jstate_op.cur_task.mode, state_op.cur_task_op.mode);

            _get_action(jstate_op.next_task.action, state_op.next_task_op.action);
            _get_mode(jstate_op.next_task.mode, state_op.next_task_op.mode);

            _set_state_op(state_op);
        }

        SAFE_DELETE(root);
        return true;
    }while (0);

    SAFE_DELETE(root);
    return false;
}

bool UBTStateMachine::_set_state_op(StateOp& state_op)
{
    if (state_op.cur_state < STATE_MAX && state_op.msg < UBTMSG_MAX && state_op.next_state < STATE_MAX)
    {
        UBTUtil::print_msg("task msg:%d [%d|%d|%d] [%d|%d|%d]", state_op.msg,
                           state_op.cur_state, state_op.cur_task_op.action, state_op.cur_task_op.mode,
                           state_op.next_state, state_op.next_task_op.action, state_op.next_task_op.mode);
        _state_table[state_op.cur_state][state_op.msg] = &state_op;
        return true;
    }

    UBTUtil::print_msg("task cur state:%d next state:%d msg:%d over range", state_op.cur_state, state_op.next_state, state_op.msg);
    return false;
}


StateOp& UBTStateMachine::get_sate_op(unsigned int state, unsigned int msg)
{
    UBTUtil::print_msg("task get_sate_op state:%d msg:%d", state, msg);
    if (_state_table != nullptr && state < STATE_MAX && msg < UBTMSG_MAX)
    {
        StateOp& state_op = *(_state_table[state][msg]);
        UBTUtil::print_msg("task [%d|%d|%d] [%d|%d|%d]",
                           state_op.cur_state, state_op.cur_task_op.action, state_op.cur_task_op.mode,
                           state_op.next_state, state_op.next_task_op.action, state_op.next_task_op.mode);
        return state_op;
    }

    return _none_state_op;
}
