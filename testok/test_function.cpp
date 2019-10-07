#include <iostream>
#include <functional>

using namespace std;

int add(int a, int b)
{
	cout << "add(" << a << "," << b << ")=" << a+b << endl;
	return a+b;
}

class A
{
public:
	A(const string& name):_name(name){}
	int operator()(int a, int b){cout << _name << endl; return a+b;}
private:
	string _name;
};

class B
{
public:
	B(A& a):_a(a){}
	void call(){cout << _a(100, 200) << endl;}
private:
	A _a;
};

int main(int argc, char** argv)
{
	function<int (int, int)> f = add;
	cout << f(1, 2) << endl;
	A a("funca");
	cout << a(5, 6) << endl;
	f = a;
	cout << f(3,4) << endl;
	B b(a);
	b.call();
	function<int ()> f1 = bind(add, 1, 2);
	f1();
	return 0;
}
