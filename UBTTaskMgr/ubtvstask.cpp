#include "ubtvstask.h"

UBTVSTask::UBTVSTask(const string& name, int type, int id, TaskMode mode, IUBTTaskMsgNotifyer* notifyer)
:UBTTask(name, type, id, mode, notifyer)
,_msc_proxy(nullptr)
{

}
