#include <any>
#include <iostream>
#include <cstring>
#include <array>
#include <vector>
#include "misc_func.hpp"

// Functions not in the Namespace Raytrace
double distanceBetween(Vec3D a, Vec3D b)
{
    return Vec3D::norm(a-b);
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