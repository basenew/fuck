#include <iostream>

using namespace std;

class A
{
public:
	int pub;
	static int s1 = 100;
protected:
	int ptd;
private:
	int pri;
};
//int A::s1;
int g1;
int g2 = 100;
static int gs1;
static int gs2 = 200;

class B:protected A
{
public:
	void visit(){cout << "pub:" << pub << " ptd:" << ptd << /*" pri:" << pri*/ " s1:" << A::s1 <<endl;ptd=100;};
};

int main()
{
	static int s1;
	static int s2 = 300;
	B b;
	b.visit();
	return 0;
}
