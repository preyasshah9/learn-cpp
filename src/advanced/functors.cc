/*
This code shows the example with functors and lambdas in C++.

What is a functor? It is basically a class that overrides an operator ().
Example

class Foo {
    public:
        void operator()(int bar) {
            std::cout << "Bar: " << bar << std::endl;
        }
};

Struct usage (Since members are public by default)
struct Foo {
    int operator()(int bar) {
        return bar + 1;
    }
};

// Lambdas basically use a functor and is a syntactic sugar around it.
Lambda in the above use case:
auto func = [](int bar) {
    return bar + 1;
};

Lambdas can capture variables from their surrounding scope:

- Capture-by-value copies the variable into the lambda object.
- Capture-by-reference stores a reference to the original variable.

Example:

auto func = [foo, &baz](int bar) {
    return (bar + foo) * baz;
};

Conceptually, the compiler generates something similar to:

class __lambda {
    int foo;
    int& baz;
public:
    __lambda(int f, int& b) : foo(f), baz(b) {}

    int operator()(int bar) const {
        return (bar + foo) * baz;
    }
};

Important rule:
Never capture by reference if the lambda may outlive the referenced variable,
otherwise a dangling reference may cause undefined behavior.

Let's put these learnings by example.
*/
# include <iostream>
using namespace std;

int main() {
    int foo = 5, bar = 10;
    auto add5 = [](int x) { return x + 5; };
    std::cout << "add5(15): " << add5(15) << std::endl; // prints 20.

    // Capture variable by Value.
    auto addBar = [bar](int x) { return x + bar; };
    std::cout << "addBar(20): " << addBar(20) << std::endl; // prints 30.

    // Captured-by-value variables are const inside the lambda unless the lambda is marked mutable
    auto addBarMutable = [bar](int x) mutable {
        bar = 15;
        return x + bar;
    };
    std::cout << "addBarMutable(20): " << addBarMutable(20) << std::endl; // prints 35.
    
    // Capture variable by reference.
    auto addFooRef = [&foo](int x) {
        // Usually this causes confusion and depending upon lambda invocation, foo will change. 
        // Just for demo we want to prove that outer variable gets reflected.
        foo = 15;
        return x + foo;
    };
    std::cout << "addFooRef(20): " << addFooRef(20) << std::endl; // prints 35.
    std::cout << "Modified Foo: " << foo << std::endl; // prints 15 now.
}
