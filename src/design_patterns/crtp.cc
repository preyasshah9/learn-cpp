/*
This is an example of the Curiously Recurring Template Pattern (CRTP) design pattern.

We are designing a way to log() any class. Each class must provide a method get_name(), which
will be logged by this method. Instead of virtual functions, we will leverage compile time
polymorphism using CRTP.
*/

#include <string>
#include <iostream>
#include <vector>

template <typename Derived>
class Loggable {
    public:
        void log() const {
            const Derived& d = static_cast<const Derived&>(*this);
            std::cout << "LOG: " << d.get_name() << std::endl;
        }
        void print_type() const {
            std::cout << "Type: <class " << typeid(Derived).name() << ">" << std::endl;
        }
};

class User: public Loggable<User> {
    public:
        User(std::string name) : name_(name) {}
        std::string get_name() const {
            return name_;
        }
    private:
        std::string name_;
};

class Product: public Loggable<Product> {
    public:
        Product(std::string name) : name_(name) {}
        std::string get_name() const {
            return name_;
        }
    private:
        std::string name_;
};

// New Problem: Policy-Based Data Processor (CRTP + Mixins)
// Design a system where a Processor:

// Logs data
// Processes data
// Stores results

// All behaviors must be pluggable at compile time using policies

template <typename Derived,
          typename LoggingPolicy,
          typename ProcessingPolicy,
          typename StoragePolicy>
class Processor: public LoggingPolicy, public StoragePolicy {
    public:
        void run(int data) {
            auto& d = static_cast<Derived&>(*this);
            d.log(data);
            auto result = ProcessingPolicy::process(data);
            d.store(result);
        }
};

template <typename Derived>
struct ConsoleLogger {
    void log(int data) {
        auto& d = static_cast<Derived&>(*this);
        std::cout << "Log " << d.name() << ", Data: " << data << std::endl;
    }
};

struct SilentLogger {
    void log(int data) {
        // Do nothing.
    }
};

struct SquareProcessor {
    static int process(int data) {
        int result = data * data;
        std::cout << "Process " << result << std::endl;
        return result;
    }
};

struct DoubleProcessor {
    static int process(int data) {
        return 2 * data;
    }
};

template <typename Derived>
struct PrintStorage {
    void store(int data) {
        auto& d = static_cast<Derived&>(*this);
        std::cout << "Store: " << d.name() << std::endl;
    }
};

struct VectorStorage {
    std::vector<int> history;
    void store(int data) {
        history.push_back(data);
    }
};

class SimpleProcessor: public Processor<SimpleProcessor, ConsoleLogger<SimpleProcessor>, SquareProcessor, VectorStorage> {
    public:
        std::string name() {
            return "SimpleProcessor"; 
        }
};

int main() {
    User u("Alice");
    Product p("Laptop");

    u.log();
    p.log();

    SimpleProcessor pr;
    pr.run(5);
}
