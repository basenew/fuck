#include "task_flow.h"

namespace nav{

bool TaskFlow::start(function<void()> on_finish){
	cout << _name << " start..." << endl;
	if (StateMachine::start()){
		for (Task* t:_tasks){
			if (t->is_ready()){
				t->start();
				_actives.push_back(t);

				TaskThread* thd = new TaskThread();
				_thds.push_back(thd);
				thd->push(t);
				thd->start();
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
}

void TaskFlow::_wait(){
	bool finished = false;
	while (!finished){
		finished = true;
		list<TaskThread*> thds = _thds;
		for (TaskThread* thd:thds){
			if (thd->is_exited()){
				cout << "already stoped" << endl;
				continue;
			}

			Task* t = dynamic_cast<Task*>(thd->task());
			assert(t);
			t->wait();
			if (t->is_last()){
				cout << t->name() << " is last" << endl;
				thd->stop();
				continue;
			}

			bool only_one = true;
			list<Task*> behinds = t->behinds();	
			for (auto bt:behinds){
				t->rm_behind(bt);
				bt->rm_front(t);
				cout << bt->name() << " state:" << bt->state() << endl;
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
}


}

