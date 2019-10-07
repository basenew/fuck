#include "ubtttstask.h"
#include "UBTReminderProxy/ubtreminderproxy.h"
#include "UBTAlarmProxy/ubtalarmproxy.h"
//#include "UBTReminderProxy/ubtreminderdefine.h"

#include <algorithm>

UBTTTSTask::UBTTTSTask(IUBTTaskMsgNotifyer* notifyer)
:UBTVSTask(TTS, STATE_TTS, 0, MODE_FG, notifyer)
{

}

UBTTTSTask::~UBTTTSTask()
{
}

int UBTTTSTask::operate(TaskAction action)
{
    switch (action) {
    case ACTION_STOP:
        UBTUtil::print_msg("task TTS stop");
        _ui_proxy.stopCurrentExpression();
        _audio_proxy.operation_stopPlayAudio(E_ASST_PLAYER_TTS);
        _exit = true;
        break;
    case ACTION_PAUSE:
        UBTUtil::print_msg("task TTS pause");
        _exit = false;
        //_ui_proxy.stopCurrentExpression();
        _audio_proxy.operate_pausePlayAudio(E_ASST_PLAYER_TTS);
        break;
    case ACTION_RESUME:
        UBTUtil::print_msg("task TTS resume");
        _exit = false;
        _show_window(_ctx, _domain, _lparam);
        _audio_proxy.operate_resumePlayAudio(E_ASST_PLAYER_TTS);
        break;
    case ACTION_START:
        UBTUtil::print_msg("task TTS start");
        break;
    default:
        break;
    }

    return 0;
}

string UBTTTSTask::_get_weather_type(int type)
{
    switch (type)
    {
        case eDingdangWeatherSunny:
            return UBTWINDOW_NAME_WEATHER_FINE_DAY;
        case eDingdangWeatherCloudy:
            return UBTWINDOW_NAME_WEATHER_CLOUDY;
        case eDingdangWeatherOvercase:
            return UBTWINDOW_NAME_WEATHER_OVERCAST;
        case eDingdangWeatherDrizzle:
            return UBTWINDOW_NAME_WEATHER_LITTLE_RAIN;
        case eDingdangWeatherModerateRain:
            return UBTWINDOW_NAME_WEATHER_MIDDLE_RAIN;
        case eDingdangWeatherSleet:
            return UBTWINDOW_NAME_WEATHER_SLEET;
        case eDingdangWeatherHeavilyRain:
            return UBTWINDOW_NAME_WEATHER_BIG_RAIN;
        case eDingdangWeatherLightSnow:
            return UBTWINDOW_NAME_WEATHER_LITTLE_SNOW;
        case eDingdangWeatherModerateSnow:
            return UBTWINDOW_NAME_WEATHER_MIDDLE_SNOW;
        case eDingdangWeatherHeavilySnow:
            return UBTWINDOW_NAME_WEATHER_BIG_SNOW;
        case eDingdangWeatherWind:
            return UBTWINDOW_NAME_WEATHER_WIND;
        case eDingdangWeatherFog:
            return UBTWINDOW_NAME_WEATHER_FOG;
        case eDingdangWeatherThickHaze:
            return UBTWINDOW_NAME_WEATHER_HAZE;
        case eDingdangWeatherRainAndSnowMixed:
            return UBTWINDOW_NAME_WEATHER_RAINANDSNOW_MIXED;
        case eDingdangWeatherThunderShower:
            return UBTWINDOW_NAME_WEATHER_THUNDER_SHOWER;
        default:
            return "";
    }
}


int UBTTTSTask::_show_weather(const WearherMsg *msg)
{
    UBTUtil::print_msg("task TTS _show_weather");
    if(msg == nullptr) return -1;

    if (!msg->strCity.empty() && msg->nWeather != eDingdangWeatherUnknow)
    {
        UBTUtil::print_msg("UBTStateMgr::handleMessage strCity = %s nWeather = %d nTemperature = %d nMinTemperature = %d, nMaxTemperature =%d\n",
                           msg->strCity.c_str(), msg->nWeather, msg->nCurTemperature, msg->nMinTemperature, msg->nMaxTemperature);
        _ui_proxy.showWeatherWindow(msg->strCity, _get_weather_type(msg->nWeather), msg->nCurTemperature,
                                    msg->nMinTemperature, msg->nMaxTemperature);
    }
    else
        _ui_proxy.showWindow(UBTWINDOW_NAME_BLINK);

    return 0;
}

