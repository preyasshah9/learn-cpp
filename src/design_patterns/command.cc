/*
This is an example of the Command design pattern. In the command design
pattern, there are 3 main concepts:
- Command - Base class / `std::function` for value based implementation.
- Receiver - The business logic of what happens within the commnad.
- Invoker - Triggers the command.

# Let's build a small application like calculator first with inheritance
based design. Then we will get another "remote" control like design pattern
implemented that will turn on/off different electronics within the room.
*/

// Base class for calculator command. For simplicity we will assume, the 
// tolerances are always within bound, and no overflow will occur.
// All operations are in double.

#include <iostream>
#include <vector>
#include <memory>
#include <stack>

template <typename T>
class Command {
public:
    virtual ~Command() = default;
    virtual T execute(T current) const = 0;
    virtual T undo(T current) const = 0;
};

template <typename T>
class AddCommand : public Command<T> {
    T operand_;
public:
    explicit AddCommand(T op) : operand_(op) {}
    T execute(T a) const override { return a + operand_; }
    T undo(T a) const override { return a - operand_; } // Fixed logic
};

template <typename T>
class MultiplyCommand : public Command<T> {
    T operand_;
public:
    explicit MultiplyCommand(T op) : operand_(op) {}
    T execute(T a) const override { return a * operand_; }
    T undo(T a) const override { return a / operand_; }
};

template <typename T>
class Calculator {
    T result = 0;
    // Store pointers to commands to allow undoing
    std::stack<std::unique_ptr<Command<T>>> history;

public:
    void compute(std::unique_ptr<Command<T>> cmd) {
        result = cmd->execute(result);
        history.push(std::move(cmd));
    }

    void undo() {
        if (history.empty()) return;
        result = history.top()->undo(result);
        history.pop();
    }

    void print() const { std::cout << "Current Result: " << result << std::endl; }
};

int main() {
    Calculator<double> calc;

    calc.compute(std::make_unique<AddCommand<double>>(12));
    calc.compute(std::make_unique<AddCommand<double>>(45));
    calc.compute(std::make_unique<MultiplyCommand<double>>(2));
    
    calc.print(); // 114
    calc.undo();
    calc.print(); // 57
    
    return 0;
}

