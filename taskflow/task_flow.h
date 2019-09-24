#pragma once
#include <list>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <type_traits>

#include "task.h"
#include "task_thread.h"
#include "state_machine.h"

using namespace std;

namespace nav{

class TaskFlow:public StateMachine
{
public:
	TaskFlow(const string& name = "")
	:StateMachine(name)
	,_thd(nullptr){
	};

	virtual ~TaskFlow(){
		for (auto thd:_thds){
		  if (thd) {
			  delete thd;
		  }
		}

		_thds.clear();

		if (_thd)
		{
			delete _thd;
			_thd = nullptr;
		}
	};

	TaskFlow(const TaskFlow&)=delete;
	void operator=(const TaskFlow&)=delete;

	inline size_t total_count(){return _tasks.size() + _actives.size();};
	inline size_t running_count(){return _thds.size();};
	inline void   push(Task* t){_tasks.push_back(t);};

	bool start(function<void()> on_finish){
		cout << _name << " start" << endl;
		if (StateMachine::start()){
			int i = 0;
			for (Task* t:_tasks){
				if (t->is_ready()){
					_actives.push_back(t);

					stringstream ss;
					ss << i++;
					TaskThread* thd = new TaskThread(ss.str());
					_thds.push_back(thd);
					thd->start();
					thd->push(t);
//					t->start();
				}
			}

			if (on_finish){
				_on_finish = on_finish;
				if (_thd)
				  delete _thd;

				_thd = new thread(_thread_proc, this);
				_thd->detach();
				cout << _name << " start ok" << endl;
			}
			else{
				cout << _name << " start ok to wait..." << endl;
				_wait();
			}
			return true;
		}

		cout << _name << " start fail" << endl;
		return false;
	};

	bool stop(){
		if (StateMachine::stop())
		{
			for (auto t:_tasks)
				t->stop();
		}
	}

private:
	static void _thread_proc(TaskFlow* tf)
	{
		if (tf) tf->_wait();
	};

	void _wait(){
		bool finished = false;
		cout << _name << " wait..." << endl;
		while (!is_finished() && !finished){
			finished = true;
			list<TaskThread*> thds = _thds;
			for (TaskThread* thd:thds){
				if (thd->is_exited()){
					cout << _name << "already stoped" << endl;
					continue;
				}

				Task* t = thd->task();
				assert(t);
				if (t->is_ready()){
					t->start();
					continue;
				}
				else if (!t->wait_finished(10)){
					//cout << _name << " " << t->name() << " not finished" << endl;
					finished = false;
					continue;
				}
				else if (is_finished()){
					cout << _name << " stop by user" << endl;
					thd->stop();
					continue;
				}
				else if (t->is_finished() && t->is_last()){
					cout << t->name() << " is finished and is last" << endl;
					thd->stop();
					continue;
				}

				bool only_one = true;
				list<Task*> behinds = t->behinds();	
				for (auto bt:behinds){
					t->rm_behind(bt);
					bt->rm_front(t);
					cout << bt->name() << " state:" << bt->status() << endl;
					if (bt->is_ready()){
						cout << bt->name() << " is ready" << endl;
						bt->start();
						if (only_one){
							only_one = false;
							thd->push(bt);
						}
						else{
							TaskThread* thd = new TaskThread();
							_thds.push_back(thd);
							thd->push(bt);
							thd->start();
						}
						finished = false;
					}
					else
						cout << bt->name() << " is not ready" << endl;
				}
			}
		}

		if (_on_finish)
		  _on_finish();

		cout << _name << " finish" << endl;
	};

private:
	thread* _thd;

	list<Task*>  _tasks;	
	list<Task*> _actives;	
	//list<Task*> _finised_tasks;	

	list<TaskThread*>  _thds;	
	function<void()>   _on_finish;
};

}
