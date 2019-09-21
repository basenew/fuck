#include <mutex>
#include <thread>
#include <iostream>
#include <condition_variable>

class Notifier
{
public:
	Notifier()=default;
	~Notifier(){};

	void notify();
	void notify_all();
	void wait();
	void wait_all();
};

int main(int argc, char** argv)
{
	return 0;
}