int UBTTTSTask::_show_idiom(IdiomMsg* msg)
{
    UBTUtil::print_msg("task TTS _show_idiom");
    if (msg && !msg->strIdiomText.empty())
    {
        UBTUtil::print_msg("zy show idiom:%s", msg->strIdiomText.c_str());
        _ui_proxy.showIdiomWindow(msg->strIdiomText);
    }
    else
    {
        if (_msc_proxy.IsPlayingIdiom() && !_msc_proxy.CurrentIdiom().empty())
        {
            UBTUtil::print_msg("zy show idiom2:%s", msg->strIdiomText.c_str());
            _ui_proxy.showIdiomWindow(_msc_proxy.CurrentIdiom());
        }
        else
        {
            UBTUtil::print_msg("zy err idiom msg is null");
            _ui_proxy.showWindow(UBTWINDOW_NAME_BLINK);
        }
    }

    return 0;
}

int UBTTTSTask::_show_reminder(int session, REMINDERLIST_INFO *msg)
{
    UBTUtil::print_msg("task TTS _show_reminder");
    std::string reminderNote;
    Alpha1E_SDK::UBTReminderProxy ubtReminderProxy(nullptr);

    if (session == 0 || msg)
    {
        _ui_proxy.showWindow(UBTWINDOW_NAME_BLINK);
        return 0;
    }

    if (msg->size() > 0)
    {
        std::sort(msg->begin(),
                  msg->end(),
                  [](const REMINDER_INFO &pre, const REMINDER_INFO &next)
                  {
                      return pre.lStart < next.lStart;
                  });

        int        number = 0;
        for (auto  &reminder:(*msg))
        {
            UBTUtil::print_msg("addScheduleListItem time:%d", reminder.lStart);
            reminderNote = reminder.sNote;
            _ui_proxy.addScheduleListItem(reminder.lStart, reminderNote);
            if (++number >= 3) break;
        }
        _ui_proxy.showWindow(UBTWINDOW_NAME_REMIND_LOOK_REMIND);
        /*todo
        m_uiReminderTimer.stop();
        m_uiReminderTimer.start(std::chrono::seconds(5),
                                [this]()
                                {
                                    _ui_proxy.removeAllScheduleListItems();
                                });*/
    }
    else if(!ubtReminderProxy.isAddFlagSet())
    {
        _ui_proxy.showWindow(UBTWINDOW_NAME_BLINK);
    }

    if(ubtReminderProxy.isAddFlagSet())
        ubtReminderProxy.setAddFlag(false);
    ubtReminderProxy.displayReminderIcon();
}

int UBTTTSTask::_show_alarm(int session, ALARMLIST_INFO *msg)
{
    UBTUtil::print_msg("task TTS _show_alarm");
    Alpha1E_SDK::UBTAlarmProxy ubtAlarmProxy(NULL);
    if (session == 0 || msg == nullptr)
    {
        _ui_proxy.showWindow(UBTWINDOW_NAME_BLINK);
        return 0;
    }

    if (msg->size() > 0)
    {
        std::sort(msg->begin(),
                  msg->end(),
                  [](const ALARM_INFO &pre, const ALARM_INFO &next)
                  {
                      return pre.lStart < next.lStart;
                  });
        int        number = 0;
        for (auto  &alarm:(*msg)) {
            UBTUtil::print_msg("addAlarmListItem time:%d", alarm.lStart);
            _ui_proxy.addAlarmListItem(alarm.lStart);
            if (++number >= 3) break; //max display 3 items
        }
        _ui_proxy.showWindow(UBTWINDOW_NAME_CLOCK_LOOK_CLOCK);
        /*todo
        m_uiAlarmTimer.stop();
        m_uiAlarmTimer.start(std::chrono::seconds(5),
                             [this]()
                             {
                                 _ui_proxy.removeAllAlarmListItems();
                             });*/
    }
    else if(!ubtAlarmProxy.isAddFlagSet())
    {
        _ui_proxy.showWindow(UBTWINDOW_NAME_BLINK);
    }
    if(ubtAlarmProxy.isAddFlagSet())
        ubtAlarmProxy.setAddFlag(false);
    ubtAlarmProxy.displayAlarmIcon();
}

