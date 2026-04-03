/*
This is an example of the strategy design pattern.

Strategy defines how something needs to be done.
- Compared to Command design pattern, it does not dictate what needs to be done. Command
design pattern is especially mandatory for undo operations.
- Unlike Visitor design pattern, it is Open for types, but closed for operations.

Let's implement the Strategy pattern using two classes, Square and Circle which needs to provide
Drawable behavior using draw(...args). We will implement inheritance based solution
and also show Value based implementation using `std::function`.
*/

#include <functional>
#include <iostream>
#include <vector>
#include <memory> 

class Shape {
    public:
        virtual void draw() = 0;
        virtual ~Shape() = default;
};

class DrawStrategy {
    public:
        virtual void draw() = 0;
        virtual ~DrawStrategy() = default;
};

class SimpleDrawStrategy: public DrawStrategy {
    public:
        void draw() override {
            std::cout << "Drawing Standard" << std::endl;
        }
};

class ComplexDrawStrategy: public DrawStrategy {
    public:
        void draw() override {
            std::cout << "Drawing Complex" << std::endl;
        }
};

class Square: public Shape {
    public:
        Square(int side, std::unique_ptr<DrawStrategy> drawer_): side_(side), drawer_(std::move(drawer_)) {} 
        void draw() override {
            drawer_->draw();
        }
        int area() {
            return side_ * side_;
        }
    private:
        std::unique_ptr<DrawStrategy> drawer_;
        int side_;
};

class Circle: public Shape {
    public:
        Circle(int radius, std::unique_ptr<DrawStrategy> drawer): radius_(radius), drawer_(std::move(drawer)) {} 
        void draw() override {
            drawer_->draw();
        }
    double getArea() {
        return 3.14 * radius_ * radius_;
    }

    double getPerimeter() {
        return 2 * 3.14 * radius_;
    }
    private:
        std::unique_ptr<DrawStrategy> drawer_;
        int radius_;
};

namespace Functional {
    class Shape {
        public:
            virtual void draw() const = 0;
            virtual ~Shape() = default;
    };
    
    class Square: public Shape {
        using DrawStrategy = std::function<void(Square const&)>;
        public:
            Square(int side, DrawStrategy drawer_): side_(side), drawer_(std::move(drawer_)) {} 
            void draw() const override {
                drawer_(*this);
            }
            int area() {
                return side_ * side_;
            }
        private:
            DrawStrategy drawer_;
            int side_;
    };
    
    class Circle: public Shape {
        using DrawStrategy = std::function<void(Circle const&)>;
        public:
        Circle(int side, DrawStrategy drawer_): side_(side), drawer_(std::move(drawer_)) {} 
            void draw() const override {
                drawer_(*this);
            }
            int area() {
                return side_ * side_;
            }
        private:
            DrawStrategy drawer_;
            int side_;
    };
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes{};
    shapes.emplace_back(std::make_unique<Circle>(10, std::make_unique<SimpleDrawStrategy>()));
    shapes.emplace_back(std::make_unique<Square>(15, std::make_unique<SimpleDrawStrategy>()));
    shapes.emplace_back(std::make_unique<Circle>(12, std::make_unique<ComplexDrawStrategy>()));

    for(auto& shape: shapes) {
        shape->draw();
    }

    // Using Functional - Value based pattern.
    auto simpleDrawingFunc = [](const Functional::Shape& shape) {
        std::cout << "Drawing Simple";
    };

    auto ComplexDrawingFunc = [](const Functional::Shape& shape) {
        std::cout << "Drawing Complex";
    };

    std::vector<std::unique_ptr<Functional::Shape>> shapes_func{};
    shapes_func.emplace_back(std::make_unique<Functional::Circle>(10, simpleDrawingFunc));
    shapes_func.emplace_back(std::make_unique<Functional::Square>(15, simpleDrawingFunc));
    shapes_func.emplace_back(std::make_unique<Functional::Circle>(12, ComplexDrawingFunc));

    for(auto& shape: shapes) {
        shape->draw();
    }    
}