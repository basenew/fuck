#include "../../comm/msg_list.h"
#include <chrono>
#include <thread>

using namespace comm;
using namespace std::chrono;

int main(int argc, char** argv)
{
	MsgList msg_lst;
	msg_lst.run();
	int i = 0;
	for (;i < 10;i++)
		msg_lst.post(i, nullptr, 0);
	int c = getchar();
	msg_lst.stop();
	this_thread::sleep_for(seconds(1));
	return 0;
}


