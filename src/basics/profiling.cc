/*
This code shows how we can profile C++ code more accurately using std::chrono library.
This we might use throughout the tutorial series to profile the code to demonstrate speedup as required.
*/ 

#include <chrono>
#include <iostream>

void profileTotalN(int n) {
    int total = 0;
    auto start = std::chrono::high_resolution_clock::now(); // Before profiling start.
    for(int i = 0; i < n; i++) {
        total += i;
    }
    auto end = std::chrono::high_resolution_clock::now();  // After profiling end.
    std::chrono::duration<double, std::micro> duration = end - start;
    std::cout << "Total of 1000 element took " << duration.count() << " microseconds." << std::endl;
}
int main() {
    profileTotalN(10000);
    profileTotalN(1000);
    profileTotalN(100000);
    return 0;
}

/*
Output:
Total of 1000 elements took 40 microseconds.
Total of 10000 elements took 45 microseconds.
Total of 100000 elements took 1070 microseconds.

You could play around flipping the operations around and view profiling results.
*/