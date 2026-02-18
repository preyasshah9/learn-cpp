/*
This code shows the example usage of constexpr in C++.
*/

#include <iostream>

constexpr int x = 10;

constexpr int square(int x) {
    return x * x;
}

class Point {
    public:
        constexpr Point(int x, int y) : x_(x), y_(y) {}
        constexpr int x() const { return x_; }
        constexpr int y() const { return y_; }
    
    private:
        int x_, y_;
};
    
constexpr int y = square(5);

int main() {
    // Evaluated at Compile time.
    constexpr Point p = Point(3,5);

    // Even though `Point` constructor is constexpr, p2 is not constant object.
    // This will be evaluated at runtime.
    Point p2 = Point(2,3);
    std::cout << "x:" << p.x() << "," << "y:" << p.y() << std::endl;
    std::cout << "x:" << p2.x() << "," << "y:" << p2.y() << std::endl;
}