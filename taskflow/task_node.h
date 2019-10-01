#pragma once
#include <list>
#include <thread>
#include <mutex>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <condition_variable>

#include "task.h"

using namespace std;
using namespace std::chrono;

namespace nav{

class TaskNode:public Task
{
friend class TaskFlow;
friend class TaskThread;
public:
	struct TaskNodeResult{
		enum RESULT{
			OK,
			ERR,
			IGR,
		};

		int ret;
		int len;
		void* param;
		TaskNodeResult():ret(ERR), len(0), param(nullptr){};
	};

public:
	TaskNode(const string& name = ""):Task(name){
		cout << "create task:" << _name << endl;
	};

	virtual ~TaskNode(){
		cout << "destroy task:" << _name << endl;
	};

	TaskNode(const TaskNode&) = delete;
	void operator=(const TaskNode&) = delete;

	inline bool front(TaskNode* t){
		cout << _name << " front " << t->_name << endl;
		auto it = find(begin(_behinds), end(_behinds), t);
		if (it != end(_behinds)) return true;

		_behinds.push_back(t);
		t->_fronts.push_back(this);
		return true;
	};

	inline bool behind(TaskNode* t){
		cout << _name << " behind " << t->_name << endl;
		auto it = find(begin(_fronts), end(_fronts), t);
		if (it != end(_fronts)) return true;

		_fronts.push_back(t);
		t->_behinds.push_back(this);
		return true;
	};

	inline bool rm_front(TaskNode* t){
		cout << _name << " rm front " << t->_name << endl;
		auto it = find(begin(_fronts), end(_fronts), t);
		if (it == end(_fronts)) return false;
		
		_fronts.erase(it);
		cout << _name << " font size: " << _fronts.size() << endl;
		return true;
	};

	inline bool rm_behind(TaskNode* t){
		cout << _name << " rm behind " << t->_name << endl;
		auto it = find(begin(_behinds), end(_behinds), t);
		if (it == end(_behinds)) return false;

		_behinds.erase(it);
		cout << _name << " behind size: " << _behinds.size() << endl;
		return true;
	};

	inline bool is_ready(){
		unique_lock<mutex> lock(_mt);
		return (_st == WTG || _st == IDL) && _fronts.empty();
	};

	inline bool is_last(){
		unique_lock<mutex> lock(_mt);
		return  _behinds.empty();
	};

	inline list<TaskNode*>& fronts() {return _fronts;};
	inline list<TaskNode*>& behinds(){return _behinds;};

protected:
	virtual void on_running()
	{
		cout << _name << " on_running" << endl;
		//getchar();
		//wait();
		this_thread::sleep_for(seconds(2));
		cout << _name << " on_running finished" << endl;
	};

private:
	list<TaskNode*> _fronts;
	list<TaskNode*> _behinds;
	TaskNodeResult  _task_result;
};

}

