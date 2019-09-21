#pragma once
#include <list>
#include <assert.h>
#include <iostream>
#include <type_traits>

#include "task.h"
#include "task_thread.h"
#include "state_machine.h"

using namespace std;

namespace nav{

class TaskFlow:public StateMachine
{
public:
	inline TaskFlow(const string& name = "")
	:_name(name)
	,_thd(nullptr){
	};

	inline virtual ~TaskFlow(){
		for (auto thd:_thds)
		  if (thd) delete thd;
		_thds.clear();

		if (_thd)
		{
			delete _thd;
			_thd = nullptr;
		}
	};

	inline size_t total_count(){return _tasks.size() + _actives.size();};
	inline size_t running_count(){return _thds.size();};
	inline void   push(Task* t){_tasks.push_back(t);};

	bool start(function<void()> on_finish);

	TaskFlow(const TaskFlow&)=delete;
	void operator=(const TaskFlow&)=delete;

private:
	inline static void _thread_proc(TaskFlow* tf)
	{
		if (tf) tf->_wait();
	};

	void _wait();

private:
	string	_name;
	thread* _thd;

	list<Task*>  _tasks;	
	list<Task*> _actives;	
	//list<Task*> _finised_tasks;	

	list<TaskThread*>  _thds;	
	function<void()>  _on_finish;
};

}
