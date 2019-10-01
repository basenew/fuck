#include "task.h"

namespace nav{

void Task::wait(int ms){
	cout << _name << " loop..." << endl;
	bool is_started = true;
	{
		unique_lock<mutex> lock(_mt);
		if (_st == IDL)
		{
			_st = WTG;
			is_started = false;
		}
		_cv.notify_one();
	}

	int loop_ms = (ms == FOREVER) ? _loop_ms:ms;
	while (_st != FIN){
		unique_lock<mutex> lock(_mt);
		if (_st == FIN){
			cout << _name << " already finished" << endl;
			return;
		}

		cout << _name << " wait status to change..." << endl;
		while (_st == WTG){
			if (is_started)
			{
				if (_cb)
					_cb->on_loop();
				else 
					on_loop();
			}

			if (loop_ms == FOREVER)
				_cv.wait(lock);
			else{
				_cv.wait_for(lock, milliseconds(loop_ms));
				if (_st == WTG)
				  return;
			}
		}
		cout << _name << " status changed" << endl;
		//getchar();
		switch(_st)
		{
		case RNG:
			if (_cb)
				_cb->on_running();
			else
				on_running();
			break;
		case PSD:
			if (_cb)
	    		_cb->on_paused();
			else
				on_paused();
			break;
		case FIN:
			if (_cb)
				_cb->on_finished();
			else
				on_finished();
			return;
		default:
			cout << _name << " error invalid status" << endl;
			assert(false);
			break;
		}
		_st = WTG;
	}
}

bool Task::wait(int st, int ms){
	//cout << _name << " _st:" << _st << " wait st: " << st << " ms:" << ms << endl;
	unique_lock<mutex> lock(_mt);
	if (st == _st) return true;

	if (ms == FOREVER){
		while (st != _st)_cv.wait(lock);
	}
	else{
		_cv.wait_for(lock, ms*milliseconds(1));
	}
	//cout << _name << " ret:" << bool(st==_st) << endl;
	return st == _st;
}

int Task::_next_state(OP op)
{
	unique_lock<mutex> lock(_mt);
	int st = st_trans[_st][op];
	cout << _name << " cur st:" << _st << " next st:" << st << " op:" << op<< endl;
	if (st != _st)
	{
		_st = st;
		_cv.notify_all();
		return ERR_OK;
	}

	return ERR_FAIL;
}

}
