#include "vector_list.h"

#include <iostream>

using namespace comm;
using namespace std;

class bn
{
public:
	bn(){cout << "bn" << endl;}
	~bn(){cout << "~bn" << endl;}
private:
	int _v;
};

int main(int argc, char** argv)
{
	int size = 10;
	if (argc > 1)
		size = atoi(argv[1]);
	vector_list<bn> lv;
	lv.init(size);

	for (int i = 0; i < size + 5; i++)
	{
		bn& t = lv.pop();
		if (!lv.isvalid(t))
		{
			cout <<"invalid:" << i << endl;
		}
		lv.push(t);
	}
	
	cout << "used:" << lv.used_size() << " free:" << lv.free_size() << endl;
	bn& t = lv.pop();
	
	return 0;
}
