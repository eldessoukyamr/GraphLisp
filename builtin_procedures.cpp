#include "builtin_procedures.hpp"
#include <cmath>


/* A uniary procedure that nots an incoming boolean value */
void procNot(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 1 || params[0].type != BooleanType) {
        throw InterpreterSemanticError("not expects one boolean argument");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = !params[0].value.bool_value;
}

/* A m-ary procedure that ands incoming boolean values */
void procAnd(const std::vector<Atom>& params, Expression& output) {
    if (params.empty()) {
        throw InterpreterSemanticError("and expects at least one boolean argument");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = true;
    for (const auto& param : params) {
        if (param.type != BooleanType) {
            throw InterpreterSemanticError("and expects boolean arguments");
        }
        if (!param.value.bool_value) {
            output.head.value.bool_value = false;
            return;
        }
    }
}

/* A m-ary procedure that ors incoming boolean values */
void procOr(const std::vector<Atom>& params, Expression& output) {
    if (params.empty()) {
        throw InterpreterSemanticError("or expects at least one boolean argument");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = false;
    for (const auto& param : params) {
        if (param.type != BooleanType) {
            throw InterpreterSemanticError("or expects boolean arguments");
        }
        if (param.value.bool_value) {
            output.head.value.bool_value = true;
            return;
        }
    }
}

/* A m-ary procedure that adds incoming number values */
void procAdd(const std::vector<Atom>& params, Expression& output) {
    if (params.empty()) {
        throw InterpreterSemanticError("+ expects at least one numeric argument");
    }
    output.head.type = NumberType;
    output.head.value.num_value = 0;
    for (const auto& param : params) {
        if (param.type != NumberType) {
            throw InterpreterSemanticError("+ expects numeric arguments");
        }
        output.head.value.num_value += param.value.num_value;
    }
}

/* A m-ary procedure that subs incoming number values */
void procSubtract(const std::vector<Atom>& params, Expression& output) {
    if (params.empty() || params[0].type != NumberType) {
        throw InterpreterSemanticError("- expects at least one numeric argument");
    }
    output.head.type = NumberType;
    output.head.value.num_value = params[0].value.num_value;
    auto s = params.size();
    if (s == 1) {
        output.head.value.num_value = -output.head.value.num_value;
        return;
    }
    else if (s > 2)
    {
        throw InterpreterSemanticError("- expects at most two numeric argument");

    }
    
    output.head.value.num_value -= params[1].value.num_value;
}


/* A m-ary procedure that multiplies incoming number values */
void procMultiply(const std::vector<Atom>& params, Expression& output) {
    if (params.empty()) {
        throw InterpreterSemanticError("* expects at least one numeric argument");
    }
    output.head.type = NumberType;
    output.head.value.num_value = 1;
    for (const auto& param : params) {
        if (param.type != NumberType) {
            throw InterpreterSemanticError("* expects numeric arguments");
        }
        output.head.value.num_value *= param.value.num_value;
    }
}


/* A binary-ary procedure that divides two incoming number values */
void procDivide(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("/ expects two numeric arguments");
    }
    if (params[1].value.num_value == 0) {
        throw InterpreterSemanticError("Division by zero");
    }
    output.head.type = NumberType;
    output.head.value.num_value = params[0].value.num_value / params[1].value.num_value;
}


/* A uniary procedure that performs log base 10 on a incoming number value */
void procLog10(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 1 || params[0].type != NumberType) {
        throw InterpreterSemanticError("log10 expects one numeric argument");
    }
    output.head.type = NumberType;
    output.head.value.num_value = std::log10(params[0].value.num_value);
}


/* A binary procedure that performs pow on two incoming number values */
void procPow(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("pow expects two numeric arguments");
    }
    output.head.type = NumberType;
    output.head.value.num_value = std::pow(params[0].value.num_value, params[1].value.num_value);
}

/* A binary procedure that compares two incoming number values whether they are less than */
void procLessThan(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("< expects two numeric arguments");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = params[0].value.num_value < params[1].value.num_value;
}


/* A binary procedure that compares two incoming number values whether they are less than or equal */
void procLessThanOrEqual(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("<= expects two numeric arguments");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = params[0].value.num_value <= params[1].value.num_value;
}


