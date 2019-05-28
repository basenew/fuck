#ifndef MSGMAPPING_H
#define MSGMAPPING_H

#include "ubtmsg.h"

#include <iostream>
#include <unordered_map>

using namespace std;

class UBTMsgMapping
{
public:
    static UBTMSG msg_str2int(const string& msg);
    static string msg_int2str(UBTMSG msg);
private:
    static unordered_map<UBTMSG, string> s_int2str_msg;
    static unordered_map<string, UBTMSG> s_str2int_msg;
};

#endif // MSGMAPPING_H
