#include <thread>
#include <future> // 引入future所在的头文件
#include <string>
#include <iostream>

using namespace std;

// 需要从线程函数传递出来的数据
class Food
{
public:
	Food(){} // 默认构造函数
	// 通过菜名构建Food对象
	Food(string strName) : m_strName(strName){}
	// 获取菜名
	string GetName() const
	{
		return m_strName;
	}
private:
	string m_strName; // 菜名
};

#if 0
// 线程函数
// 根据菜名创建Food对象，并通过promise对象返回结果数据
void Cook(const string strName,promise<Food>& prom)
{
	// 做菜…
	Food food(strName);
	// 将创建完成的food对象放到promise传递出去
	prom.set_value(food);
}

int main()
{ 
	// 用于存放结果数据的promise对象
	promise<Food> prom;
	// 获得promise所关联的future对象
	future<Food> fu = prom.get_future();

	// 创建分支线程执行Cook()函数
	// 同时将菜名和用于存放结果数据的promise对象传递给Cook()函数
	// ref()函数用于获取promise对象的引用
	thread t(Cook,"回锅肉",ref(prom));

	// 等待分支线程完成Food对象的创建，一旦完成，立即获取完成的Food对象
	Food food = fu.get();
	// 上菜
	cout<<"客官，你点的"<<food.GetName()<<"来了，请慢用！"<<endl;
	t.join(); // 等待分支线程最终完成

	return 0;
}

#endif
//Food Cook(const string strName)
void Cook(const string strName)
{
  // 做菜…
	Food food(strName);
	  // 直接将结果数据通过返回值返回
	std::this_thread::sleep_for(std::chrono::seconds(10));
	//return food;
}
	 
int main()
{ 
	// 使用线程函数的返回值和参数类型特化packaged_task类模板
	// 利用其构造函数，将线程函数打包成一个packaged_task对象
	packaged_task<Food(string)> cooker(Cook);
	 
	// 从packaged_task对象获得与之关联的future对象
	future<Food> fu = cooker.get_future();
	// 创建线程执行packaged_task对象，实际上执行的是Cook()函数
	// 这里也不再需要传递promise对象
	thread t(move(cooker),"回锅肉");
	 
	  // 同样地获得结果数据
	Food food = fu.get();
	cout<<"客官，你点的"<<food.GetName()<<"来了，请慢用！"<<endl;
	t.join();
	  
	return 0;
} 
