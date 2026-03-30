/*
This code shows the example usage of composition. Composition (has a) is a
recommended relationship instead of private inheritance, which is
"implementated in terms of" in most cases. 
*/

#include <iostream>

class Foo {
    public:
        Foo(int foo_): foo_(foo_) {};
        int getFoo() {
            return foo_;
        }

        virtual void fooAction() {
            std::cout << "Action on Foo()" << std::endl;
        };
    private:
        int foo_;
};

class Bar: private Foo {
    public:
        Bar(int foo): Foo(foo) {};

        // Private inheritance allows you to access virtual methods,
        // composition doesn't allow you to do that.
        void barAction() {
            fooAction();
        }
        int getBar() {
            return getFoo();
        }
};

int main() {
    Bar b = Bar(12);
    b.barAction();
    b.getBar();
    // This is error because `getFoo` is private in Bar.
    // b.getFoo();
}