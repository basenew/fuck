#include <iostream>
#include <list>
#include <stdio.h>

using namespace std;

class A
{
public:
	A(const string& name):_name(name){cout << "create " << _name << endl;};
	~A(){cout << "destroy " << _name << endl;};
	A(const A&& a){cout << "copy create " << a._name << endl;}
	void print(){cout << "pring " << _name << endl;}

private:
	string _name;
};

int main(int argc, char** argv)
{
	A* pa;
	list<A> la;
	{
	A a("basenew");
	pa = &a;
	la.push_back(move(a));
	pa->print();
	}
	A& ra = la.front();
	if (pa == &ra)
	{
		cout << "pa == &ra" << endl;
	}
	getchar();

	return 0;
}
