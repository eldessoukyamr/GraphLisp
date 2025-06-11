#include "expression.hpp"
#include <cctype>
#include <sstream>
#include <stdexcept>

// chatgpt was used on this file for structure


static bool isNumber(const std::string &token);
static bool isSymbol(const std::string &token);

// Constructor for BooleanType expression
Expression::Expression(bool tf) {
    head.type = BooleanType;
    head.value.bool_value = tf;
}

// Constructor for NumberType expression
Expression::Expression(double num) {
    head.type = NumberType;
    head.value.num_value = num;
}

// Constructor for SymbolType expression
Expression::Expression(const std::string & sym) {
    head.type = SymbolType;
    head.value.sym_value = sym;
}

// Constructor for PointType expression
Expression::Expression(std::tuple<double, double> value)
{
    head.type = PointType;
    //head.value.point_value.x = std::get<0>(value);
    //head.value.point_value.y = std::get<1>(value);
    head.value.point_value = value;
}

// Constructor for LineType expression
Expression::Expression(std::tuple<double, double> start, std::tuple<double, double>  end)
{
    head.type = LineType;
    head.value.line_value.first = start;
    head.value.line_value.second = end;
}

// Constructor for ArcType expression
Expression::Expression(std::tuple<double, double> center, std::tuple<double, double> start, double angle)
{
    head.type = ArcType;
    head.value.arc_value.center = center;
    head.value.arc_value.start = start;
    head.value.arc_value.span = angle;
}

// Constructor for RectType expression
Expression::Expression(double x1, double y1, double x2, double y2)
{
    head.type = RectType;
    head.value.rect_value.x1 = x1;
    head.value.rect_value.y1 = y1;
    head.value.rect_value.x2 = x2;
    head.value.rect_value.y2 = y2;
}

// Constructor for EllipseType expression
Expression::Expression(std::tuple<double, double, double, double> rect)
{
    head.type = EllipseType;
    head.value.ellipse_value.rect.x1 = std::get<0>(rect);
    head.value.ellipse_value.rect.y1 = std::get<1>(rect);
    head.value.ellipse_value.rect.x2 = std::get<2>(rect);
    head.value.ellipse_value.rect.y2 = std::get<3>(rect);
}

// Constructor for SymbolType expression
Expression::Expression(std::tuple<double, double, double, double> rect, double r, double g, double b)
{
    head.type = FillRectType;
    head.value.fillRect_value.rect.x1 = std::get<0>(rect);
    head.value.fillRect_value.rect.y1 = std::get<1>(rect);
    head.value.fillRect_value.rect.x2 = std::get<2>(rect);
    head.value.fillRect_value.rect.y2 = std::get<3>(rect);

    head.value.fillRect_value.r = r;
    head.value.fillRect_value.g = g;
    head.value.fillRect_value.b = b;
}

// Equality operator for comparing two expressions
bool Expression::operator==(const Expression & exp) const noexcept {

    // Check if types are different
    if (head.type != exp.head.type)
    {
        return false;
    }

    // Compare based on type
    switch (head.type) {
        case NoneType:
            return true; // NoneType expressions are always equal
        case BooleanType:
            return head.value.bool_value == exp.head.value.bool_value;
        case NumberType:
            return compareNumbers(head.value.num_value, exp.head.value.num_value);//std::fabs(head.value.num_value - exp.head.value.num_value) < EPSILON;
        case SymbolType:
            return head.value.sym_value == exp.head.value.sym_value;
        case PointType:
            return (head.value.point_value == exp.head.value.point_value);
        case LineType:
            return ((head.value.line_value.first == exp.head.value.line_value.first) &&
                    (head.value.line_value.second == exp.head.value.line_value.second));
        case ArcType:
            return (compareNumbers(head.value.arc_value.span, exp.head.value.arc_value.span) &&
                    head.value.arc_value.center == exp.head.value.arc_value.center &&
                    head.value.arc_value.start == exp.head.value.arc_value.start);
        case RectType:
            return (head.value.rect_value == exp.head.value.rect_value);
        case FillRectType:
            return ((head.value.fillRect_value.rect == exp.head.value.fillRect_value.rect) &&
                    (compareNumbers(head.value.fillRect_value.r, exp.head.value.fillRect_value.r)) &&
                    (compareNumbers(head.value.fillRect_value.g, exp.head.value.fillRect_value.g)) &&
                    (compareNumbers(head.value.fillRect_value.b, exp.head.value.fillRect_value.b)));
        case EllipseType:
            return ((head.value.ellipse_value.rect == exp.head.value.ellipse_value.rect));
    }

    return false;
}



