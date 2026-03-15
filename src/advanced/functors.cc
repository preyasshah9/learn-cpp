/*
This code shows the example with functors (aka Function Pointers) and
lambdas in C++.

What is a functor? It is basically a class that overrides an operator ().
Example

class Foo {
    public:
        operator()(int bar) {
            std::cout << "Bar: " << bar << std::endl;
        }
};

Struct usage (Since members are public by default)
struct Foo {
    int operator()(int bar) {
        return bar + 1;
    }
}

// Lambdas basically use a functor and is a syntactic sugar around it.
Lambda in the above use case:
auto func = [](int bar) {
    return bar + 1;
}

Lambdas allow you to capture variables by reference or value. A couple of points
to remember.
- Never Capture by reference if lambda is intended to outlive the calling function (threading) to
avoid program crash.
- Capture by value will invoke copy constructor, which is still okay for smaller objects.

auto func = [foo, &baz](int bar) { // foo captured by value, and baz captured by reference.
    return (bar + foo) * baz;
}

Internally, compiler will create a functor class with foo, baz as member variables and 
overload operator (), passing bar as argument.

Let's put this by example.
*/
using namespace std;

int main() {
    int foo = 5, bar = 10;
    auto add5 = [](int x) {
        return x + 5;
    };
    std::cout << "add5(15): " << add5(15) << std::endl; // prints 20.

    // Capture variable by Value.
    auto addBar = [bar](int x) {
        return x + bar;
    }
    std::cout << "addBar(20): " << addBar(20) << std::endl; // prints 30.

    // Capture variable by reference.
    auto addFooRef = [&foo](int x) {
        // Usually this causes confusion and depending upon lambda invocation, foo will change. 
        // Just for demo we want to prove that outer variable gets reflected.
        foo = 15;
        return x + foo;
    }
    std::cout << "addFooRef(20): " << addFooRef(20) << std::endl; // prints 35.
    std::cout << "Modified Foo: " << foo << std::endl; // prints 15 now.
}
