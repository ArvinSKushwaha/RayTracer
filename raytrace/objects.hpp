#include <any>
#include <iostream>
#include <cmath>
#include <cstring>
#include <array>
#include <vector>
#include "colors.hpp"

class Ray
{
public:
    Vec3D origin;
    Vec3D direction;
    Color color = Color(NAN, NAN, NAN);
    Ray(Vec3D, Vec3D);
    Ray reflect(Vec3D, Vec3D);
    Ray refract(Vec3D, Vec3D, double n1, double n2);
};
class PointLightSource
{
public:
    Color color;
    Vec3D position;
    PointLightSource(Vec3D, Color);
};
class Sphere
{
public:
    Color color;
    double refl = 0;
    double refr = 1;
    double n = 1.5;
    Vec3D position;
    double radius;
    Sphere(Vec3D, double, Color);
    Sphere operator()(Vec3D, double, Color);
    double distanceToSphere(Vec3D);
    Vec3D normal(Vec3D);
    double intersectionDistance(Ray);
};
class Camera
{
public:
    Vec3D position = Vec3D();
    Vec3D direction;
    int width, height;
    double fovw = M_PI_4, fovh = M_PI_4;
    Camera(Vec3D, int, int);
    Camera(Vec3D, Vec3D, int, int, double, double);
    Ray generateRays(int, int);
};

Color trace(std::vector<Sphere>, std::vector<PointLightSource>, Ray, int);