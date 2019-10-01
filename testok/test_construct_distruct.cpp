#include <iostream>

using namespace std;

struct A
{
	virtual void f0() { std::cout << "A\n"; }

	A() { this->f0(); }

	virtual ~A() { std::cout << "~A" << endl; this->f0(); }
};

struct B : public A
{
	virtual void f0() { std::cout << "B\n"; }

	B() { this->f0(); }

	~B() override { this->f0(); }
};

int main()
{
	for (int i = 0;i < 10;i++)
	  A a;
	//A* b = new B;
	//delete b;
	return 0;
}
