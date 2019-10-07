#include "ubtactiontask.h"
#include "ubttasktype.h"

UBTActionTask::UBTActionTask(const string& name, int type, int id, TaskMode mode, IUBTTaskMsgNotifyer* notifyer)
:UBTTask(name, type, id, mode, notifyer)
{
    UBTUtil::print_msg("task name:%s type:%d id:%d mode:%d notifyer:%p",
                       _name.c_str(), _type, _id, _mode, _notifyer);
}

UBTActionTask::~UBTActionTask()
{

}

int UBTActionTask::operate(TaskAction action)
{
    UBTUtil::print_msg("task %s operate:%d", _name.c_str(), action);
    switch (action) {
    case ACTION_START:
    case ACTION_RESUME:
        _exit = false;
        if (_mode == MODE_FG)
        {
            if (!_ui.empty())
                _ui_proxy.showWindow(_ui);

            if (!_audio.empty())
                _audio_proxy.SendPlayVoicePromptCmd(_audio.c_str());
        }
        break;
    case ACTION_STOP:
        if (!_ui.empty())
            _ui_proxy.hideWindow(_ui);

        if (!_audio.empty())
            _audio_proxy.operation_stopPlayAudio(E_ASST_PLAYER_PROMPT);

        _exit = true;
        break;
    case ACTION_PAUSE:
        _exit = false;
        if (!_audio.empty())
            _audio_proxy.operate_pausePlayAudio(E_ASST_PLAYER_PROMPT);
        //todo hide window?
        break;
    default:
        UBTUtil::print_msg("task %s ignore:%d", _name.c_str(), action);
        break;
    }

    return 0;
}

int UBTActionTask::handle_msg(int msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam)
{
    UBTUtil::print_msg("task %s handle_msg:%d ignore", _name.c_str(), msg);
    return 0;
}
