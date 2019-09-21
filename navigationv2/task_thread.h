#pragma once
#include <list>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <functional>
#include <condition_variable>

#include "task_proc.h"

using namespace std;
using namespace std::chrono;

namespace nav{

class TaskThread
{
public:
	TaskThread(const string& name = "")
	:_name(name)
	,_thd(nullptr)
	,_running(false)
	,_exited(false)
	,_t(nullptr)
	{};
	
	virtual ~TaskThread(){
		if (_thd)
		{
			delete _thd;
			_thd = nullptr;
		}
	};

	TaskThread(const TaskThread&)=delete;
	void operator=(const TaskThread&)=delete;

	bool start()
	{
		cout << _name << " start" << endl;
		unique_lock<mutex> lock(_mt);
		if (_running) return true;

		_running = true;	
		_task_lst.clear();
		if (_thd == nullptr)
		  _thd = new thread(&_thread_proc_cb, this);

		cout << _name << " start ok" << endl;

		return true;
	};

	bool stop()
	{
		cout << _name << " stop..." << endl;
		unique_lock<mutex> lock(_mt);
		if (_thd == nullptr || !_running) return true;

		_running = false;	
		_cv.notify_one();
		lock.unlock();

		_thd->join();

		cout << _name << " stop ok" << endl;
		return true;
	};

	size_t push(TaskProc* t){
		if (t == nullptr) return 0;

		unique_lock<mutex> lock(_mt);
		_task_lst.push_back(t);
		_t = t;
		_cv.notify_one();
		return _task_lst.size();
	};

	TaskProc* task(){return _t;};
	
	bool is_exited(){return _exited;};

private:
	static void _thread_proc_cb(TaskThread* t)
	{
		if (t) t->_thread_proc();
	};

	void _thread_proc()
	{
		cout << _name << " running" << endl;
		
		while (_running)
		{
			unique_lock<mutex> lock(_mt);
			cout << _name << " wait..." << endl;
			//while (_running && _task_lst.empty())_cv.wait(lock);
			while (_running && (_t == nullptr || _t->is_finished())) _cv.wait(lock);
		
			cout << _name << " wait ok" << endl;

			if (!_running)
			{
				cout << _name << " break..." << endl;
				break;
			}

#if 0
			//while (!_task_lst.empty())
			{
				Task* t = _task_lst.front();
				_task_lst.pop_front();
				if (t)
					t->_thread_proc();
				if (t->is_last())
				  break;
			}
#endif
			if (_t)
			{
				_t->thread_proc();
			}
		}
		_task_lst.clear();
		_exited = true;
		cout << _name << " exit" << endl;
	};

private:
	bool    _running;
	bool    _exited;
	string  _name;
	thread* _thd;

	TaskProc*          _t;
	list<TaskProc*>    _task_lst;
	mutex              _mt;
	condition_variable _cv;
};

}

