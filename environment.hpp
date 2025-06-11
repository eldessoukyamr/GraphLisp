#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <string>
#include <functional>
#include "expression.hpp"
#include "builtin_procedures.hpp"

// Environment class manages symbols and procedures
class Environment {
public:
    // Constructor: Initializes the environment with built-in procedures and constants
    Environment();

    // Adds a symbol-value pair to the environment
    void add(const std::string &symbol, const Expression &value);

    // Adds a procedure to the environment
    void add_procedure(const std::string &symbol, std::function<void(const std::vector<Atom>&, Expression&)> proc);

    // Retrieves the value associated with a symbol
    Expression get(const std::string &symbol) const;

    // Retrieves the procedure associated with a symbol
    std::function<void(const std::vector<Atom>&, Expression&)> get_procedure(const std::string &symbol) const;

    // Checks if a symbol is defined in the environment
    bool is_symbol_defined(const std::string &symbol) const;

    // Checks if a procedure is defined in the environment
    bool is_procedure_defined(const std::string &symbol) const;

    void reset();

private:
    // Symbol table to store variables and constants
    std::map<std::string, Expression> symbol_table;

    // Procedure table to store built-in procedures
    std::map<std::string, std::function<void(const std::vector<Atom>&, Expression&)>> procedure_table;
};

#endif
