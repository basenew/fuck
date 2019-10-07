#include <stdio.h>
#include "task.h"
#include "task_node.h"
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

void on_finish()
{
	cout << "xxxxxxxxxxxxxxxxfinishedxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
}

void test_task_flow()
{
	TaskFlow tf("patrol");
	cout << "test_task_flow start..." << endl;
	TaskNode* ta = tf.push("A");	
	TaskNode* tb = tf.push("B");	
	TaskNode* tc = tf.push("C");	
	TaskNode* td = tf.push("D");	

	ta->front(td);
	tb->front(td);
	tc->front(td);

	//tf.push(&ta);
	//tf.push(&tb);
	//tf.push(&tc);
	//tf.push(&td);

	//tf.run(on_finish);
	tf.run();
	cout << "------------input any key to stop task flow-----------------------" << endl;
	getchar();
	cout << "test_task_flow finish" << endl;
}

int main(int argc, char** argv)
{
	test_task_flow();
	//thread t(test_task_flow, std::ref(tf));
	//while (tf.running_count()){
	//	cout << "to stop task" << endl;
//		getchar();
	//}

	return 0;
}

