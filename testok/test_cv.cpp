#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

class Condition {
public:
    Condition() : count(0) { }
    virtual ~Condition() { }
 
    void Increase() {
    	boost::mutex::scoped_lock lock(mutex);
    	++count;
    }
 
    void Decrease() {
    	boost::mutex::scoped_lock lock(mutex);
    	--count;
    	cond.notify_all(); //通知所有的线程
    }
 
    void Wait() {
    	boost::mutex::scoped_lock lock(mutex);
    	while (count > 0) {
    		cond.wait(lock);
    	}
    }
 
private:
    boost::condition_variable cond;  //条件变量
    boost::mutex              mutex;
    int32_t    count;

};
