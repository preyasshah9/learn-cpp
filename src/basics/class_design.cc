/*
This code shows basic methods to consider when designing your own class in C++.
*/

#include <iostream>

class A {

    public:
        // Method 1: Constructor
        A(int a, char b, int c):
            _foo(a), _bar(b), _baz(new int(c)) {
            std::cout << "A() called" << std::endl;
        }

        // Method 2: Destructor, only write if you own memory
        // Another use case if you want to log statements.
        ~A() {
            // delete a raw pointer.
            delete _baz;
            // Not needed if you use RAII classes.
            std::cout << "~A() called" << std::endl;
        }
        // Define virtual destructor for polymorphic base class.
        //virtual ~A() = default;

        // Default copy constructor will do a shallow copy, result
        // in a double delete when an object is out of scope (destructor called)
        A(const A& other):
            _foo(other._foo), _bar(other._bar), _baz(new int(*other._baz)) {
                std::cout << "Copy Constructor Called" << std::endl;
            }

        // Copy Assignment needed for same reason, using RAII / smart pointers will
        // eliminate the need to write these.
        A& operator=(const A& other) {
            std::cout << "Copy Assignment Called" << std::endl;
            if (this == &other)
                return *this;  // self-assignment guard
            delete _baz;  // free existing memory

            _foo = other._foo;
            _bar = other._bar;
            _baz = new int(*other._baz);

            return *this;
        }

        // Move Constructor
        A& operator=(A&& other) noexcept {
            std::cout << "Move Assignment Called" << std::endl;
            if (this == &other)
                return *this;
        
            delete _baz;   // clean up existing resource
        
            _foo = other._foo;
            _bar = other._bar;
            _baz = other._baz;
        
            other._baz = nullptr;
        
            return *this;
        }

        // Move Constructor
        A(A&& other) noexcept: 
            _foo(other._foo),
            _bar(other._bar),
            _baz(other._baz)x
        {
            std::cout << "Move Constructor Called" << std::endl;
            other._baz = nullptr;  // prevent double delete
        }

    // Private Instance Variables.
    private:
        int _foo;
        char _bar;
        // Raw pointer. Use std::unique to RAII manage the allocations.
        int* _baz;
};

int main() {
    std::cout << "Create first Object" << std::endl;
    A a = A(1, 'c', 2);
    std::cout << "Move object" << std::endl;
    A a2 = std::move(a);
    std::cout << "Create Second Object" << std::endl;
    A a3 = A(2, 'b', 3);
    std::cout << "Reassign second object" << std::endl;
    a3 = A(3, 'c', 4);
    return 0;
}