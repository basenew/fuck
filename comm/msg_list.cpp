#include "msg_list.h"
#include <string.h>
#include <exception>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

namespace comm
{

MsgList::MsgList()
:_working(false)
,_msg_proc(nullptr)
{
	LOG_MSG << "MsgList" << endl;
}

MsgList::~MsgList()
{
	LOG_MSG << "~MsgList" << endl;
	clear();
}

int MsgList::post(int msg_type, char* data, int len)
{
	LOG_MSG << "post msg:" << msg_type << " len:" << len << endl;
	if (!_working)
	{
		LOG_MSG << "msg list is not running" << endl;
		return ERR;
	}

	unique_lock<mutex> lck(_mtx);
	Msg msg;
	msg.type = msg_type;
	if (data != nullptr && len > 0)
	{
		msg.data = new char[len];
		memcpy(msg.data, data, len);
		msg.len = len;
	}
	_msg_lst.push_back(msg);
	_cv.notify_all();
	return OK;
}

int MsgList::on_msg(int msg_type, char* data, int len)
{
	LOG_MSG << "on_msg:" << msg_type << " len:" << len << endl;
	return OK;
}

int MsgList::clear()
{
	LOG_MSG << "clear" << endl;
	unique_lock<mutex> lck(_mtx);
	for (Msg& msg:_msg_lst)
	{
		if (msg.data)
		  delete[] msg.data;
	}
	_msg_lst.clear();
	return OK;
}

void MsgList::thread_proc(MsgList* msg_lst)
{
	LOG_MSG << "thread_proc" << endl;
	msg_lst->_thread_proc();
}

void MsgList::_thread_proc()
{
	LOG_MSG << "_thread_proc" << endl;
	while (_working)
	{
		unique_lock<mutex> lck(_mtx);
		if (_msg_lst.empty()) 
		{
			if (cv_status::timeout == _cv.wait_for(lck, milliseconds(100)))
				continue;
		}
		//while (_msg_lst.empty()) _cv.wait_for(lck, 100ms);
		LOG_MSG << "wait ok" << endl;
		Msg& msg = _msg_lst.front();
		if (_msg_proc)
			(*_msg_proc)(msg.type, msg.data, msg.len);
		else
			on_msg(msg.type, msg.data, msg.len);
		_msg_lst.pop_front();
	}
	LOG_MSG << "_thread_proc exit" << endl;
}

int MsgList::run(MsgProc* msg_proc)
{
	LOG_MSG << "run" << endl;
	_msg_proc = msg_proc;
	_working = true;
	try
	{
		_thd = thread(thread_proc, this);
		_thd.detach();
	}
	catch (exception e)
	{
		LOG_ERR << e.what() << endl;
		return ERR;
	}
	return OK;
}

int MsgList::stop()
{
	LOG_MSG << "stop" << endl;
	_working = false;
	_thd.join();
	return OK;
}


}
