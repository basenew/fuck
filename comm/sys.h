#ifndef __COMM_SYS_H__
#define __COMM_SYS_H__

#include "typedef.h"
#include <iostream>

using namespace std;

namespace comm
{
	string get_errno_str();
	void print_error_str();
	string get_proc_name(int pid = -1);
	string get_time_string(const string& cat_str = string(" "));
}

#endif
