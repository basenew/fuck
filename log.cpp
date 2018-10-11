#include "log.h"
#include <ofstream>
#include <mutex>

using namespace std;

namespace comm
{
class Log
{
private:
	ofstream _out;
	bool _safe;	
	long _size;
	long _line;
	string _path;
	string _name;
	ELVL _lvl;
	EOUT _out;
	ESPLIT _split;
	static Log* _log;
	static mutex _mtx;

	Log();
	
public:
	~Log();
	static Log& ins();
};


Log* Log::_log;
mutex Log::_mtx;

Log::Log():
_safe(false),
_size(0),
_line(0),
_lvl(DBG),
_out(STD|FILE),
_split(DAY),
_log(NULL)
{

}

Log::~Log()
{
}

Log& Log::ins()
{
	if (_log == nullptr)
	{
		_mtx.lock();
		if (_log == nullptr)
		{
			_log = new Log();
		}
		_mtx.unlock();
	}
	return *_log;
}

bool config(EOUT out, const string& path, const string& name, bool safe, ESPLIT split)
{
	return true;
}

int print(pcchar* msg, pcchar* file_name, int line, pcchar* func_name, LLVL lvl, OUTER outs)
{
	return size;
}

template<class T> Log& operator<<(const T& v pcchar* file_name = nullptr, int line = -1, pcchar* func_name = nullptr, LLVL lvl = LDBG, OUTER outs = OFILE);
{
	outer << v;
	return *this;
};

}






