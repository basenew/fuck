#pragma once
#include <iostream>
#include <atomic>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <assert.h>
#include "nav_error_code.h"

#define FOREVER     (-1)

using namespace std;
using namespace std::chrono;

namespace nav{

struct TaskCB{
	virtual ~TaskCB(){};
	virtual void on_loop()=0;
	virtual void on_running()=0;
	virtual void on_paused()=0;
	virtual void on_finished()=0;
};

class Task{
public:
	enum STATE
	{
		IDL,
		RNG,
		PSD,
		FIN,
		ST_MAX
	};

	enum OP
	{
		STA,
		STP,
		PUS,
		RSM,
		OP_MAX,
	};

	STATE st_trans[ST_MAX][OP_MAX]{
	   //STA  STP  PUS  RSM
		{RNG, IDL, IDL, IDL},//IDL
		{RNG, FIN, PSD, RNG},//RNG
		{RNG, FIN, PSD, RNG},//PSD
		{FIN, FIN, FIN, FIN},//FIN
	};

	Task(const string& name = "")
	:_ready(false), _cb(nullptr), _st(IDL), _name(name), _loop_ms(FOREVER){};

	inline int status(){return _st;};
	inline const string& name(){return _name;};	
	inline void name(const string& name){_name = name;};
	inline int  loop_ms(){return _loop_ms;};	
	inline void loop_ms(int loop_ms){_loop_ms = loop_ms;};

	inline virtual int start() {cout << _name << " start"  << endl; return _next_state(STA);};
	inline virtual int stop()  {cout << _name << " stop"   << endl; return _next_state(STP);};
	inline virtual int pause() {cout << _name << " pause"  << endl; return _next_state(PUS);};
	inline virtual int resume(){cout << _name << " resume" << endl; return _next_state(RSM);};

	inline virtual void on_loop()    {cout << _name << " on_loop" << endl;};
	inline virtual void on_running() {cout << _name << " on_running" << endl;};
	inline virtual void on_paused()  {cout << _name << " on_paused" << endl;};
	inline virtual void on_finished(){cout << _name << " on_finished"  << endl;};
	inline void cb(TaskCB* cb){_cb = cb;};

	inline virtual bool is_idle()    {unique_lock<recursive_mutex> lock(_mt);return _st == IDL;};
	inline virtual bool is_ready()   {unique_lock<recursive_mutex> lock(_mt);return _ready;    };
	inline virtual bool is_paused()  {unique_lock<recursive_mutex> lock(_mt);return _st == PSD;};
	inline virtual bool is_running() {unique_lock<recursive_mutex> lock(_mt);return _st == RNG;};
	inline virtual bool is_finished(){unique_lock<recursive_mutex> lock(_mt);return _st == FIN;};

	inline virtual bool wait_ready   (int ms = FOREVER){
		unique_lock<recursive_mutex> lock(_mt);
		while(!_ready && _st != FIN){
			if (ms == FOREVER)
			  _cv.wait(lock);
			else
			  _cv.wait_for(lock, milliseconds(ms));
		}

		return _ready || _st == FIN;
	};
	inline virtual bool wait_running (int ms = FOREVER){return wait(RNG, ms);};
	inline virtual bool wait_paused  (int ms = FOREVER){return wait(PSD, ms);};
	inline virtual bool wait_finished(int ms = FOREVER){return wait(FIN, ms);};

	void wait(int ms = FOREVER);
protected:
	virtual bool wait(int st, int ms);

private:
	int _next_state(OP op);

protected:	
	bool               _ready;
	int                _st;
	int                _loop_ms;
	string		       _name;
	TaskCB*            _cb;
	recursive_mutex    _mt;
	condition_variable_any _cv;
};

}










