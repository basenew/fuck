#pragma once
#include <list>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <type_traits>

#include "task.h"
#include "task_node.h"
#include "task_thread.h"

using namespace std;

namespace nav{

class TaskFlow:public Task 
{
public:
	TaskFlow(const string& name = "")
	:Task(name)
	,_thd(nullptr){
	};

	virtual ~TaskFlow(){
		for (auto thd:_thds){
			if (thd) delete thd;
		}

		_thds.clear();

		if (_thd)
		{
			delete _thd;
			_thd = nullptr;
		}

		for (auto t:_tasks){
			if (t) delete t;	
		}
	};

	TaskFlow(const TaskFlow&)=delete;
	void operator=(const TaskFlow&)=delete;

	inline size_t total_count(){return _tasks.size() + _actives.size();};
	inline size_t running_count(){return _thds.size();};
	//inline void   push(TaskNode* t){_tasks.push_back(t);};
	inline TaskNode*  push(const string& name, TaskCB* cb = nullptr){
		TaskNode* t = new TaskNode(name);
		_tasks.push_back(t);
		t->cb(cb);
		return t;
	};

	virtual bool run(function<void()> on_finish = nullptr);

	inline virtual int stop(){
		if (ERR_OK == Task::stop())
		{
			for (auto t:_tasks)
				t->stop();

			for (auto thd:_thds)
				thd->stop();
		
		}
		return true;
	};
	
	inline bool empty(){return _tasks.empty();};

	inline TaskNode* operator[](size_t idx){
		if (_tasks.size() > idx) return _tasks[idx]; 
		return nullptr;
	};
private:
	static void _thread_proc(TaskFlow* tf)
	{
		if (tf) tf->_wait();
	};

	void _wait();

private:
	thread* _thd;

	vector<TaskNode*>  _tasks;	
	list<TaskNode*>  _actives;	
	//list<TaskNode*> _finised_tasks;	

	list<TaskThread*>  _thds;	
	function<void()>   _on_finish;
};

}
