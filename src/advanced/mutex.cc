// This example covers the mutexes and conditional variables in the
// multithreading applications.

#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

std::mutex mtx;
std::condition_variable cv;
int shared_ctr = 0;
bool ready = false;

void worker() {
    {
        std::lock_guard<std::mutex> lock(mtx); 
        // 1. Do all the work while holding the lock
        for (int i = 0; i < 5; i++) {
            ++shared_ctr;
            std::cout << "Shared Ptr: " << shared_ctr << std::endl;
        }
        // 2. Set the flag while still holding the lock
        ready = true;
    }
    cv.notify_one(); 
}

int main() {
    std::thread th(worker);

    // 3. Main thread wait logic
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return ready; });

        // Shared counter will only be accessed after worker thread.
        std::cout << "Main: shared_ctr = " << shared_ctr << std::endl;
        shared_ctr *= 2;
        std::cout << "Value of shared_ctr after mul by 2: " << shared_ctr << std::endl;
    } // lock released here

    th.join();
    std::cout << "Successfully joined thread." << std::endl;
    return 0;
}
