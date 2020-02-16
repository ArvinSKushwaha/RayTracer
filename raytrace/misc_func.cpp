#include <any>
#include <iostream>
#include <cstring>
#include <array>
#include <vector>
#include <eigen3/Eigen/Core>
#include "misc_func.hpp"

// Functions not in the Namespace Raytrace
double distanceBetween(Vec a, Vec b)
{
    return (a-b).norm();
}

double clamp(double value, double min, double max)
{
    return (value >= min && value <= max ? value : (value < min ? min : max));
}

double map(double value, double min_r1, double max_r1, double min_r2, double max_r2)
{
    double pc = (value - min_r1)/(max_r1 - min_r1);
    return min_r2 + pc * (max_r2 - min_r2);
}

double max(double a, double b)
{
    return (a < b) ? b : a;
}

double davg(double a, double b)
{
    return (a+b)/2.0;
}