#include "common_functions.hpp"
#include <cmath>
#include <limits>

/* Compares two double numbers whether they are equal*/
bool compareNumbers(double A, double B)
{
    return std::fabs(A - B) < std::numeric_limits<double>::epsilon();
}