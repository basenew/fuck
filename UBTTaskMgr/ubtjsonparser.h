#ifndef JSONPARSER_H
#define JSONPARSER_H
#include "ubttasktype.h"
#include "ubtjson.h"
#include "ubttasktype.h"

class UBTJsonParser
{
public:
    static bool parse_action_task_conf(const string& file_name, VecATCnf& vcnf);
    static bool get_item_val(cJSON* root, const char* key, string& val);
};

#endif // JSONPARSER_H
