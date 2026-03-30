/*
This code shows the example usage of composition. Composition (has a) is a
recommended relationship instead of private inheritance, which is
"implementated in terms of" in most cases. 
*/

#include <iostream>

class Bar {
    public:
        Bar(int bar) : bar_(bar) {}
        int getBar() {
            return bar_;
        } 
    private:
        int bar_;
};
    
class Foo {
    public:
        Foo(int foo, int bar) : b(bar), foo_(foo) {}
        
        int getFoo() {
            return b.getBar() + foo_;
        }
    private:
        // Foo has a Bar object.
        Bar b;
        int foo_;
};

int main() {
    Bar b(12);
    Foo f(12,14);
    std::cout << "f.getFoo(): " << f.getFoo() << std::endl;
    std::cout << "b.getBar(): " << b.getBar() << std::endl;
}