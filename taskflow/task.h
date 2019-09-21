#pragma once
#include <list>
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <condition_variable>
#include "state_machine.h"

using namespace std;
using namespace std::chrono;

namespace nav{

class Task:public StateMachine
{
friend class TaskFlow;
friend class TaskThread;
public:
	struct TaskResult{
		enum RESULT{
			OK,
			ERR,
			IGR,
		};

		int ret;
		int len;
		void* param;
		TaskResult():ret(ERR), len(0), param(nullptr){};
	};

public:
	Task(const string& name = ""):_name(name){
		cout << "create task:" << _name << endl;
	};

	~Task(){
		cout << "destroy task:" << _name << endl;
	};

	Task(const Task&) = delete;
	void operator=(const Task&) = delete;



	bool front(Task* t){
		cout << _name << " front " << t->_name << endl;
		auto it = find(begin(_behinds), end(_behinds), t);
		if (it != end(_behinds)) return true;

		_behinds.push_back(t);
		t->_fronts.push_back(this);
		return true;
	};

	bool behind(Task* t){
		cout << _name << " behind " << t->_name << endl;
		auto it = find(begin(_fronts), end(_fronts), t);
		if (it != end(_fronts)) return true;

		_fronts.push_back(t);
		t->_behinds.push_back(this);
		return true;
	};

	bool rm_front(Task* t){
		cout << _name << " rm front " << t->_name << endl;
		auto it = find(begin(_fronts), end(_fronts), t);
		if (it == end(_fronts)) return false;
		
		_fronts.erase(it);
		cout << _name << " font size: " << _fronts.size() << endl;
		return true;
	};
	bool rm_behind(Task* t){
		cout << _name << " rm behind " << t->_name << endl;
		auto it = find(begin(_behinds), end(_behinds), t);
		if (it == end(_behinds)) return false;

		_behinds.erase(it);
		cout << _name << " behind size: " << _behinds.size() << endl;
		return true;
	};

	bool start()
	{
		if (StateMachine::start())
		{
			_cv.notify_one();
			return true;
		}
		return false;
	}

	void wait()
	{
		cout << "wait " << _name << "..." << endl;
		unique_lock<mutex> lock(_mt);
		while (_st != FINISHED) _cv.wait(lock);
		cout << "wait " << _name << " ok" << endl;
	};

	inline const string& name(){return _name;};	
	inline void name(const string& name){_name = name;	};
	inline bool is_idle(){return _st == IDLE;};
	inline bool is_paused(){return _st == PAUSED;};
	inline bool is_running(){return _st == RUNNING;};
	inline bool is_finished(){return _st == FINISHED;};
	inline bool is_ready(){return is_idle() && _fronts.empty();};
	inline bool is_last(){return  _behinds.empty();};

	inline list<Task*>& fronts(){return _fronts;};
	inline list<Task*>& behinds(){return _behinds;};


protected:
	virtual void execute()
	{
		cout << _name << " execute" << endl;
		this_thread::sleep_for(seconds(1));	
	};

private:
	void _thread_proc()
	{
		cout << _name << " running" << endl;
		unique_lock<mutex> lock(_mt);
		while (_st != RUNNING)_cv.wait(lock);

		if (_st == RUNNING)
			execute();

		_st = FINISHED;
		_cv.notify_all();
	};

private:
	mutex              _mt;
	condition_variable _cv;

	string		_name;
	list<Task*> _fronts;
	list<Task*> _behinds;
	TaskResult  _task_result;
};

}

