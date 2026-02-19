/*
This code shows the example usage of mixins and the design pattern 
CRTP (Curiously Recurring Template Pattern) in C++.

Benefits: Compile Time Type Safety
No runtime vtable lookups

Limitations: Types must be known at compile time. Containers (e.g. Vectors) storing 
objects through base class pointers won't work well.
Lifecycle Management: The derived class destructor is not called when deleting objects.
*/

#include <iostream>

// Basic Mixin usage.
struct Serializer {
    void serialize(std::string name) {
        std::cout << "c: " << name << std::endl;
    }
};

class A: public Serializer {
    public:
        A(std::string name_) : name_(name_) {}
        void bar() {
            // Mixin method.
            serialize(name_);
        }
    private:
        std::string name_;
};

// Mixin Pattern: CRTP.
template <class Derived>
struct Loggable{
    void log() {
        Derived& obj = static_cast<Derived&>(*this);
        std::cout << "Name: " << obj.name << std::endl;
    }
};

class B: public Loggable<B> {
    public:
        std::string name{"bar"}; // Default
        // Type Safety
        // Compilation error: no member named 'name_' in 'B'
        // std::string name_;
};

// Multiple Mixins
class C: public Loggable<C>, Serializer {
    public:
        std::string name;
        C(std::string name): name(name) {};
};


int main() {
    A a("test");
    a.bar();
    B b;
    b.log();
    C c("foo");
    c.log();
    // CRTP Limitation, can't create a container using base type.
    // std::vector<Loggable<?>>
    Loggable<C> loggable = C("bar");
    // Destructor of only base class is called, unlike virtual polymorphism.
    // Might lead to memory leaks if C defines a resource ownership.
}
