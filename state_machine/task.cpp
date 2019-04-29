#include "task.h"

static int st_tb = 
{
	{ST_WORKING, ST_IDLE, ST_IDLE, ST_IDLE},
	{ST_WORKING, ST_IDLE, ST_WORKING, ST_PAUSED},
	{ST_WORKING, ST_IDLE, ST_WORKING, ST_PAUSED},
};

int get_next_state(int cur_state, int op)
{
	if (cur_state < ST_MAX && op < OP_MAX)
	  return st_tab[cur_state][op];
	LOG_ERR << "cur:%d" << cur_state << " op:" << op << " invalid" << endl;
	return ST_INVALID;
}

Task::Task(const string& name, int tid)
{
}

Task::~Task()
{
}

int Task::init()
{
	return OK;
}


int Task::start()
{
	TSTATE st = get_next_state(_st, OP_START);
	if (st == ST_INVALID)
	  return ERR;
	else if (_st != ST_WORKING)
	  return run(this);

	LOG_MSG << "already working" << endl;
	return OK;
}


int Task::stop()
{
	return OK;
}


int Task::pause()
{
	return OK;
}


int Task::resume()
{
	return OK;
}


bool Task::operator()(int msg_type, char* data, int len)
{
	LOG_MSG << "msg:%d" << msg_type << " len:%d" << len << endl;
}
