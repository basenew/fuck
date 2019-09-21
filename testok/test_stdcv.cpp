#include <iostream> // std::cout
#include <thread> // std::thread
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;
void print_id(int id) {
	if (id%2 != 0) std::this_thread::sleep_for(std::chrono::seconds(5));
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck); //线程将进入休眠
    // ...
    std::cout << "thread " << id << '\n';
}
void go() {
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all();
}
int main()
{
    go(); // go!
    std::thread threads[10];
    // spawn 10 threads:
    for (int i = 0; i<10; ++i)
        threads[i] = std::thread(print_id, i);
    std::cout << "10 threads ready to race...\n";
    for (auto& th : threads) th.join();
    return 0;
}