/* A binary procedure that compares two incoming number values whether they are greater than */
void procGreaterThan(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("> expects two numeric arguments");
    }


    output.head.type = BooleanType;
    output.head.value.bool_value = params[0].value.num_value > params[1].value.num_value;
}


/* A binary procedure that compares two incoming number values whether they are greater than or equal to */
void procGreaterThanOrEqual(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError(">= expects two numeric arguments");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = params[0].value.num_value >= params[1].value.num_value;
}


/* A binary procedure that compares two incoming number values whether they are equal */
void procEqual(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("= expects two numeric arguments");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = params[0].value.num_value == params[1].value.num_value;
}


/* A binary procedure that creates a point expression based on two numerical values */
void procPoint(const std::vector<Atom>& params, Expression& output)
{
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType)
    {
        throw InterpreterSemanticError("point expects two numeric arguments");
    }

    new (&output) Expression(std::make_tuple(params[0].value.num_value, params[1].value.num_value));
}

/* A binary procedure that creates a line expression based on two point values */
void procLine(const std::vector<Atom>& params, Expression& output)
{
    if (params.size() != 2 || params[0].type != PointType || params[1].type != PointType)
    {
        throw InterpreterSemanticError("line expects two point arguments");
    }

    new (&output) Expression(params[0].value.point_value.toTuple(), params[1].value.point_value.toTuple());
}


/* A tri-ary procedure that creates an arc expression based on two numerical values and a numerical span angle */
void procArc(const std::vector<Atom>& params, Expression& output)
{
    if (params.size() != 3 || params[0].type != PointType || params[1].type != PointType || params[2].type != NumberType)
    {
        throw InterpreterSemanticError("arc expects two point arguments and a number argument");
    }

    new (&output) Expression(params[0].value.point_value.toTuple(), params[1].value.point_value.toTuple(), params[2].value.num_value);
}



/* A 4-ary procedure that creates a rect expression based on four numerical values */
void procRect(const std::vector<Atom>& params, Expression& output)
{
    if (params.size() != 4 || 
        params[0].type != NumberType || 
        params[1].type != NumberType || 
        params[2].type != NumberType || 
        params[3].type != NumberType)
    {
        throw InterpreterSemanticError("rect expects four point arguments");
    }

    new (&output) Expression(params[0].value.num_value, params[1].value.num_value, params[2].value.num_value, params[3].value.num_value);
}


/* A 4-ary procedure that creates a rect expression based on one rect value and three numerical values */
void procFillRect(const std::vector<Atom>& params, Expression& output)
{
    if (params.size() != 4 || 
        params[0].type != RectType || 
        params[1].type != NumberType || 
        params[2].type != NumberType || 
        params[3].type != NumberType)
    {
        throw InterpreterSemanticError("arc expects one rect argument and three number arguments");
    }

    new (&output) Expression(params[0].value.rect_value.toTuple(), params[1].value.num_value, params[2].value.num_value, params[3].value.num_value);
}


/* A uniary procedure that creates a rect expression based on one rect value */
void procEllipse(const std::vector<Atom>& params, Expression& output)
{
    if (params.size() != 1 ||
        params[0].type != RectType)
    {
        throw InterpreterSemanticError("ellipse expects one rect argument");
    }

    new (&output) Expression(params[0].value.rect_value.toTuple());
}




/* A uniary procedure that calculates sine based on one numerical value */
void procSine(const std::vector<Atom>& params, Expression& output)
{
    if (params.size() != 1 || params[0].type != NumberType)
    {
        throw InterpreterSemanticError("sin expects one numeric arguments");
    }

    output.head.type = NumberType;
    output.head.value.num_value = std::sin(params[0].value.num_value);
}


/* A uniary procedure that calculates cosine based on one numerical value */
void procCosine(const std::vector<Atom>& params, Expression& output)
{
    if (params.size() != 1 || params[0].type != NumberType)
    {
        throw InterpreterSemanticError("cos expects one numeric arguments");
    }

    output.head.type = NumberType;
    output.head.value.num_value = std::cos(params[0].value.num_value);
}


/* A binary procedure that calculates arctangent based on two numerical value */
void procArctan(const std::vector<Atom>& params, Expression& output)
{
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType)
    {
        throw InterpreterSemanticError("arctan expects two numeric arguments");
    }

    output.head.type = NumberType;
    output.head.value.num_value = std::atan2(params[0].value.num_value, params[1].value.num_value);
}