string UBTTTSTask::_get_exp_type(int type)
{
    switch (type) {
    case eDingdangExpReplyLaugh:
        return UBTWINDOW_NAME_EXP_REPLY_LAUGH;
    case eDingdangExpReplyCryLoudly:
        return UBTWINDOW_NAME_EXP_REPLY_CRYLOUDLY;
    case eDingdangExpReplyCute:
        return UBTWINDOW_NAME_EXP_REPLY_CUTE;
    case eDingdangExpReplyKiss:
        return UBTWINDOW_NAME_EXP_REPLY_KISS;
    case eDingdangExpReplySleep:
        return UBTWINDOW_NAME_EXP_REPLY_SLEEP;
    case eDingdangExpReplyGreedy:
        return UBTWINDOW_NAME_EXP_REPLY_GREEDY;
    case eDingdangExpReplyForbidden:
        return UBTWINDOW_NAME_EXP_REPLY_FORBIDDEN;
    case eDingdangExpReplyLookUp:
        return UBTWINDOW_NAME_EXP_REPLY_LOOK_UP;
    case eDingdangExpReplyLookLeft:
        return UBTWINDOW_NAME_EXP_REPLY_LOOK_LEFT;
    case eDingdangExpReplyLookRight:
        return UBTWINDOW_NAME_EXP_REPLY_LOOK_RIGHT;
    case eDingdangExpReplyAngry:
        return UBTWINDOW_NAME_EXP_REPLY_ANGRY;
    case eDingdangExpReplyConfuse:
        return UBTWINDOW_NAME_EXP_REPLY_CONFUSE;
    default:
        return "";
    }
}

int UBTTTSTask::_show_experssion(ExpMsg* msg)
{
    UBTUtil::print_msg("task TTS _show_experssion");
    if(msg == nullptr) return -1;

    std::string exp = _get_exp_type(msg->nExpType);
    UBTUtil::print_msg("zy exp:%s", exp.c_str());
    if (!exp.empty())
        _ui_proxy.showWindow(exp);
    else
        _ui_proxy.hideWindow(UBTWINDOW_NAME_LISTENING);

    return 0;
}


int UBTTTSTask::_show_holiday(string* msg)
{
    /*
    if (msg && 0 == msg->compare(UBTDINGDANG_HOLIDAY_SEARCHTIME))
    {
        if (getCurrentState() == eUBTStateSleep)
        {
            UBTStateSleep *pStateSleep = dynamic_cast<UBTStateSleep*>(pState);
            if (pStateSleep)
            {
                pStateSleep->stopIdleTimer();
            }
        }
        _ui_proxy.showWindow(UBTWINDOW_NAME_DESKTOP);
        m_stateExector.submit([this](){
            usleep(5500000);
            ASSISTANT_CMD_PLAYBACK_STATUS_T playback;
            UBTAudioProxy audioProxy(NULL);
            audioProxy.operation_getPlayerState((LONG64)&playback);
            if (UBTStateBase::m_nBusyCount > 0
                    && playback.sBtStatus.eStatus == E_PLAYER_STAUTS_PLAYING
                    && getCurrentState() != eUBTStateLowPowerConsum)
            {
                _ui_proxy.showWindow(UBTWINDOW_NAME_BLUETOOTH_PLAYER);
                _ui_proxy.setBluetoothPlayerState(ePlayStatePlaying);
            }
        });
        _ui_proxy.stopCurrentExpression();
    }
    else
    {
        _ui_proxy.showWindow(UBTWINDOW_NAME_BLINK);
        UBTUtil::print_msg("Handle message = UBTEVENT_DINGDANG_MSC_TTS_FACE_EVENT3 holiday not search time\n");
    }*/
    return 0;
}

