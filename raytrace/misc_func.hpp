#include <any>
#include <cstring>
#include <cmath>
#include <array>
#include <vector>
#include "vectors.hpp"

#define Vec Eigen::Vector3d
double distanceBetween(Vec a, Vec b);
double clamp (double, double, double);
double map (double, double, double, double, double);
double max (double, double);
double davg (double, double);