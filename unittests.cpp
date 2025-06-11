#define CATCH_CONFIG_MAIN  // Define the main function for Catch2 framework
#define CATCH_CONFIG_COLOUR_NONE // Disable colored output for Catch2 tests
#include "catch.hpp"
#include "interpreter.hpp"
#include "tokenize.hpp"
#include "environment.hpp"
#include <sstream> // For handling input stream manipulations

// ------------------------------- Interpreter Tests -------------------------------

// Test case for handling division by zero in the interpreter
TEST_CASE("Test division by zero", "[interpreter]") {
    Interpreter interpreter;
    std::istringstream iss("(/ 5 0)"); // Input expression that divides by zero
    REQUIRE(interpreter.parse(iss)); // Ensure parsing is successful
    REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError); // Expect an exception
}

// Test case for multiple define statements inside a 'begin' block
TEST_CASE("Test multiple define statements in begin", "[interpreter]") {
    Interpreter interpreter;
    std::istringstream iss("(begin (define x 10) (define y 20) (+ x y))");
    REQUIRE(interpreter.parse(iss)); // Ensure parsing works
    Expression result = interpreter.eval(); // Evaluate the expression
    REQUIRE(result == Expression(30.0)); // Expect result to be 30
}

// Test case for logical operations with non-boolean values
TEST_CASE("Test logical operations with non-boolean values", "[interpreter]") {
    Interpreter interpreter;
    std::istringstream iss("(and 1 2 3)"); // Logical 'and' with non-boolean values
    REQUIRE(interpreter.parse(iss)); // Parsing should succeed
    REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError); // Expect an error
}

// Test case for evaluating nested mathematical expressions
TEST_CASE("Test nested expressions", "[interpreter]") {
    Interpreter interpreter;
    std::istringstream iss("(+ (* 2 3) (/ 10 2))"); // Nested operations
    REQUIRE(interpreter.parse(iss)); // Ensure parsing works
    Expression result = interpreter.eval(); // Evaluate the expression
    REQUIRE(result == Expression(11.0)); // Expected result is 11
}

// Test case for redeclaring an already defined symbol
TEST_CASE("Test redeclaring a symbol", "[interpreter]") {
    Interpreter interpreter;
    std::istringstream iss("(begin (define x 10) (define x 20))"); // Attempt to redefine 'x'
    REQUIRE(interpreter.parse(iss)); // Ensure parsing works
    REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError); // Expect an error
}

// ------------------------------- Tokenization Tests -------------------------------

// Test case for tokenizing symbols that include special characters
TEST_CASE("Test tokenization of symbols with special characters", "[tokenize]") {
    std::istringstream input("(define !special_symbol 10)");
    auto tokens = tokenize(input);
    REQUIRE((tokens == TokenSequenceType{"(", "define", "!special_symbol", "10", ")"})); // Expect correct token sequence
}

// Test case for handling inconsistent spacing between tokens
TEST_CASE("Test tokenization with mixed spacing", "[tokenize]") {
    std::istringstream input("( +  1   2 )"); // Extra spaces between tokens
    auto tokens = tokenize(input);
    REQUIRE((tokens == TokenSequenceType{"(", "+", "1", "2", ")"})); // Expect correctly parsed tokens
}

// Test case for handling consecutive parentheses
TEST_CASE("Test tokenization with multiple consecutive parentheses", "[tokenize]") {
    std::istringstream input("(()())"); // Input with multiple parentheses
    auto tokens = tokenize(input);
    REQUIRE((tokens == TokenSequenceType{"(", "(", ")", "(", ")", ")"})); // Expect correct token sequence
}

// Test case for ignoring comments within expressions
TEST_CASE("Test tokenization with comments interspersed in input", "[tokenize]") {
    std::istringstream input("(+ 1 2 ; this is a comment\n 3)");
    auto tokens = tokenize(input);
    REQUIRE((tokens == TokenSequenceType{"(", "+", "1", "2", "3", ")"})); // Expect tokens without comment
}

