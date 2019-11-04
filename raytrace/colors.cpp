#include <any>
#include <iostream>
#include <cmath>
#include <cstring>
#include <array>
#include <vector>
#include "colors.hpp"

// Color Class Functions

Color::Color (double r_, double g_, double b_)
{
    r = r_;
    g = g_;
    b = b_;
}

Color::Color ()
{
    r = 0;
    g = 0;
    b = 0;
}

Color Color::operator() (double r, double g, double b){
    Color d = Color(r, g, b);
    return d;
}

std::array<double, 3> Color::getArr ()
{
    std::array<double, 3> col = {r, g, b};

    return col;
}

Color Color::mixColor (Color a, Color b, double mix)
{
    return Color(a.r * (1 - mix) + b.r * mix, a.g * (1 - mix) + b.g * mix, a.b * (1 - mix) + b.b * mix);
}