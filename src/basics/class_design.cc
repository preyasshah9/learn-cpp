/*
This code shows basic methods to consider when designing your own class in C++.
Typically, a class owns resource (Defined by pointer member variables). The RAII design
paradigm suggests using `unique_ptr` or `shared_ptr` to automatically manage the memory and 
prevent issues such as memory leaks.
In this example though, we are assuming a resource management is responsibilities of developer designing the 
class itself.
We will touch these methods:
- Constructor (Initializing an object)
- Destructor (Destroying object, without RAII, need to delete pointer by itself)
- Copy Constructor (Support Deep Copy)
- Copy Assignment (Support Deep Copy)
- Move Constructor
- Move Assignment
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

        // Method 3: Copy Constructor
        // Default copy constructor will do a shallow copy, result
        // in a double delete when an object is out of scope (destructor called)
        A(const A& other):
            _foo(other._foo), _bar(other._bar), _baz(new int(*other._baz)) {
                std::cout << "Copy Constructor Called" << std::endl;
            }

        // Method 4: Copy Assignment
        A& operator=(const A& other) {
            std::cout << "Copy Assignment Called" << std::endl;
            if (this == &other)
                return *this;  // self-assignment guard
            // copy and swap idiom for exception safety.
            A temp(other);        // copy first (may throw)
            std::swap(_foo, temp._foo);
            std::swap(_bar, temp._bar);
            std::swap(_baz, temp._baz);

            return *this;
        }

        // Method 5: Move Constructor
        A(A&& other) noexcept: 
            _foo(other._foo),
            _bar(other._bar),
            _baz(other._baz)
        {
            std::cout << "Move Constructor Called" << std::endl;
            other._baz = nullptr;  // prevent double delete
        }

        // Method 6: Move Assignment
        A& operator=(A&& other) noexcept {
            std::cout << "Move Assignment Called" << std::endl;
            if (this == &other)
                return *this;
        
            delete _baz;   // clean up existing resource
        
            _foo = other._foo;
            _bar = other._bar;
            _baz = std::exchange(other._baz, nullptr); 
            return *this;
        }

    // Private Instance Variables.
    private:
        int _foo;
        char _bar;
        // Raw pointer. Use std::unique to RAII manage the allocations.
        int* _baz;
};

// RAII Design
class B {
    B(int _foo, char _bar, int _baz): 
        _foo(_foo), 
        _bar(_bar), 
        _baz(std::make_unique<int>(_baz))
    {}
    // No need for destructor and destroying memory manually.

    // Copy Operations are deleted. If you want to support deep copy, you need to implement them like
    // shown below.

    // Copy Constructor
    B(const B& other) :
        _foo(other._foo), 
        _bar(other._bar), 
        _baz(other._baz ? 
            std::make_unique<int>(*other._baz)  // Creates a new ownership.
            : nullptr) {}

    // Copy Assignment
    B& operator=(const B& other) {
        _foo = other._foo;
        _bar = other._bar;
        _baz = other._baz ? std::make_unique<int>(*other._baz) : nullptr;
        return *this;
    }

    // Move Operations are defined default.
    private:
        int _foo;
        char _bar;
        // Unique pointer.
        std::unique_ptr<int> _baz;
};

int main() {
    A a(1, 'c', 2); // Constructor
    A a2 = a; // Copy Constructor
    a2 = a; // Copy Assignment
    A a3 = A(3, 'c', 4); // Move Constructor
    a3 = A(3, 'c', 4); // Move Assignment
    return 0;
} // Destructors will be called when objects go out of scope.
