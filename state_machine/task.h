#pragma once
#include "../comm/comm.h"
#include "../comm/msg_list.h"
#include <iostream>

using namespace std;
using namespace comm;
namespace state_machine
{

	enum TSTATE
	{
		ST_INVALID,
		ST_IDLE,
		ST_WORKING,
		ST_PAUSED,
		ST_MAX,
	};
	
	enum TOP
	{
		OP_START,
		OP_STOP,
		OP_PAUSE,
		OP_RESUME,
		OP_MAX,
	};

	class Task:public MsgList, public MsgProc
	{
		public:
			Task(const string& name);
			virtual ~Task();
			virtual int init();
			virtual int start();
			virtual int stop();
			virtual int pause();
			virtual int resume();
			virtual bool operator()(int msg_type, char* data, int len);

			inline const string& name(){return _name;};
		private:
			int _op(int op);

			string _name;
			int    _st;
			int	   _tid;
	};
};
