#include "sys.h"

#include <unistd.h>
#include <string.h>
#include <errno.h>

namespace comm
{
#define LOG cout

string get_errno_str()
{
	return string(strerror(errno));
}

void print_error_str()
{
	perror(get_errno_str().c_str());
}

string get_proc_name(int pid)
{
	cout << "get_proc_name" << endl;
	char name[256];	
	int ret = readlink("/proc/self/exe", name, sizeof(name));
	if (ret <= 0)
	{
		LOG << "readlink err:" << get_errno_str() << endl;	
	}
	cout << "proc:" << name << endl;
	return string(name);
}

string get_time_string(const string& cat_str)
{
	char date_time[240]; 
 
	time_t now;
	tm* tm_now;
 
	time(&now);
	tm_now = localtime(&now);
	string format("%Y%m%d");
	format += cat_str;
	format += "%H%M%S";
	strftime(date_time, sizeof(date_time), format.c_str(), tm_now); 
//	strftime(date_time, sizeof(date_time), "%I:%M%p", tm_now); 
	return date_time;	
} 
}

