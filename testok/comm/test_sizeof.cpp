#include <iostream>

using namespace std;

class A
{
virtual string name(){cout << "A" << endl;};
};

int main()
{
	A a;
	cout << "sizeof A:" << sizeof(A) << endl;
	return 0;
}
