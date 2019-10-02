#include <stdarg.h>
#include <string.h>
#include <iostream>

using namespace std;

void mtrace(const char *chfr, ...) {
    char buffer[4096] = {0};
    va_list ap;
    va_start(ap, chfr);
    vsnprintf(buffer, sizeof(buffer) - 1, chfr, ap);
    va_end(ap);
}

void test(string& str)
{
	cout << "test..." << endl;
	char *pstr = "123456";
	str.append(pstr, 0);
	mtrace("%s", str.c_str()); 
	cout << "test ok" << endl;
}

int main()
{
	string str;
	test(str);
	return 0;
}
