#include "task_thread.h"

namespace nav{

bool TaskThread::start()
{
	cout << _name << " start" << endl;
	unique_lock<recursive_mutex> lock(_mt);
	if (_running) return true;

	_running = true;	
	if (_thd == nullptr)
	{
		_thd = new thread(&_thread_proc_cb, this);
		_thd->detach();
	}

	cout << _name << " start ok" << endl;

	return true;
}

bool TaskThread::stop(){
	cout << _name << " stop...xxxxxxxxxxxxxxx0" << endl;
	unique_lock<recursive_mutex> lock(_mt);
	if (_thd == nullptr || !_running) return true;

	cout << _name << " stop... xxxxxxxxxxxxxxx1" << endl;
	if (_t)_t->stop();
	cout << _name << " stop... xxxxxxxxxxxxxxx2" << endl;
	_running = false;	
	_cv.notify_one();
	lock.unlock();
	cout << _name << " stop... xxxxxxxxxxxxxxx3" << endl;
	
	if (_thd->joinable())
		_thd->join();

	cout << _name << " stop ok" << endl;
	return true;
}

bool TaskThread::push(Task* t){
	cout << _name << " push task" << endl;
	if (t == nullptr) return false;

	unique_lock<recursive_mutex> lock(_mt);
	if (_t && _t->is_finished()){
		cout << "pre task still running" << endl;
		return false;
	}

	_t = t;
	_cv.notify_one();
	lock.unlock();

	_t->Task::wait_ready();
	cout << _name << " push " << t->name() << endl;
	return true;
}

void TaskThread::_thread_proc()
{
	cout << _name << " running" << endl;
	
	while (_running)
	{
		//{
		cout << _name << " lock..." << endl;
		this_thread::sleep_for(milliseconds(10));
		unique_lock<recursive_mutex> lock(_mt);
		cout << _name << " locked to wait..." << endl;
		//while (_running && (_t == nullptr || _t->is_finished())) _cv.wait(lock);
		while (_running && (_t == nullptr || _t->is_finished()))
			_cv.wait_for(lock, milliseconds(10));
	
		cout << _name << " wait:" << _t->name() << " st:" << _t->status() << endl;

		if (!_running)
		{
			cout << _name << " break..." << endl;
			break;
		}
		//}

		//while (!_t->is_finished()){
		if (!_t->is_finished()){
			_t->wait(10);
		}
	}
	_exited = true;
	cout << _name << " exit" << endl;
}

}

