#pragma once
#include <iostream>
#include <atomic>
#include "../navigation/nav_error_code.h"

using namespace std;

namespace nav{

class StateMachine{
public:
	//todo may be protected
	enum STATE
	{
		IDLE,
		RUNNING,
		PAUSED,
		FINISHED,
		ST_MAX
	};

	enum OP
	{
		START,
		STOP,
		PAUSE,
		RESUME,
		OP_MAX,
	};

	STATE st_trans[ST_MAX][OP_MAX]{
		{RUNNING,  IDLE,     IDLE,     IDLE    },
		{RUNNING,  FINISHED, PAUSED,   RUNNING },
		{RUNNING,  FINISHED, PAUSED,   RUNNING },
		{FINISHED, FINISHED, FINISHED, FINISHED},
	};

	StateMachine():_st(IDLE){};

	int state(){
		return _st;
	};

	virtual int start() {
		return _next_state(START);
	};

	virtual int stop(){
		return _next_state(STOP);
	};

	virtual int pause(){
		return _next_state(PAUSE);
	};

	virtual int resume(){
		return _next_state(RESUME);
	};


private:
	bool _next_state(OP op)
	{
		int st = st_trans[_st][op];
		cout << "cur st:" << _st << " next st:" << st << " op:" << op<< endl;
		if (st != _st)
		{
			_st = st;
			return ERR_OK;
		}

		return ERR_INVALID_OP;
	};

	
protected:
	//atomic_int  _st;
	int  _st;
};

}










