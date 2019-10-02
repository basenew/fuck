#include <thread>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

int* p = nullptr;

void thread1()
{
	int i = 0;
	while(1){cout << "1111111" << i++ << endl;this_thread::sleep_for(milliseconds(50));}
}

void thread2()
{
	int i = 0;
	while(1){cout << "2222222" << i++ << endl;this_thread::sleep_for(milliseconds(50));}
}

void thread3()
{
	int i = 0;
	int j = 0;
	while(1){if (i >= 100) *p = i; cout << "33333333" << i++ << endl;this_thread::sleep_for(milliseconds(50));}
}

void test_core_for_mutil_thread()
{
	thread t1(thread1);
	thread t2(thread2);
	thread t3(thread3);

	t1.join();
	t2.join();
	t3.join();
}

int main()
{
	int i = 0;
	int j = 0;
	//while(1){if (i >= 100) *p = i; cout << "33333333" << i++ << endl;this_thread::sleep_for(milliseconds(50));}
	test_core_for_mutil_thread();
	return 0;
}
