#include <iostream>
#include <cmath>
#include <cstring>
#include <array>
#include <any>
#include <vector>

class Color
{
    public:
        double r, g, b;
        Color (double, double, double);
        Color ();
        Color operator() (double, double, double);
        std::array<double, 3> getArr ();
        static Color mixColor (Color, Color, double);
};