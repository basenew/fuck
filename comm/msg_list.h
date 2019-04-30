#pragma once
#include "comm.h"
#include <forward_list>
#include <thread>
#include <mutex>
#include <list>
#include <condition_variable>

using namespace std;

namespace comm
{
class MsgProc
{
public:
	virtual bool operator()(int msg_type, char* data, int len);
};

class Msg
{
public:
	Msg(int t = 0, char* d = nullptr, int l = 0):type(t), data(d), len(l){};
	virtual ~Msg(){if (data) delete[] data;};
	int   type;
	char* data;
	int   len;
};

class MsgList
{
public:
	MsgList();
	virtual ~MsgList();
	virtual int clear();
	virtual int post(int msg_type, char* data, int len);
	virtual int on_msg(int msg_type, char* data, int len);
	virtual int run(MsgProc* msg_proc = nullptr);
	virtual int stop();

private:
	static void thread_proc(MsgList* msg_lst);
	virtual void _thread_proc();

private:
	list<Msg>          _msg_lst;
	thread             _thd;
	mutex              _mtx;
	condition_variable _cv;
	bool               _working;
	MsgProc*           _msg_proc;
};

};


