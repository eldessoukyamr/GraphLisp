#include "environment.hpp"
#include "interpreter_semantic_error.hpp"
#include "builtin_procedures.hpp"

#include <math.h>

// Constructor: Initializes the environment with built-in procedures and constants
Environment::Environment() {
    
}

/* Resets the environment */
void Environment::reset()
{
    symbol_table.clear();
    procedure_table.clear();
}

// Adds a symbol-value pair to the environment
void Environment::add(const std::string &symbol, const Expression &value) {
    symbol_table[symbol] = value; // Store or update the symbol in the environment
}

// Adds a procedure to the environment
void Environment::add_procedure(const std::string &symbol, std::function<void(const std::vector<Atom>&, Expression&)> proc) {
    procedure_table[symbol] = proc; // Store the procedure in the procedure table
}

// Retrieves the value associated with a symbol
Expression Environment::get(const std::string &symbol) const {
    auto it = symbol_table.find(symbol);
    if (it != symbol_table.end()) {
        return it->second; // Return the symbol value if found
    }
    throw InterpreterSemanticError("Symbol '" + symbol + "' not found in environment");
}

// Retrieves the procedure associated with a symbol
std::function<void(const std::vector<Atom>&, Expression&)> Environment::get_procedure(const std::string &symbol) const {
    auto it = procedure_table.find(symbol);
    if (it != procedure_table.end()) {
        return it->second; // Return the procedure if found
    }
    throw InterpreterSemanticError("Procedure '" + symbol + "' not found in environment");
}

// Checks if a symbol is defined in the environment
bool Environment::is_symbol_defined(const std::string &symbol) const {
    return symbol_table.find(symbol) != symbol_table.end();
}

// Checks if a procedure is defined in the environment
bool Environment::is_procedure_defined(const std::string &symbol) const {
    return procedure_table.find(symbol) != procedure_table.end();
}
