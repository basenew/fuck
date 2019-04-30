#include "task.h"

namespace state_machine
{

static int st_tb[3][4] ={{ST_WORKING, ST_IDLE, ST_IDLE,	  ST_IDLE},	{ST_WORKING, ST_IDLE, ST_WORKING, ST_PAUSED},{ST_WORKING, ST_IDLE, ST_WORKING, ST_PAUSED}};

int get_next_state(int cur_state, int op)
{
	int st = ST_INVALID;
	if (cur_state < ST_MAX && op < OP_MAX)
	  st = st_tb[cur_state][op];
	LOG_MSG << "cur:%d" << cur_state << " next:%d" << st << " op:" << op << endl;
	return st;
}

Task::Task(const string& name)
:_name(name)
{
}

Task::~Task()
{
}

int Task::init()
{
	return OK;
}

int Task::_op(int op)
{
	int st = get_next_state(_st, op);
	if (st == _st)
	  return OK;

	switch (st)
	{
		case ST_WORKING:
		{
			run(this);
			break;
		}
		case ST_PAUSED:
		{
			break;
		}
		case ST_IDLE:
		{
			MsgList::stop();
			break;
		}
		default:
		{
			LOG_ERR << "invalid op at st:" << _st << endl;
			return ERR;
		}
	}

  _st = st;
  return OK;
}

int Task::start()
{
	return _op(OP_START);
}


int Task::stop()
{
	return _op(OP_STOP);
}


int Task::pause()
{
	return _op(OP_PAUSE);
}


int Task::resume()
{
	return _op(OP_RESUME);
}


bool Task::operator()(int msg_type, char* data, int len)
{
	LOG_MSG << "msg:%d" << msg_type << " len:%d" << len << endl;
}

}
