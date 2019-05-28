#include "ubtjsonparser.h"
#include "ubtutil.h"
#include "ubtstatemachine.h"

#include <algorithm>

#define JKEY_ACTION_TASKS "action_tasks"
#define JKEY_AT_NAME      "name"
#define JKEY_AT_AUDIO     "audio"
#define JKEY_AT_UI        "ui"

using namespace std;

bool UBTJsonParser::get_item_val(cJSON* root, const char* key, string& val)
{
    if (root == nullptr)
    {
        UBTUtil::print_msg("task get_item_val %s root is null", key);
        return false;
    }

    cJSON* item = cJSON_GetObjectItem(root, key);
    UBTUtil::print_msg("task key:%s item:%p type:%d", key, item, item?item->type:-1);
    if(item && item->type == cJSON_String)
    {
        val = item->valuestring;
        return true;
    }

    UBTUtil::print_msg("task %s not exist or not string", key);
    return false;
}

bool UBTJsonParser::parse_action_task_conf(const string& file_name, VecATCnf& vcnf)
{
    //todo
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

        int    state;
        string name;
        cJSON* jitem, *jaction_tasks;
        ActionTaskConf* cnf;

        jaction_tasks = cJSON_GetObjectItem(root, JKEY_ACTION_TASKS);
        if (jaction_tasks == nullptr || jaction_tasks->type != cJSON_Array)
        {
            UBTUtil::print_msg("task key:%s not exist or not array", JKEY_ACTION_TASKS);
            break;
        }

        int size = cJSON_GetArraySize(jaction_tasks);
        UBTUtil::print_msg("task action task size:%d", size);
        for(int i = 0; i < size; i++)
        {
            jitem = cJSON_GetArrayItem(jaction_tasks, i);
            if (!get_item_val(jitem, JKEY_AT_NAME, name)) continue;

            transform(name.begin(), name.end(), name.begin(), ::toupper);
            state = UBTStateMachine::GetInstance()->state_str2int(name);
            if (state == STATE_NONE)
            {
                UBTUtil::print_msg("task invalid state name:%s", name.c_str());
                continue;
            }

            cnf = new ActionTaskConf;
            get_item_val(jitem, JKEY_AT_AUDIO, cnf->audio);
            get_item_val(jitem, JKEY_AT_UI, cnf->ui);
            //todo some system operate?
            vcnf[state] = cnf;
            UBTUtil::print_msg("task [%s|%s|%s]", name.c_str(), cnf->audio.c_str(), cnf->ui.c_str());
        }

        SAFE_DELETE(root);
        return true;
    }while (0);

    SAFE_DELETE(root);
    return false;
}
