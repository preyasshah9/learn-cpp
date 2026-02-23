/*
This code shows the advance techniques of object construction using plaacement
new. 
When a new operator is used as:
A a = new A(Args...)
There are two operations involved.
- Heap Allocation
- Object Construction
With placement new, you can decouple these operations and use allocations separately.
This is useful for low level embedded/games programming.

NOTE: In C++20, the placement new (and destruction) syntax is simplified with 
- std::construct_at
- std::destroy_at
*/

#include <iostream>
#include <array>
#include <bitset>
#include <new>      // placement new
# include <memory> // Will be needed if you use newer C++ construct.

// Single Object
class A {
    public:
        A(int x): _foo(x) {
            std::cout << "A() called" << std::endl;
        }
        ~A() noexcept {
            std::cout << "~A() called" << std::endl;
        }
        int getFoo() {
            return _foo;
        }
    private:
        int _foo;
};

// Now, let's create a memory pool with list of A objects on a custom memory pool.
template <std::size_t N>
class APool {
    public:
        APool() 
        {
            std::cout << "APool() called" << std::endl;
        }
        ~APool() {
            for(int i = 0; i < N; i++) {
                if(used[i]) {
                    get_ptr(i)->~A();
                }
            }
            std::cout << "~APool() called" << std::endl;
        }
        A* create(int val) {
            for(std::size_t i = 0; i < N; i++) {
                if(!used[i]) {
                    std::cout << "A() called at " << i << std::endl;
                    used[i] = true;
                    A* a = new (buffer[i].data()) A(val);
                    // If val is instead multiple values, use args as a template parameter. 
                    // and use std::forward<Args><args...> 
                    // Alternatively, use std::construct_at(get_ptr(i), val);
                    return a;
                }
            }
            return nullptr;
        }
        void destroy(A* a) {
            for(std::size_t i = 0; i < N; i++) {
                if(a == get_ptr(i)) {
                    std::cout << "~A() called at " << i << std::endl;
                    a->~A();
                    // Alternatively, use std::destroy_at(a);
                    used[i] = false;
                    return;
                }
            }
        }
    private:
        alignas(A) std::array<std::byte, sizeof(A)> buffer[N];
        std::bitset<N> used{};
        A* get_ptr(std::size_t i) {
            return reinterpret_cast<A*>(buffer[i].data());
        }

};

// Let's use placement new for allocation.
A* allocate_single_object(int x) {
    // Memory allocation and alignment.
    // 1. Allocate a buffer of size A on stack.
    // 2. Make sure Alignment of this allocation happens at boundary that is required for A.
    alignas(A) char buffer[sizeof(A)];
    // Construct an A object in this buffer.
    A* a = new (buffer) A(x);
    std::cout << "A->getFoo() = " << a->getFoo() << std::endl;
    return a;
}

int main() {
    std::cout << "Allocating a single object" << std::endl;
    A* a = allocate_single_object(12);
    a->~A(); // Destroy object manually.

    APool<5> aPool;
    A* a1 = aPool.create(3);
    A* a2 = aPool.create(4);
    aPool.destroy(a1);
    aPool.destroy(a2);
}
