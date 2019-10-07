#include <iostream>

using namespace std;

class A
{
public:
	virtual void info()
	{
		cout << "this:" << this
			 << " v1:" << &v1
			 << " v2:" << &v2
			 << " v3:" << &vc3
			 << " v4:" << &vs4 << endl;
	};
protected:
	int v1;
	int v2;
	int vc3;
	string vs4;
};

class B:public A
{
public:
	virtual void info()
	{
		cout << "this:" << this
			 << " v1:" << &v1
			 << " v2:" << &v2
			 << " v3:" << &vc3
			 << " v4:" << &vs4
			 << " v5:" << &vp5 << endl;
	};

private:
	int* vp5;
};


int main(int argc, char** argv)
{
	A a;
	B b;
	a.info();
	b.info();
	return 0;
}
