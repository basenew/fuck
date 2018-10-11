#include "typedef.h"
#include "log.h"
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
using namespace comm;

int main(int argc, char* argv[])
{
	LOG_DBG << "test log" << endl;
	//cout << pthread_self() << endl;//" tid:" << gettid() << endl;
	//printf("tid:%lu", pthread_self());
	//Log::ins() << "test log" << endl;
	return 0;
}
