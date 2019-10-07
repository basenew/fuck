#include "../../comm/comm.h"
#include <dlfcn.h>

using namespace comm;
//typedef string (*test_dl)(const string& str);
typedef void (*test_dl)();

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "please give lib name to open" << endl;
		return -1;
	}

	void* hdl = dlopen(argv[1], RTLD_LAZY);
	if (hdl == nullptr)
	{
	  cout << "open " << argv[1] << " err:" << dlerror() << endl;
	  return -1;
	}

	cout << "open " << argv[1] << " ok" << endl;

	if (argc >= 3)
	{
		void* func = dlsym(hdl, argv[2]);
		if (func == nullptr)
		{
			cout << "get func:" << argv[2] << " err:" << dlerror() << endl;
			return -1;
		}
		else
		{

			//string str = ((test_dl)func)("yangzou");
			((test_dl)func)();
			//cout << "return:" << str << endl;
		}
	}

	return 0;
}