// Test case for handling empty input
TEST_CASE("Test tokenization of empty input", "[tokenize]") {
    std::istringstream input(""); // Empty input stream
    auto tokens = tokenize(input);
    REQUIRE(tokens.empty()); // Expect empty token sequence
}

// Test case for handling unusual whitespace characters
TEST_CASE("Test tokenization with unusual whitespace characters", "[tokenize]") {
    std::istringstream input("(define\t x\n 10)"); // Includes tab and newline characters
    auto tokens = tokenize(input);
    REQUIRE((tokens == TokenSequenceType{"(", "define", "x", "10", ")"})); // Expect properly parsed tokens
}


TEST_CASE("Test procNot", "[builtin]") {
    Expression out;
    Atom a;
    a.type = BooleanType;
    a.value.bool_value = true;
    std::vector<Atom> params = {a};
    procNot(params, out);
    REQUIRE(out == Expression(false));

    SECTION("Invalid argument type") {
        Atom n; n.type = NumberType; n.value.num_value = 1.0;
        REQUIRE_THROWS_AS(procNot({n}, out), InterpreterSemanticError);
    }
    SECTION("Too many arguments") {
        REQUIRE_THROWS_AS(procNot({a, a}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procAnd", "[builtin]") {
    Expression out;
    Atom t; t.type = BooleanType; t.value.bool_value = true;
    Atom f; f.type = BooleanType; f.value.bool_value = false;
    std::vector<Atom> params = {t, f};
    procAnd(params, out);
    REQUIRE(out == Expression(false));

    SECTION("Empty input") {
        std::vector<Atom> empty;
        REQUIRE_THROWS_AS(procAnd(empty, out), InterpreterSemanticError);
    }
    SECTION("Invalid argument type") {
        Atom n; n.type = NumberType; n.value.num_value = 1.0;
        REQUIRE_THROWS_AS(procAnd({t, n}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procOr", "[builtin]") {
    Expression out;
    Atom t; t.type = BooleanType; t.value.bool_value = true;
    Atom f; f.type = BooleanType; f.value.bool_value = false;
    std::vector<Atom> params = {f, t};
    procOr(params, out);
    REQUIRE(out == Expression(true));

    SECTION("Empty input") {
        std::vector<Atom> empty;
        REQUIRE_THROWS_AS(procOr(empty, out), InterpreterSemanticError);
    }
    SECTION("Invalid argument type") {
        Atom n; n.type = NumberType; n.value.num_value = 1.0;
        REQUIRE_THROWS_AS(procOr({f, n}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procAdd", "[builtin]") {
    Expression out;
    Atom a; a.type = NumberType; a.value.num_value = 5.0;
    Atom b; b.type = NumberType; b.value.num_value = 3.0;
    std::vector<Atom> params = {a, b};
    procAdd(params, out);
    REQUIRE(out == Expression(8.0));

    SECTION("Empty input") {
        std::vector<Atom> empty;
        REQUIRE_THROWS_AS(procAdd(empty, out), InterpreterSemanticError);
    }
    SECTION("Invalid argument type") {
        Atom t; t.type = BooleanType; t.value.bool_value = true;
        REQUIRE_THROWS_AS(procAdd({a, t}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procSubtract", "[builtin]") {
    Expression out;
    Atom a; a.type = NumberType; a.value.num_value = 10.0;
    Atom b; b.type = NumberType; b.value.num_value = 4.0;
    std::vector<Atom> params = {a, b};
    procSubtract(params, out);
    REQUIRE(out == Expression(6.0));

    SECTION("Unary minus") {
        std::vector<Atom> unary = {b};
        procSubtract(unary, out);
        REQUIRE(out == Expression(-4.0));
    }
    SECTION("Invalid argument count") {
        REQUIRE_THROWS_AS(procSubtract({}, out), InterpreterSemanticError);
        REQUIRE_THROWS_AS(procSubtract({a, b, a}, out), InterpreterSemanticError);
    }
    SECTION("Invalid type") {
        Atom t; t.type = BooleanType; t.value.bool_value = false;
        REQUIRE_THROWS_AS(procSubtract({t}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procMultiply", "[builtin]") {
    Expression out;
    Atom a; a.type = NumberType; a.value.num_value = 2.0;
    Atom b; b.type = NumberType; b.value.num_value = 3.0;
    std::vector<Atom> params = {a, b};
    procMultiply(params, out);
    REQUIRE(out == Expression(6.0));

    SECTION("Empty input") {
        std::vector<Atom> empty;
        REQUIRE_THROWS_AS(procMultiply(empty, out), InterpreterSemanticError);
    }
    SECTION("Invalid type") {
        Atom t; t.type = BooleanType; t.value.bool_value = true;
        REQUIRE_THROWS_AS(procMultiply({a, t}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procDivide", "[builtin]") {
    Expression out;
    Atom a; a.type = NumberType; a.value.num_value = 10.0;
    Atom b; b.type = NumberType; b.value.num_value = 2.0;
    std::vector<Atom> params = {a, b};
    procDivide(params, out);
    REQUIRE(out == Expression(5.0));

    SECTION("Division by zero") {
        Atom zero; zero.type = NumberType; zero.value.num_value = 0.0;
        REQUIRE_THROWS_AS(procDivide({a, zero}, out), InterpreterSemanticError);
    }
    SECTION("Invalid count or type") {
        REQUIRE_THROWS_AS(procDivide({a}, out), InterpreterSemanticError);
        Atom t; t.type = BooleanType; t.value.bool_value = true;
        REQUIRE_THROWS_AS(procDivide({a, t}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procLog10", "[builtin]") {
    Expression out;
    Atom a; a.type = NumberType; a.value.num_value = 100.0;
    std::vector<Atom> params = {a};
    procLog10(params, out);
    REQUIRE(out == Expression(2.0));

    SECTION("Invalid input") {
        Atom t; t.type = BooleanType; t.value.bool_value = false;
        REQUIRE_THROWS_AS(procLog10({t}, out), InterpreterSemanticError);
        REQUIRE_THROWS_AS(procLog10({}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procPow", "[builtin]") {
    Expression out;
    Atom base; base.type = NumberType; base.value.num_value = 2.0;
    Atom exp; exp.type = NumberType; exp.value.num_value = 3.0;
    std::vector<Atom> params = {base, exp};
    procPow(params, out);
    REQUIRE(out == Expression(8.0));

    SECTION("Invalid args") {
        Atom t; t.type = BooleanType; t.value.bool_value = true;
        REQUIRE_THROWS_AS(procPow({base, t}, out), InterpreterSemanticError);
        REQUIRE_THROWS_AS(procPow({base}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procLessThan", "[builtin]") {
    Expression out;
    Atom a; a.type = NumberType; a.value.num_value = 2.0;
    Atom b; b.type = NumberType; b.value.num_value = 5.0;
    std::vector<Atom> params = {a, b};
    procLessThan(params, out);
    REQUIRE(out == Expression(true));

    SECTION("Invalid args") {
        REQUIRE_THROWS_AS(procLessThan({a}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procGreaterThan", "[builtin]") {
    Expression out;
    Atom a; a.type = NumberType; a.value.num_value = 10.0;
    Atom b; b.type = NumberType; b.value.num_value = 2.0;
    std::vector<Atom> params = {a, b};
    procGreaterThan(params, out);
    REQUIRE(out == Expression(true));

    SECTION("Invalid args") {
        REQUIRE_THROWS_AS(procGreaterThan({a}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procEqual", "[builtin]") {
    Expression out;
    Atom a; a.type = NumberType; a.value.num_value = 7.0;
    Atom b; b.type = NumberType; b.value.num_value = 7.0;
    std::vector<Atom> params = {a, b};
    procEqual(params, out);
    REQUIRE(out == Expression(true));

    SECTION("Invalid args") {
        REQUIRE_THROWS_AS(procEqual({a}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procSine", "[builtin]") {
    Expression out;
    Atom a; a.type = NumberType; a.value.num_value = 0.0;
    std::vector<Atom> params = {a};
    procSine(params, out);
    REQUIRE(out == Expression(0.0));

    SECTION("Invalid args") {
        Atom b; b.type = BooleanType; b.value.bool_value = false;
        REQUIRE_THROWS_AS(procSine({b}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procCosine", "[builtin]") {
    Expression out;
    Atom a; a.type = NumberType; a.value.num_value = 0.0;
    std::vector<Atom> params = {a};
    procCosine(params, out);
    REQUIRE(out == Expression(1.0));

    SECTION("Invalid args") {
        Atom b; b.type = BooleanType; b.value.bool_value = false;
        REQUIRE_THROWS_AS(procCosine({b}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Test procArctan", "[builtin]") {
    Expression out;
    Atom y; y.type = NumberType; y.value.num_value = 1.0;
    Atom x; x.type = NumberType; x.value.num_value = 1.0;
    std::vector<Atom> params = {y, x};
    procArctan(params, out);
    REQUIRE(out.head.value.num_value == Approx(0.785398));

    SECTION("Invalid args") {
        REQUIRE_THROWS_AS(procArctan({x}, out), InterpreterSemanticError);
    }
}

TEST_CASE("Environment symbol table behavior", "[environment]") {
    Environment env;

    Expression val1(10.0);
    env.add("a", val1);
    REQUIRE(env.is_symbol_defined("a"));
    REQUIRE(env.get("a") == val1);

    SECTION("Symbol redefinition") {
        Expression val2(20.0);
        env.add("a", val2);
        REQUIRE(env.get("a") == val2);
    }

    SECTION("Missing symbol throws") {
        REQUIRE_THROWS_AS(env.get("missing"), InterpreterSemanticError);
    }
}

TEST_CASE("Environment procedure table behavior", "[environment]") {
    Environment env;
    env.add_procedure("truthy", [](const std::vector<Atom>&, Expression& out) {
        out = Expression(true);
    });
    REQUIRE(env.is_procedure_defined("truthy"));

    Expression result;
    env.get_procedure("truthy")({}, result);
    REQUIRE(result == Expression(true));

    SECTION("Missing procedure throws") {
        REQUIRE_THROWS_AS(env.get_procedure("missing_proc"), InterpreterSemanticError);
    }
}

TEST_CASE("Environment reset clears symbol and procedure tables", "[environment]") {
    Environment env;
    env.add("temp", Expression(1.0));
    env.add_procedure("dummy", [](const std::vector<Atom>&, Expression& out) {
        out = Expression(false);
    });
    REQUIRE(env.is_symbol_defined("temp"));
    REQUIRE(env.is_procedure_defined("dummy"));

    env.reset();
    REQUIRE_FALSE(env.is_symbol_defined("temp"));
    REQUIRE_FALSE(env.is_procedure_defined("dummy"));

    REQUIRE_THROWS_AS(env.get("temp"), InterpreterSemanticError);
    REQUIRE_THROWS_AS(env.get_procedure("dummy"), InterpreterSemanticError);
} 

TEST_CASE("Expression assignment operator: different types", "[expression][operator=]") {
    Expression num(3.14);
    Expression boolean(false);

    num = boolean;

    REQUIRE(num == boolean);
    REQUIRE(num.head.type == BooleanType);
}

TEST_CASE("Expression assignment operator: both SymbolType", "[expression][operator=]") {
    std::string _a = "alpha";
    std::string _b = "beta";
    Expression a(_a);
    Expression b(_b);

    //a = b;

    REQUIRE_FALSE(a == b);
    REQUIRE(a.head.type == SymbolType);
    //REQUIRE(a.head.value.sym_value == "beta");

    a = b;
    REQUIRE(a == b);
}

TEST_CASE("Expression assignment operator: default/fallback case", "[expression][operator=]") {
    Expression a;
    Expression b(std::make_tuple(1.0, 2.0));

    a = b;

    REQUIRE(a == b);
    REQUIRE(a.head.type == PointType);
}