#ifndef BUILTIN_PROCEDURES_HPP
#define BUILTIN_PROCEDURES_HPP

#include "expression.hpp"
#include "interpreter_semantic_error.hpp"
#include <vector>

// Boolean procedures
void procNot(const std::vector<Atom>& params, Expression& output);
void procAnd(const std::vector<Atom>& params, Expression& output);
void procOr(const std::vector<Atom>& params, Expression& output);

// Numeric procedures
void procLessThan(const std::vector<Atom>& params, Expression& output);
void procLessThanOrEqual(const std::vector<Atom>& params, Expression& output);
void procGreaterThan(const std::vector<Atom>& params, Expression& output);
void procGreaterThanOrEqual(const std::vector<Atom>& params, Expression& output);
void procEqual(const std::vector<Atom>& params, Expression& output);
void procAdd(const std::vector<Atom>& params, Expression& output);
void procSubtract(const std::vector<Atom>& params, Expression& output);
void procMultiply(const std::vector<Atom>& params, Expression& output);
void procDivide(const std::vector<Atom>& params, Expression& output);
void procLog10(const std::vector<Atom>& params, Expression& output);
void procPow(const std::vector<Atom>& params, Expression& output);

// Shape procedures
void procPoint(const std::vector<Atom>& params, Expression& output);
void procLine(const std::vector<Atom>& params, Expression& output);
void procArc(const std::vector<Atom>& params, Expression& output);
void procRect(const std::vector<Atom>& params, Expression& output);
void procFillRect(const std::vector<Atom>& params, Expression& output);
void procEllipse(const std::vector<Atom>& params, Expression& output);


// Math procedures
void procSine(const std::vector<Atom>& params, Expression& output);
void procCosine(const std::vector<Atom>& params, Expression& output);
void procArctan(const std::vector<Atom>& params, Expression& output);

#endif // BUILTIN_PROCEDURES_HPP