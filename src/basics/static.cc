/*
This code shows the static keyword usage.
*/ 

#include <iostream>

void foo() {
    static int count = 0;
    count++;
    std::cout << "Static Count: " << count << std::endl;
}

class Bar {
    public: 
        static int count; // Declaration.
};

int Bar::count = 3; // Initialization.

int main() {
    foo();
    foo();

    // Bar::count is not associated with the Bar.
    Bar::count = 4;
    std::cout << "Bar Count: " << Bar::count << std::endl;

    Bar::count = 6;
    std::cout << "Bar Count: " << Bar::count << std::endl;
}

// Output: 1,2