#include <stdio.h>
#include "task_thread.h"
#include "task_flow.h"

using namespace nav;
#if 0
class A
{
public:
	void Test(){cout << "Test in A" << endl;};
};

void test_task_thread(int argc, char** argv)
{
	TaskThread th("test");
	th.start();
	
	//TaskProc t{[](){cout << "test task proc" << endl;}};
	TaskProc t{[](){cout << "test task proc" << endl;}};
	int i = 0, count = 1;
	if (argc > 1)
		count = atoi(argv[1]);

	count = count >= 0 ? count:1;
	while (i++ < count)
		th.push(t);
	//this_thread::sleep_for(chrono::seconds(1));
	
	cout << "please input any key to exit" << endl;
	//getchar();
	th.stop();

	return 0;
}
#endif

void test_task_flow()
{
	cout << "test_task_flow start..." << endl;
	Task ta("A");	
	Task tb("B");	
	Task tc("C");	
	Task td("D");	

	ta.front(&tb);
	ta.front(&tc);
	tb.front(&tc);
	tc.front(&td);

	TaskFlow tf("patrol");
	tf.push(&ta);
	tf.push(&tb);
	tf.push(&tc);
	tf.push(&td);

	tf.start(nullptr);
	cout << "test_task_flow finish" << endl;
}

int main(int argc, char** argv)
{
	test_task_flow();

	return 0;
}
