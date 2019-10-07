#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char** argv)
{
	/*
	int *p;
	*p = 100;

	char* str = new char[100];
	cout << "str:" << str << endl;

	delete str;
	cout << "afger delete str:" << str << endl;

	delete str;
*/
	char* str2 = new char[100];
	strcpy(str2, "hello yangzou");

	return 0;
}
