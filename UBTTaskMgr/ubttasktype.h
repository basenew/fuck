#ifndef UBTTASKCONST_H
#define UBTTASKCONST_H

#include "ubtmsg.h"
#include "UBTMscProxy/ubtmscdefine.h"
#include "UBTUIProxy/ubtuidefine.h"

#include <iostream>
#include <vector>

using namespace std;

enum TaskAction
{
    ACTION_NONE,
    ACTION_START,
    ACTION_STOP,
    ACTION_PAUSE,
    ACTION_RESUME,
};

enum TaskMode
{
    MODE_NONE,
    MODE_FG,
    MODE_BG,
    MODE_SILENCE,
};

enum UIType
{
    UI_NONE,
    UI_WINDOW,
    UI_EXPERSSION
};

enum AudioType
{
    AUDIO_NONE,
    AUDIO_URI,
    AUDIO_PMT,
    AUDIO_TTS
};

enum State
{
    STATE_NONE,
    STATE_FIRST_POWERON_WAKEUP_VIDEO,
    STATE_FIRST_POWERON_QR_CODE,
    STATE_FIRST_POWERON_SELF_INTRODUCTION,
    STATE_POWERON,
    STATE_POWEROFF,
    STATE_IDLE,
    STATE_RECO,
    STATE_MUTE,
    STATE_DISCONNECT,
    STATE_NETWORK_CONFIG,
    STATE_OTA_DOWNLOADING,
    STATE_BATTERY_CHARGING,
    STATE_LOW_POWER,
    STATE_DEEP_SLEEP,
    STATE_TTS,
    STATE_OFFLINE_QUERY,
    STATE_IDIOM,
    STATE_TRANSLATE,
    STATE_ONGOING_DIALOGUE,
    STATE_MUSIC,
    STATE_FM,
    STATE_NEWS,
    STATE_JOKE,
    STATE_PAUSED_MUSIC,
    STATE_PAUSED_FM,
    STATE_PAUSED_NEWS,
    STATE_PAUSED_JOKE,
    STATE_ALERT,
    STATE_REMIND,
    STATE_QUERY_ALERT,
    STATE_QUERY_REMIND,
    STATE_BT_PLAYING,
    STATE_TOUCH_EXPERSSION,
    STATE_MOVE_EXPERSSION,
    STATE_COIN,
    STATE_MAX,
};

const string FIRST_POWERON_WAKEUP_VIDEO("FIRST_POWERON_WAKEUP_VIDEO");
const string FIRST_POWERON_QR_CODE("FIRST_POWERON_QR_CODE");
const string FIRST_POWERON_SELF_INTRODUCTION("FIRST_POWERON_SELF_INTRODUCTION");
const string POWERON("POWERON");
const string POWEROFF("POWEROFF");
const string IDLE("IDLE");
const string RECO("RECO");
const string MUTE("MUTE");
const string DISCONNECT("DISCONNECT");
const string NETWORK_CONFIG("NETWORK_CONFIG");
const string OTA_DOWNLOADING("OTA_DOWNLOADING");
const string BATTERY_CHARGING("BATTERY_CHARGING");
const string LOW_POWER("LOW_POWER");
const string DEEP_SLEEP("DEEP_SLEEP");
const string TTS("TTS");
const string OFFLINE_QUERY("OFFLINE_QUERY");
const string IDIOM("IDIOM");
const string TRANSLATE("TRANSLATE");
const string ONGOING_DIALOGUE("ONGOING_DIALOGUE");
const string MUSIC("MUSIC");
const string FM("FM");
const string NEWS("NEWS");
const string JOKE("JOKE");
const string PAUSED_MUSIC("PAUSED_MUSIC");
const string PAUSED_FM("PAUSED_FM");
const string PAUSED_NEWS("PAUSED_NEWS");
const string PAUSED_JOKE("PAUSED_JOKE");
const string ALERT("ALERT");
const string REMIND("REMIND");
const string QUERY_ALERT("QUERY_ALERT");
const string QUERY_REMIND("QUERY_REMIND");
const string BT_PLAYING("BT_PLAYING");
const string TOUCH_EXPERSSION("TOUCH_EXPERSSION");
const string MOVE_EXPERSSION("MOVE_EXPERSSION");
const string COIN("COIN");

struct ActionTaskConf
{
    string audio;
    string ui;
    int    audio_type;
    int    ui_type;
};

using VecATCnf = vector<ActionTaskConf*>;

class IUBTTaskMsgNotifyer
{
public:
    virtual bool notify_msg(UBTMSG msg, void* param_data, int data_len) = 0;
};

#endif // UBTTASKCONST_H
