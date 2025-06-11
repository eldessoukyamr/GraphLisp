// tokenize.hpp
#ifndef TOKENIZE_HPP
#define TOKENIZE_HPP

#include <deque>
#include <string>
#include <istream>

// Enumeration representing different token types recognized by the tokenizer
enum class TokenType {
    OpenParen,   // Token for '(' (opening parenthesis)
    CloseParen,  // Token for ')' (closing parenthesis)
    Number,      // Token representing numeric values
    Symbol,      // Token representing symbols (e.g., variable names, operators)
    Comment,     // Token representing comments (ignored during processing)
    EndOfFile    // Token indicating the end of the input stream
};

// Define a type alias for storing a sequence of tokens
using TokenSequenceType = std::deque<std::string>;

// Function declaration for tokenizing an input stream
// Reads from the provided input stream and returns a sequence of tokens
TokenSequenceType tokenize(std::istream & seq);

#endif // TOKENIZE_HPP
