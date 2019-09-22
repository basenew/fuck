#pragma once
#include <list>
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <condition_variable>
#include "task_proc.h"

using namespace std;
using namespace std::chrono;

namespace nav{

class Task:public TaskProc 
{
friend class TaskFlow;
friend class TaskThread;

public:
	Task(const string& name = "", TaskCB cb = nullptr):TaskProc(name, cb){
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

	inline bool is_ready(){return is_idle() && _fronts.empty();};
	inline bool is_last(){return  _behinds.empty();};

	inline list<Task*>& fronts(){return _fronts;};
	inline list<Task*>& behinds(){return _behinds;};

protected:
	list<Task*> _fronts;
	list<Task*> _behinds;
	TaskResult  _task_result;
};

}

