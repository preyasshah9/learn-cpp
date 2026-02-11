/*
This code shows how alignment works by defining a few structs, and ordering of the members within struct
can affect the storage occupied by it.
*/

#include <iostream>

struct A {
    int foo;
    char bar;
    short baz;
};

struct B {
    char bar;
    int foo;
    short baz;
};

/*
Output 
Sizeof A: 8
Sizeof B: 12

Allocation for A:
|<- int (4 bytes) ->|<- char (1 byte) alignment (1 byte) short (2 bytes) ->|

Allocation for B:
|<- char (1 byte) alignment (3 bytes) ->|<- int (4 bytes) ->|<- short (2 bytes) alignment (2 bytes) ->|

If we consider adding one more char variable, A2 -> 8 bytes and A3 -> 12 bytes and hence A2 is more space-efficient.
struct A2 {
    int foo;
    char bar;
    char bar2;
    short baz;
}

struct A3 {
    int foo;
    char bar;
    short baz;
    char bar2;
}
*/

struct A2 {
    int foo;
    char bar;
    char bar2;
    short baz;
};

struct A3 {
    int foo;
    char bar;
    short baz;
    char bar2;
};


int main() {
    std::cout << "Sizeof A: " << sizeof(A) << ", Sizeof B: " << sizeof(B) << std::endl;
    std::cout << "A occupied " << sizeof(B) - sizeof(A) << " bytes less than B for same set of fields." << std::endl;
    std::cout << "Sizeof A2: " << sizeof(A2) << ", Sizeof A3: " << sizeof(A3) << std::endl;
    std::cout << "A2 occupied " << sizeof(A3) - sizeof(A2) << " bytes less than A3 for same set of fields." << std::endl;
    return 0;
}