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
#include <stack>
class Command {
    // Executes a command.
    public:
        virtual ~Command() = default;
        virtual double execute(double a) = 0;
        // Undo a command.
        virtual double undo(double a) = 0;

};

class AddCommand : public Command {
    public:
        AddCommand(int operand): operand_(operand) {}
        double execute(double a) override {
            return a + operand_;
        }
        double undo(double a) override {
            return operand_ - a;
        }
    private:
        int operand_;
};

class MultiplyCommand : public Command {
    public:
        MultiplyCommand(int operand): operand_(operand) {}
        double execute(double a) override {
            return a * operand_;
        }
        double undo(double a) override {
            return a / operand_;
        } 
    private:
        double operand_;
};

class Calculator {
    public:
        void execute(std::unique_ptr<Command> command) {
            result = command->execute(result);
            cmdStack.push(std::move(command));
        }
        void undo() {
            if(!cmdStack.empty()) {
                result = cmdStack.top()->undo(result);
                cmdStack.pop();
            }
        }
        void printResult() {
            std::cout << result << std::endl;
        }
    private:
        std::stack<std::unique_ptr<Command>> cmdStack;
        int result = 0;
};

int main() {
    Calculator calc;
    calc.execute(std::make_unique<AddCommand>(12));
    calc.printResult();
    calc.execute(std::make_unique<AddCommand>(45));
    calc.printResult();
    calc.execute(std::make_unique<MultiplyCommand>(2));
    calc.printResult();
    calc.undo();
    calc.printResult();
}
