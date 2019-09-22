#pragma once
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <condition_variable>

#include "state_machine.h"

using namespace std;
using namespace std::chrono;

namespace nav{

class TaskProc:public StateMachine
{
public:
	struct TaskResult{
		enum RESULT{
			OK,
			ERR,
			IGR,
		};

		int  ret;
		long param1;
		long param2;
		TaskResult():ret(ERR), param1(0), param2(0){};
	};

	using TaskCB = function<void()>;

public:
	TaskProc(const string& name = "", TaskCB cb = nullptr):_name(name), _cb(cb){};
	TaskProc(const TaskProc&) = delete;
	void operator=(const TaskProc&) = delete;

	inline const string& name(){return _name;};	
	inline void name(const string& name){_name = name;};
	inline bool is_idle(){return _st == IDLE;};
	inline bool is_paused(){return _st == PAUSED;};
	inline bool is_running(){return _st == RUNNING;};
	inline bool is_finished(){return _st == FINISHED;};

	inline bool start()
	{
		if (StateMachine::start())
		{
			_cv.notify_one();
			return true;
		}
		return false;
	}

	inline TaskResult& result(){return _ret;};

	inline void wait()
	{
		cout << "wait " << _name << "..." << endl;
		unique_lock<mutex> lock(_mt);
		while (_st != FINISHED) _cv.wait(lock);
		cout << "wait " << _name << " ok" << endl;
	};

	inline void thread_proc()
	{
		cout << _name << " running" << endl;
		unique_lock<mutex> lock(_mt);
		while (_st != RUNNING)_cv.wait(lock);

		if (_st == RUNNING){
			if (_cb) _cb();
			else execute();
		}

		_st = FINISHED;
		_cv.notify_all();
	};

protected:
	virtual void execute()
	{
		cout << _name << " execute" << endl;
		this_thread::sleep_for(seconds(1));	
	};

protected:
	mutex              _mt;
	string             _name;
	TaskCB             _cb;
	TaskResult         _ret;
	condition_variable _cv;
};

}

