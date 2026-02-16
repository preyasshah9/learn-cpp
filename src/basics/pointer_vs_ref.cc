/*
This code shows the differences in the mechanism of pointers v/s references.
*/
#include <iostream>
#include <memory>

void func_using_ptr(int* x) {
    std::cout << "2x: " << *x * 2 << std::endl;
}

void pointer_example() {
    int x = 10;
    int y = 15;
    
    int* ptr = &x; // Stack
    std::cout << *ptr << std::endl;
    // Reassignment
    ptr = &y;
    std::cout << *ptr << std::endl;
    
    int* ptr2 = nullptr; // Pointers can be assigned to null.
    ptr2 = new int(8);  // On Heap
    std::cout << *ptr2 << std::endl;
    delete ptr2; // If not deleted, will be memory leak  
    
    // Passing pointer to function.
    func_using_ptr(ptr); // or func_using_ptr(&x);
    
    std::unique_ptr<int> ptr3 = std::make_unique<int>(6); // RAII, no manual deletion needed.
}

// Const T& is a better way of passing an object to func to avoid copy.
// for primitive types/smaller objects like int, passing by value is preferrable.
void func(const int& x) {
    std::cout << "2x: " << x * 2 << std::endl;
}

void reference_example() {
    int x = 5;
    int y = 6;
    int& b = x;
    
    // Reference still binds to x, value of x is now 6.
    b = y; 
    std::cout << "x: " << x << std::endl;
    
    //int& c = 6; Not possible, non-const references cannot be rvalue
    const int& c = 7; // This is valid
    
    //int& d; // References cannot be null.
    func(b); // Reference syntax is cleaner than pointer.
}

int main() {
    pointer_example();
    reference_example();
}