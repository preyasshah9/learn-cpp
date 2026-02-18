/*
This code shows the basic differences in the struct v/s class.
*/

# include <iostream>
struct A {
    // Everything is public by default in struct.
    int foo;
    char bar;
    A(int foo, char bar): foo(foo), bar(bar) {};
};

class B {
    // By default, all members are private to protect invariants.
    int foo;
    char bar;
    public: // Class members are inherently private, must make constructors public.
        B(int foo, char bar): foo(foo), bar(bar) {};

        int getFoo() {
            return foo;
        }

        char getBar() {
            return bar;
        }
};

int main() {
    A a = A(1, 'r');
    B b = B(2, 'c');
    // struct members are public by default.
    std::cout << "A(): " << a.foo << "," << a.bar << std::endl;
    // This fails due to private members.
    // std::cout << "B(): " << b.foo << "," << b.bar << std::endl;
    std::cout << "B(): " << b.getFoo() << "," << b.getBar() << std::endl;
}