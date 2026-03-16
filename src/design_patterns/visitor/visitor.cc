 /*
This code shows the example of a visitor pattern in C++.

We will leverage the std::visit, and the std::variant introduced in 
C++17 for this example. The OOP version of visitor design pattern is
more involved, unfriendly and have performance degradations. So we 
will not consider it in this exampe.

Exercise:
Extensible Expression Tree with Multiple Operations
Design a small expression tree system for a mathematical engine. The system represents arithmetic expressions as a tree of nodes.

Each node in the tree is one of the following types:
- Number — represents an integer value
- Addition — represents the sum of two expressions
- Multiplication — represents the product of two expressions

System Requirement:
Design the system so that new operations can be added without modifying the expression node classes.
Each expression node should support an `accept` method that allows external operations to be performed on it.
*/

#include <variant>
#include <string>
#include <iostream>
#include <map>

namespace Solution {

    // Define Add: Sum of two numbers. 
    struct Add {
        int accept(int a, int b) {
            return a + b;
        }
    };
    
    // Define Multiply: Product of two numbers.
    struct Multiply {
        int accept(int a, int b) {
            return a * b;
        }
    };

    // Define Subtract: Deduct 2 from 1.
    // Adding new expression nodes is easy.
    struct Subtract {
        int accept(int a, int b) {
            return a - b;
        }
    };

    using OperationT = std::variant<Add, Multiply, Subtract>;
    static const std::map<std::string, OperationT> opMap = {
        {"+", Add{}},
        {"-", Subtract{}},
        {"*", Multiply{}}
    };

    struct Node {
        std::string val;
        Node* left = nullptr;
        Node* right = nullptr;

        Node(std::string val): val(val) {};
    };

    void postFix(Node* root) {
        if(root == nullptr) return;
        postFix(root -> left);
        postFix(root -> right);
        std::cout << root -> val << " ";
    }

    void inFix(Node* root) {
        if(root == nullptr) return;
        inFix(root -> left);
        std::cout << root -> val << " ";
        inFix(root -> right);
    }

    int evaluate(Node* root) {
        if(root -> left == nullptr && root -> right == nullptr) {
            return std::stoi(root -> val);
        }
        int left = 0, right = 0;
        if(root -> left != nullptr && root -> right != nullptr) {
            left = evaluate(root -> left);
            right = evaluate(root -> right);
        }

        OperationT operation = opMap.at(root->val);
        return std::visit([left, right](auto& op){
            return op.accept(left, right);
        }, operation);
    }
}

int main() {
    using namespace Solution;

    // Build expression tree: (3 + 5) * 2
    Node* n3 = new Node("3");
    Node* n5 = new Node("5");
    Node* n2 = new Node("2");

    Node* plus = new Node("+");
    plus->left = n3;
    plus->right = n5;

    Node* mult = new Node("*");
    mult->left = plus;
    mult->right = n2;

    int result = evaluate(mult);
    std::cout << "(3 + 5) * 2 = " << result << std::endl;
    
    std::cout << "InFix Traversal: " << std::endl;
    inFix(mult);
    std::cout << "PostFix Traversal: " << std::endl;
    postFix(mult);

    // Cleanup memory
    delete n3;
    delete n5;
    delete n2;
    delete plus;
    delete mult;

    return 0;
}