string UBTTTSTask::_get_chat_face_type(int type)
{
    switch (type)
    {
        case eDingdangChatFaceSmile:
            return UBTWINDOW_NAME_DIALOGUE_SMILE;
        case eDingdangChatFaceBlink:
            return UBTWINDOW_NAME_BLINK;
        case eDingdangChatFaceLove:
            return UBTWINDOW_NAME_BLINK;   //心心眼表情不在对话表情列表里面，用blink代替
        case eDingdangChatFacePuzzled:
            return UBTWINDOW_NAME_DIALOGUE_CONFUSE;
        case eDingdangChatFaceCute:
            return UBTWINDOW_NAME_BLINK; //卖萌表情不在对话表情列表里面，用blink代替
        case eDingdangChatFacePanic:
            return UBTWINDOW_NAME_BLINK;  //惊慌表情不在对话表情列表里面，用blink代替
        case eDingdangChatFaceAcquisitive:
            return UBTWINDOW_NAME_DIALOGUE_GREEDY;
        case eDingdangChatFaceAngry:
            return UBTWINDOW_NAME_DIALOGUE_ANGRY;
        case eDingdangChatFaceSnicker:
            return UBTWINDOW_NAME_DIALOGUE_GRIN;
        case eDingdangChatFaceShy:
            return UBTWINDOW_NAME_DIALOGUE_SHY;
        case eDingdangChatFaceDepressed:
            return UBTWINDOW_NAME_DIALOGUE_DEPRESSED;
        default:
            return UBTWINDOW_NAME_BLINK;
    }
}


int UBTTTSTask::_show_window(int ctx, zxLib::WPARAM domain, zxLib::LPARAM lparam)
{
    switch (domain) {
    case eUBTDingdangBotWeather:
        return _show_weather((WearherMsg*)lparam);
    case eUBTDingdangBotReminder:
        return _show_reminder(ctx, (REMINDERLIST_INFO*)lparam);
    case eUBTDingdangBotAlarm:
        return _show_alarm(ctx, (ALARMLIST_INFO*)lparam);
    case eUBTDingdangBotExperssion:
        return _show_experssion((ExpMsg*)lparam);
    case eUBTDingdangBotHoliday:
        return _show_holiday((string*)lparam);
    case eUBTDingdangBotGame_idiom:
        return _show_idiom((IdiomMsg*)lparam);
    case eUBTDingdangBotGetBattery:
    case eUBTDingdangBotGlobalCtrl:
    case eUBTDingdangBotNativeApp:
        _ui_proxy.hideWindow(UBTWINDOW_NAME_LISTENING);
        break;
    case eUBTDingdangBotChat:
        if((ChatFaceMsg*)lparam)
            return _ui_proxy.showWindow(_get_chat_face_type(((ChatFaceMsg*)lparam)->nFaceType));
        break;
    default:
        _ui_proxy.showWindow(UBTWINDOW_NAME_BLINK);
        break;
    }
    return 0;
}

int UBTTTSTask::handle_msg(int msg, int ctx, zxLib::WPARAM wparam, zxLib::LPARAM lparam)
{
    _domain = wparam;
    _ctx = ctx;
    _lparam = lparam;
    switch (msg) {
    case UBTMSG_DINGDANG_MSC_TTS_FACE_EVENT:
        _show_window(ctx, wparam, lparam);
        break;
    case UBTMSG_UBTAUDIO_TTS_DROP:
    case UBTMSG_UBTAUDIO_BT_PLAYFINISH:
    case UBTMSG_DINGDANG_MSC_TTS_FACE_END_EVENT:
        UBTUtil::print_msg("task TTS finish or drop or end");
        operate(ACTION_STOP);
        break;
//    case UBTMSG_SYSTEMKEY_SUBTYPE_TOUCH_A:
//    case UBTMSG_SYSTEMKEY_SUBTYPE_TOUCH_B1:
//        operate(ACTION_STOP);
//        break;
    default:
        UBTUtil::print_msg("task TTS ignore msg:%d", msg);
        break;
    }

    return 0;
}
