#include <any>
#include <iostream>
#include <cmath>
#include <cstring>
#include <array>
#include <vector>
#include "colors.hpp"
#include <eigen3/Eigen/Core>

#define Vec Eigen::Vector3d

class Ray
{
public:
    Vec origin;
    Vec direction;
    Color color = Color(NAN, NAN, NAN);
    Ray(Vec, Vec);
    Ray reflect(Vec, Vec);
    Ray refract(Vec, Vec, double n1, double n2);
};
class PointLightSource
{
public:
    Color color;
    Vec position;
    PointLightSource(Vec, Color);
};
class Sphere
{
public:
    Color color;
    double refl = 0.5;
    double refr = 0;
    double n = 1.5;
    Vec position;
    double radius;
    Sphere(Vec, double, Color);
    Sphere operator()(Vec, double, Color);
    double distanceToSphere(Vec);
    Vec normal(Vec);
    double intersectionDistance(Ray);
};
class Camera
{
public:
    Vec position = Vec();
    Vec direction;
    int width, height;
    double fovw = M_PI_4, fovh = M_PI_4;
    Camera(Vec, int, int);
    Camera(Vec, Vec, int, int, double, double);
    Ray generateRays(int, int);
};

Color trace(std::vector<Sphere>, std::vector<PointLightSource>, Ray, int);