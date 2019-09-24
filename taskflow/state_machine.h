#pragma once
#include <iostream>
#include <atomic>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <assert.h>

#define WAIT_FOREVER     (-1)

using namespace std;
using namespace std::chrono;

namespace nav{

class StateMachine{
public:
	enum STATE
	{
		IDLE,
		WAITING,
		RUNNING,
		PAUSED,
		FINISHED,
		ST_MAX
	};

	enum OP
	{
		WAIT,
		START,
		STOP,
		PAUSE,
		RESUME,
		OP_MAX,
	};

	STATE st_trans[ST_MAX][OP_MAX]{
	   //WAIT      START     STOP      PAUSE     RESUME
		{WAITING,  RUNNING,  IDLE,     IDLE,     IDLE    },//IDLE
		{WAITING,  RUNNING,  FINISHED, PAUSED,   RUNNING },//WAITING
		{WAITING,  RUNNING,  FINISHED, PAUSED,   RUNNING },//RUNNING
		{WAITING,  RUNNING,  FINISHED, PAUSED,   RUNNING },//PAUSED
		{FINISHED, FINISHED, FINISHED, FINISHED, FINISHED},//FINISHED
	};

	StateMachine(const string& name = ""):_st(IDLE), _name(name){};

	inline int status(){return _st;};
	inline const string& name(){return _name;};	
	inline void name(const string& name){_name = name;};

	inline virtual bool start() {cout << _name << " start" << endl; return _next_state(START);};
	inline virtual bool stop()  {cout << _name << " start" << endl; return _next_state(STOP);};
	inline virtual bool pause() {cout << _name << " start" << endl; return _next_state(PAUSE);};
	inline virtual bool resume(){cout << _name << " start" << endl; return _next_state(RESUME);};

	inline virtual void on_running() {cout << _name << " on_running" << endl;};
	inline virtual bool on_paused()  {cout << _name << " on_paused" << endl;};
	inline virtual void on_finished(){cout << _name << " on_finished"  << endl;};

	inline virtual bool is_idle()    {unique_lock<mutex> lock(_mt);return _st == IDLE;};
	inline virtual bool is_ready()   {unique_lock<mutex> lock(_mt);return _st == WAITING;};
	inline virtual bool is_paused()  {unique_lock<mutex> lock(_mt);return _st == PAUSED;};
	inline virtual bool is_running() {unique_lock<mutex> lock(_mt);return _st == RUNNING;};
	inline virtual bool is_finished(){unique_lock<mutex> lock(_mt);return _st == FINISHED;};

	inline virtual void wait(int ms = 10){
		cout << _name << " loop..." << endl;
		{
			unique_lock<mutex> lock(_mt);
			if (_st == IDLE)
			{
				_st = WAITING;
			}
			_cv.notify_one();
		}

		while (_st != FINISHED){
			unique_lock<mutex> lock(_mt);
			if (_st == FINISHED){
				cout << _name << " already finished" << endl;
				return;
			}

			cout << _name << " wait status to change..." << endl;
			while (_st == WAITING){
				_cv.wait(lock);
			}
			cout << _name << " status changed" << endl;
			//getchar();
			switch(_st)
			{
			case RUNNING:
				on_running();
				break;
			case PAUSED:
				on_paused();
				break;
			case FINISHED:
				on_finished();
				return;
			default:
				cout << _name << " error invalid status" << endl;
				assert(false);
				break;
			}
			_st = WAITING;
		}
	};

	inline virtual bool wait_waiting (int ms = WAIT_FOREVER){return wait(WAITING, ms);};
	inline virtual bool wait_running (int ms = WAIT_FOREVER){return wait(RUNNING, ms);};
	inline virtual bool wait_paused  (int ms = WAIT_FOREVER){return wait(PAUSED, ms);};
	inline virtual bool wait_finished(int ms = WAIT_FOREVER){return wait(FINISHED, ms);};

	inline virtual bool wait(int st, int ms){
		//cout << _name << " _st:" << _st << " wait st: " << st << " ms:" << ms << endl;
		unique_lock<mutex> lock(_mt);
		if (st == _st) return true;

		if (ms == WAIT_FOREVER){
			while (st != _st)_cv.wait(lock);
		}
		else{
			_cv.wait_for(lock, ms*milliseconds(1));
		}
		//cout << _name << " ret:" << bool(st==_st) << endl;
		return st == _st;
	};

private:
	bool _next_state(OP op)
	{
		unique_lock<mutex> lock(_mt);
		int st = st_trans[_st][op];
		cout << _name << " cur st:" << _st << " next st:" << st << " op:" << op<< endl;
		if (st != _st)
		{
			_st = st;
			_cv.notify_all();
			return true;
		}

		return false;
	};

protected:	
	int                _st;
	string		       _name;
	mutex              _mt;
	condition_variable _cv;

};

}










