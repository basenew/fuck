#include "server_frame.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

namespace comm
{
	int server_frame::start(int argc, char** argv)
	{
		LOG_SYS << "server start..." << endl;
		if (0 != parse_arg(argc, argv))
			return -1;
		
		if (_status == STATUS_STOPED)
			_thd = thread(thread_static_proc, this);
	}

	int server_frame::thread_proc()
	{
		LOG_SYS << "server start..." << endl;
		on_start();
		
		_status = STATUS_RUNNING;
		LOG_SYS << "server running..." << endl;

		std::chrono::milliseconds sleep_ms(10);
		while (true)
		{
			switch(_status)
			{
			case STATUS_STOPED:
				goto THREAD_STOP;
				break;
			case STATUS_RELOAD:
				break;
			case STATUS_SUSPEND:
				this_thread.sleep_for(sleep_ms);
				break;
			case STATUS_RUNNING:
				_evt_egn.check(10);	
				break;
				
			}
			
		}

THREAD_STOP:

		LOG_SYS << "server stop..." << endl;
		
		on_stop();
		_status = STATUS_STOPED;

		return OK;

	}

	int server_frame::stop()
	{
		LOG_SYS << "call server to stop" << endl;
		_status = STATUS_PRESTOP;
	}

	int server_frame::send_sig(int sig)
	{
	}

	int server_frame::show_help()
	{
	}
	
	int server_frame::parse_arg(int argc, char** argv)
	{
		return 0;
	}
}
