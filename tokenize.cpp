// tokenize.cpp
#include "tokenize.hpp"
#include <cctype>
#include <iostream>
#include <sstream>

// ChatGPT was used in this code for structure only 

// Function to tokenize input stream into individual tokens
TokenSequenceType tokenize(std::istream &seq) {
    TokenSequenceType tokens; // Stores the extracted tokens
    char c; // Character being read from the input stream

    // Enumeration to represent the different states during tokenization
    enum class State { Start, InNumber, InSymbol, InComment } state = State::Start;

    std::string buffer; // Temporary buffer to store multi-character tokens

    // Read characters from the input stream one by one
    while (seq.get(c)) {
        switch (state) {
            case State::Start: // Initial state, deciding token type
                if ((isspace(c) != 0)) { 
                    continue; // Ignore whitespace
                } else if (c == '(') {
                    tokens.push_back("("); // Add open parenthesis token
                } else if (c == ')') {
                    tokens.push_back(")"); // Add close parenthesis token
                } else if (c == ';') {
                    state = State::InComment; // Enter comment state, ignore text until newline
                } else if ((isdigit(c) != 0) || c == '-' || c == '.' || c == '+') {
                    buffer += c; // Start collecting a number token
                    state = State::InNumber;
                } else {
                    buffer += c; // Start collecting a symbol token
                    state = State::InSymbol;
                }
                break;
            
            case State::InNumber: // Processing a number token
                if ((isspace(c) == 0) && c != ')' && c != '(' && c != '\n') {
                    buffer += c; // Continue collecting number characters
                } else {
                    tokens.push_back(buffer); // Store the completed number token
                    buffer.clear(); // Reset buffer for next token
                    state = State::Start; // Return to start state
                    seq.putback(c); // Return character to stream for reprocessing
                }
                break;
            
            case State::InSymbol: // Processing a symbol token
                if ((isspace(c) == 0) && c != ')' && c != '(' && c != '\n') {
                    buffer += c; // Continue collecting symbol characters
                } else {
                    tokens.push_back(buffer); // Store the completed symbol token
                    buffer.clear(); // Reset buffer
                    state = State::Start; // Return to start state
                    seq.putback(c); // Return character to stream for reprocessing
                }
                break;
            
            case State::InComment: // Ignoring comments
                if (c == '\n') {
                    state = State::Start; // Exit comment mode when newline is encountered
                }
                break;
        }
    }

    // Handle any remaining token stored in buffer at the end of input
    if (!buffer.empty()) {
        tokens.push_back(buffer);
    }

    return tokens; // Return the list of extracted tokens
}
