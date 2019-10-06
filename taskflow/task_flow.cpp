#include "task_flow.h"

namespace nav{

bool TaskFlow::run(function<void()> on_finish){
	cout << _name << " start" << endl;
	if (ERR_OK == Task::start()){
		int i = 0;
		for (TaskNode* t:_tasks){
			if (t->is_ready()){
				_actives.push_back(t);

				stringstream ss;
				ss << i++;
				TaskThread* thd = new TaskThread(ss.str());
				_thds.push_back(thd);
				thd->start();
				thd->push(t);
				//t->start();
			}
		}

		if (on_finish){
			_on_finish = on_finish;
			if (_thd){
				delete _thd;
				_thd = nullptr;
			}

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
	cout << _name << " wait..." << endl;
	while (!is_finished() && !finished){
		finished = true;
		bool force_stop = false;
		list<TaskThread*> thds = _thds;
		for (TaskThread* thd:thds){
			if (thd->is_exited()){
				cout << _name << "already stoped" << endl;
				continue;
			}

			TaskNode* t = (TaskNode*)thd->task();
			assert(t);
			if (t->is_ready()){
				cout << _name << " " << t->name() << " is ready" << endl;
				finished = false;
				t->start();
				continue;
			}
			else if (!t->wait_finished(10)){
				//cout << _name << " " << t->name() << " not finished" << endl;
				finished = false;
				continue;
			}

			if (is_finished()){
				cout << _name << " stop by user" << endl;
				thd->stop();
				continue;
			}
			else if (t->is_finished() && t->is_last()){
				cout << t->name() << " is finished and is last" << endl;
				thd->stop();
				continue;
			}

			if (t->is_stop_flow()){
				cout << t->name() << " force stop xxxxxxxxxxxxxx" << endl;
				force_stop = true;
				//thd->stop();
				break;
			}

			bool only_one = true;
			list<TaskNode*> behinds = t->behinds();	
			for (auto bt:behinds){
				t->rm_behind(bt);
				bt->rm_front(t);
				cout << bt->name() << " state:" << bt->status() << endl;
				if (bt->is_ready()){
					cout << bt->name() << " is ready" << endl;
					if (only_one){
						only_one = false;
						thd->push(bt);
					}
					else{
						TaskThread* thd = new TaskThread();
						_thds.push_back(thd);
						thd->start();
						thd->push(bt);
					}
					
					int ret = bt->start();
					if (ret != ERR_OK){
						cout << bt->name() << " start error:" << ret << endl;
					}

					finished = false;
				}
				else
					cout << bt->name() << " is not ready" << endl;
			}
		}

		if (force_stop){
			cout << _name << " force stop all threadxxxxxxxxx" << endl;
			for (TaskThread* thd:thds){
				if (!thd->is_exited()){
					cout << _name << " force stop thread:"<< thd->name() << " xxxxxxxxx" << endl;
					//Task* t = thd->task();
					//t->stop();
					thd->stop();
				}
			}
		}
	}

	if (_on_finish)
	  _on_finish();

	cout << _name << " finish" << endl;
}


}

