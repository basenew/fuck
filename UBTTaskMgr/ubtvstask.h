#ifndef UBTVSTASK_H
#define UBTVSTASK_H

#include "ubttask.h"
#include "UBTMscProxy/ubtmscmgrproxy.h"


class UBTVSTask : public UBTTask
{
public:
    UBTVSTask(const string& name, int type, int id, TaskMode mode, IUBTTaskMsgNotifyer* notifyer);
    virtual ~UBTVSTask() = 0;

protected:
    virtual int parse_json(const char* json) = 0;
    UBTMscMgrProxy _msc_proxy;
};

#endif // UBTVSTASK_H