// Check if a token is a number
static bool isNumber(const std::string &token) {
    if (token.empty()) 
    {
        return false;
    }
    // Check if the token starts with a digit
    if (isdigit(token[0]) != 0) 
    {
        return true;
    }
    // Check for optional + or - sign followed by a digit
    else if ((token[0] == '+' || token[0] == '-') && token.length() > 1 && (isdigit(token[1]) != 0)) 
    {
        return true;
    }

    return false;
}

// Check if a token is a symbol
static bool isSymbol(const std::string &token) {
    return !isNumber(token) && !token.empty();
}

// Convert token to an Atom based on its type
bool token_to_atom(const std::string & token, Atom & atom) {
    if (token == "True") {
        atom.type = BooleanType;
        atom.value.bool_value = true;
    } 
    else if (token == "False") {
        atom.type = BooleanType;
        atom.value.bool_value = false;
    } 
    // Check if the token is a symbol before checking if it's a number
    else if (isSymbol(token)) {
        atom.type = SymbolType;
        atom.value.sym_value = token;
    } 
    // Check if the token is a number
    else {
        size_t pos = 0;
        try {
            double num = std::stod(token, &pos);
            // Ensure the entire token was parsed successfully
            if (pos == token.length()) {
                atom.type = NumberType;
                atom.value.num_value = num;
                return true;
            }
        } catch (const std::exception &) {
            return false; // Return false if the token is invalid
        }

        return false;
    }

    return true;
}
// Overload the << operator for printing an expression
std::ostream & operator<<(std::ostream & out, const Expression & exp) {
    out << '(';
    switch (exp.head.type) {
        case NoneType:
            out << "None";
            break;
        case BooleanType:
            out << (exp.head.value.bool_value ? "True" : "False");
            break;
        case NumberType:
            out << exp.head.value.num_value;
            break;
        case SymbolType:
            out << exp.head.value.sym_value;
            break;
        case PointType:
            out << exp.head.value.point_value.x << ',' << exp.head.value.point_value.y;
            break;
        case LineType:
            out << '(' << 
            exp.head.value.line_value.first.x << ',' << exp.head.value.line_value.first.y << "),(" << 
            exp.head.value.line_value.second.x << ',' << exp.head.value.line_value.second.y << ')';
            break;
        case ArcType:
            out << '(' <<
            exp.head.value.arc_value.center.x << ',' <<
            exp.head.value.arc_value.center.y << "),(" <<
            exp.head.value.arc_value.start.x << ',' <<
            exp.head.value.arc_value.start.y << ") " <<
            exp.head.value.arc_value.span << ')';
            break;
        case RectType:
            out << 
            exp.head.value.rect_value.x1 << ',' <<
            exp.head.value.rect_value.y1 << ',' <<
            exp.head.value.rect_value.x2 << ',' <<
            exp.head.value.rect_value.y2;
            break;
        case FillRectType:
            out << '(' << 
            exp.head.value.fillRect_value.rect.x1 << ',' <<
            exp.head.value.fillRect_value.rect.y1 << ',' <<
            exp.head.value.fillRect_value.rect.x2 << ',' <<
            exp.head.value.fillRect_value.rect.y2 << ")," <<
            exp.head.value.fillRect_value.r << ',' <<
            exp.head.value.fillRect_value.g << ',' <<
            exp.head.value.fillRect_value.b;
            break;
        case EllipseType:
            out <<
            exp.head.value.fillRect_value.rect.x1 << ',' <<
            exp.head.value.fillRect_value.rect.y1 << ',' <<
            exp.head.value.fillRect_value.rect.x2 << ',' <<
            exp.head.value.fillRect_value.rect.y2 << ')';
            break;
        default:
            out << "Unknown";
            break;
    }
    out << ')';
    return out;
}


