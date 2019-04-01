#include "server_frame.h"

namespace comm
{

	int server_frame::start(int argc, char** argv)
	{
		if (0 != parse_arg(argc, argv))
			return -1;
		
		LOG_SYS << "server start..." << endl;
		on_start();
		LOG_SYS << "server running..." << endl;
		while (1)
		{
			_evt_egn.check(10);	
			switch(_status)
			{
			case STATUS_PRESTOP:
				break;
			case STATUS_RELOAD:
				break;
			case STATUS_SUSPEND:
				break;
			case STATUS_CONTINUE:
				break;
				
			}
			
		}

		LOG_SYS << "server stop..." << endl;
		
		on_stop();
		return 0;
	}

	void server_frame::stop()
	{
		LOG_SYS << "call server to stop" << endl;
		_status = STATUS_PRESTOP;
	}

	void server_frame::send_sig(int sig)
	{
	}
	void server_frame::show_help()
	{
	}
	
	int server_frame::parse_arg(int argc, char** argv)
	{
		return 0;
	}
}
