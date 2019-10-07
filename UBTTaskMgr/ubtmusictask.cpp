#include "ubtmusictask.h"

UBTMusicTask::UBTMusicTask(IUBTTaskMsgNotifyer* notifyer)
:UBTVSTask(MUSIC, STATE_MUSIC, 0, MODE_FG, notifyer)
{

}

UBTMusicTask::~UBTMusicTask()
{

}


int UBTMusicTask::operate(TaskAction action)
{
    return 0;
}

int UBTMusicTask::handle_msg(int msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam)
{
    return 0;
}
