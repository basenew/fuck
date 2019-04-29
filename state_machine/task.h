#pragma once
#include "../comm/comm.h"
#include "../comm/msg_list.h"
#include <iostream>

using namespace std;
namespace state_machine
{
enum TMsg
{
	TMSG_PRE_INIT,
	TMSG_AFT_INIT,
	TMSG_PRE_START,
	TMSG_AFT_START,
	TMSG_PRE_STOP,
	TMSG_AFT_STOP,
	TMSG_PRE_PAUSE,
	TMSG_AFT_PAUSE
	TMSG_PRE_RESUME,
	TMSG_AFT_RESUME,
};

class Task:public MsgList, public MsgProc
{
public:
	enum class TSTATE
	{
		ST_IDLE,
		ST_WORKING,
		ST_PAUSED,
		ST_MAX,
		ST_INVALID,
	};
	enum class TOP
	{
		OP_START,
		OP_STOP,
		OP_PAUSED,
		OP_RESUME,
		OP_MAX,
	};

public:
	Task(const string& name);
	inline const string& name(){return _name;};
	virtual ~ITask();
	virtual int init() = 0;
	virtual int start() = 0;
	virtual int stop() = 0;
	virtual int pause() = 0;
	virtual int resume() = 0;

	virtual bool operator()(int msg_type, char* data, int len);
private:
	string _name;
	int _tid;
	TSTATE _st;
};
};
