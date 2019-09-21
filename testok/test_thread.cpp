#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;


void f1(int n)
{
	for (int i = 0; i < 5; ++i) {
		std::cout << "Thread 1 executing\n";
		++n;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void f2(int& n)
{
	for (int i = 0; i < 5; ++i) {
		std::cout << "Thread 2 executing\n";
		++n;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

class A
{
public:
	static void func1(const string& name){cout << "hello:" << name << endl;};
};

int main()
{
	A a;
	int n = 0;
	std::thread t1; // t1 非线程
	std::thread t2(f1, n + 1); // 按值传递
	std::thread t3(f2, std::ref(n)); // 按引用传递
	std::thread t4(std::move(t3)); // t4 现在运行 f2() 。 t3 不再是线程
	std::thread t5(A::func1, "basenew");
	std::this_thread::sleep_for(std::chrono::seconds(10));
	t2.join();
	t4.join();
	t5.join();
	std::cout << "Final value of n is " << n << '\n';
}
