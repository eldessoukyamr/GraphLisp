#ifndef TYPES_HPP
#define TYPES_HPP

// system includes
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <tuple>

#include "common_functions.hpp"
// Enumeration of possible expression types
enum Type {
    NoneType, BooleanType, NumberType, ListType, SymbolType,
    PointType, LineType, ArcType, RectType, FillRectType, EllipseType
};

// Define Boolean as an alias for bool
typedef bool Boolean;

// Define Number as an alias for double
typedef double Number;

// Define Symbol as an alias for std::string
typedef std::string Symbol;

// A Point is two Numbers
struct Point {
    Number x;
    Number y;

    bool operator==(const Point &p) const {
        return (std::fabs(x - p.x) < std::numeric_limits<double>::epsilon()) &&
               (std::fabs(y - p.y) < std::numeric_limits<double>::epsilon());
    }

    bool operator!=(const Point &p) const {
        return !(*this == p);
    }


    void operator=(std::tuple<double, double> val)
    {
        x = std::get<0>(val);
        y = std::get<1>(val);
    }

    std::tuple<double, double> toTuple() const {
        return std::make_tuple(x, y);
    }
};

// A Line is two Points
struct Line {
    Point first;
    Point second;
};

// An Arc is a center and start point and a span angle in radians
struct Arc {
    Point center;
    Point start;
    Number span;
};

struct Rect {
    Number x1;
    Number y1;
    Number x2;
    Number y2;

    bool operator==(const Rect& other) const
    {
        return (compareNumbers(x1, other.x1) &&
                compareNumbers(y1, other.y1) &&
                compareNumbers(x2, other.x2) &&
                compareNumbers(y2, other.y2));
    }

    bool operator!=(const Rect& other) const
    {
        return !(*this == other);
    }

    std::tuple<double, double, double, double> toTuple() const {
        return std::make_tuple(x1, y1, x2, y2);
    }
};

struct FillRect {
    Rect rect;
    Number r;
    Number g;
    Number b;
};

struct Ellipse {
    Rect rect;
};

// A Value is a boolean, number, or symbol
// cannot use a union because symbol is non-POD
// this wastes space but is simple 
struct Value {
    Boolean bool_value;
    Number num_value;
    Symbol sym_value;
    Point point_value;
    Line line_value;
    Arc arc_value;
    Rect rect_value;
    FillRect fillRect_value;
    Ellipse ellipse_value;
};

// An Atom has a type and value
struct Atom {
    Type type;
    Value value;
};

// Structure representing an Expression
struct Expression {
  Atom head;                  // Head of the expression (an Atom)
  std::vector<Expression> tail; // Tail containing sub-expressions

  // Default constructor initializes the head to NoneType
  Expression() {
    head.type = NoneType;
  };

  // Constructor for initializing from an Atom
  Expression(const Atom & atom) : head(atom) {};

  // Constructor for BooleanType expression
  Expression(bool tf);

  // Constructor for NumberType expression
  Expression(double num);

  // Constructor for SymbolType expression
  Expression(const std::string & sym);
  

   // Construct an Expression with a single Point atom with value
   Expression(std::tuple<double, double> value);

    // Construct an Expression with a single Line atom with starting
    // point start and ending point end
   Expression(std::tuple<double, double> start,
               std::tuple<double, double> end);

    // Construct an Expression with a single Arc atom with center
    // point center, starting point start, and spanning angle angle in radians
   Expression(std::tuple<double, double> center,
               std::tuple<double, double> start,
               double angle);

    // Construct an Expression with a single Rect atom with the four
    // values x1, y1, x2, y2
   Expression(double x1, double y1, double x2, double y2);

    // Construct an Expression with a single FillRect atom with
    // a rectangle and color code R, G, B value
   Expression(std::tuple<double, double, double, double> rect, double r, double g, double b);

    // Construct an Expression with a single Ellipse atom
    // with a bounding box rectangle
   Expression(std::tuple<double, double, double, double> rect);

  // Equality operator for comparing two expressions
  bool operator==(const Expression & exp) const noexcept;
};

// Procedure type: function pointer that takes a vector of Atoms and returns an Expression
typedef Expression (*Procedure)(const std::vector<Atom> & args);

// Overloaded operator<< to print an Expression
std::ostream & operator<<(std::ostream & out, const Expression & exp);

// Converts a token to an Atom
bool token_to_atom(const std::string & token, Atom & atom);

#endif
