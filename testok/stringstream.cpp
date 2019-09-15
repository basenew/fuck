#include <sstream>
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

int main(int argc, char** argv)
{
	double d1{7890.1234567890};
	int prec=numeric_limits<double>::digits10;
	stringstream ss, ss2;
	ss.precision(prec);
	ss << d1;
	cout << "d1:" << d1 << "|d1:" << ss.str() << endl;
	ss2 << setprecision(20) << d1;
	cout << ss2.str() << endl;
	return 0;
}
