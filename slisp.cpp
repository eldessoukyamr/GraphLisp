#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "interpreter.hpp"

// Runs the Read-Eval-Print Loop (REPL)
void runREPL() {
    Interpreter interpreter;
    std::string input;
    std::cout << "slisp> ";
    // Continuously read user input
    while (std::getline(std::cin, input)) {
        std::istringstream iss(input);
        // Parse the input
        if (interpreter.parse(iss)) {
            try {
                // Evaluate and print the result
                Expression result = interpreter.eval();
                std::cout << result << std::endl;
            } catch (const InterpreterSemanticError &e) {
                // Handle semantic errors
                std::cerr << "Error: " << e.what() << std::endl;

            }
        } else {
            // Handle invalid expressions
            std::cerr << "Error: Invalid expression" << std::endl;
        }
        std::cout << "slisp> ";
    }
}

// Executes expressions from a file
void runFromFile(const std::string &filename) {
    std::ifstream file(filename);
    // Check if the file can be opened
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }
    Interpreter interpreter;
    // Parse the file contents
    if (interpreter.parse(file)) {
        try {
            // Evaluate and print the result
            Expression result = interpreter.eval();
            std::cout << result << std::endl;
        } catch (const InterpreterSemanticError &e) {
            // Handle semantic errors
            std::cerr << "Error: " << e.what() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else {
        // Handle invalid expressions in the file
        std::cerr << "Error: Invalid expression in file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// Evaluates a single expression from a string
void runExpression(const std::string &expression) {
    std::istringstream iss(expression);
    Interpreter interpreter;
    // Parse the expression
    if (interpreter.parse(iss)) {
        try {
            // Evaluate and print the result
            Expression result = interpreter.eval();
            std::cout << result << std::endl;
        } catch (const InterpreterSemanticError &e) {
            // Handle semantic errors
            std::cerr << "Error: " << e.what() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else {
        // Handle invalid expressions
        std::cerr << "Error: Invalid expression" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// Main function to handle command-line arguments
int main(int argc, char **argv) {
    if (argc == 1) {
        // No arguments: Run REPL
        runREPL();
    } 
    else if (argc == 3 && std::string(argv[1]) == "-e") {
        // Evaluate a single expression
        runExpression(argv[2]);
    } 
    else if (argc == 2) {
        // Read and evaluate from a file
        runFromFile(argv[1]);
    } 
    else {
        // Display usage information for invalid arguments
        std::cerr << "Usage: slisp [-e expression] [filename]" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
