/*
This code shows the example with reference qualifiers & and && in C++.
- Reference qualifier are for non-static members of a class.
- Avoids doing additional copies and improve performance
There are 4 types of reference qualifications.
- lvalue ref qualification &
- rvalue ref qualification &&
- lvalue const ref qualification ( const &)
- rvalue const ref qualification ( const &&) rarely used.
This affects the overload resolution. If rvalue is passed, the overload resolution helps
preventing extra copy.
*/
#include <iostream>
class X {
    public:
        X(int _foo, char _bar): _foo(_foo), _bar(_bar) {}
        X& get() & {
            std::cout << "Calling lvalue get()" << std::endl;
            return *this;
        }
        X&& get() && {
            std::cout << "Calling rvalue get()" << std::endl;
            return std::move(*this);
        }
    private:
        int _foo;
        char _bar;
};

class Builder {
    public:
        Builder(int x): _foo(x) {}
        Builder& setX(int x) & = delete;
        Builder&& setX(int x) && {
            std::cout << "Calling rvalue setX()" << std::endl;
            _foo = x;
            return std::move(*this);
        }
        Builder& increment() & {
            ++_foo;
            return *this;
        }
        // We don't want temporaries to increment as it doesn't make sense.
        Builder&& increment() && = delete;
        int getX() {
            return _foo;
        }
    private:
        int _foo;
};

int main() {
    X x1 = X(1, 'b');
    x1.get();
    // Without rvalue qualified overload, this will result in invoking copy
    // constructor, instead of move. This results in performance penalty.
    X x2 = X(1, 'c').get();
    // Methold chaining will call rvalue qualified method, this is an example of
    // fluent Builder pattern.
    Builder(12).setX(13).setX(34).setX(45);
    Builder b = Builder(14);
    // Increment lvalue.
    b.increment();
    // rvalue will not support increment(). This will not compile.
    // Builder(15).increment();
    std::cout << b.getX() << std::endl;
}
