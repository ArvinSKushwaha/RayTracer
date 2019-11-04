#include <any>
#include <iostream>
#include <cmath>
#include <cstring>
#include <array>
#include <vector>
#include "vectors.hpp"

// Vector Class Functions

Vec3D::Vec3D(double x_, double y_, double z_)
{
    x = x_;
    y = y_;
    z = z_;
}

Vec3D::Vec3D()
{
    x = 0;
    y = 0;
    z = 0;
}

Vec3D Vec3D::operator-=(Vec3D n)
{
    this->x - n.x;
    this->y - n.y;
    this->z - n.z;
    return *this;
}

Vec3D Vec3D::operator+=(Vec3D n)
{
    this->x + n.x;
    this->y + n.y;
    this->z + n.z;
    return *this;
}

Vec3D Vec3D::operator-(Vec3D n)
{
    return add(n * -1);
}

Vec3D Vec3D::operator-()
{
    return *this * -1;
}

Vec3D Vec3D::operator+(Vec3D n)
{
    return add(n);
}

Vec3D Vec3D::operator*(Vec3D n)
{
    return multiply(n);
}

Vec3D Vec3D::operator*(double n)
{
    return multiply(n);
}

Vec3D Vec3D::operator/(double n)
{
    return multiply(1.0 / n);
}

double Vec3D::operator%(Vec3D n)
{
    return dot(n);
}

Vec3D Vec3D::operator()(double x, double y, double z)
{
    Vec3D d = Vec3D(x, y, z);
    return d;
}

bool Vec3D::operator==(Vec3D n)
{
    return n.x == x && n.y == y && n.z == z;
}

double Vec3D::dot(Vec3D n)
{
    return x * n.x + y * n.y + z * n.z;
}

Vec3D Vec3D::cross(Vec3D n)
{
    return Vec3D(y * n.z - z * n.y, z * n.x - x * n.z, x * n.y - y * n.x);
}

Vec3D Vec3D::add(Vec3D n)
{
    return Vec3D(x + n.x, y + n.y, z + n.z);
}

Vec3D Vec3D::add(double n)
{
    return Vec3D(x + n, y + n, z + n);
}

Vec3D Vec3D::multiply(Vec3D n)
{
    return Vec3D(x * n.x, y * n.y, z * n.z);
}

Vec3D Vec3D::multiply(double n)
{
    return Vec3D(x * n, y * n, z * n);
}

std::string Vec3D::cstr()
{
    return "x: " + std::to_string(x) + "\r\ny: " + std::to_string(y) + "\r\nz: " + std::to_string(z);
}

std::string Vec3D::cstr(Vec3D n)
{
    return "x: " + std::to_string(n.x) + "\r\ny: " + std::to_string(n.y) + "\r\nz: " + std::to_string(n.z);
}

Vec3D Vec3D::sinVec(Vec3D v)
{
    return Vec3D(sin(v.x), sin(v.y), sin(v.z));
}

Vec3D Vec3D::cosVec(Vec3D v)
{
    return Vec3D(cos(v.x), cos(v.y), cos(v.z));
}

Vec3D Vec3D::tanVec(Vec3D v)
{
    return Vec3D(tan(v.x), tan(v.y), tan(v.z));
}

double Vec3D::norm(Vec3D n)
{
    return sqrt(n.dot(n));
}

double Vec3D::norm()
{
    return sqrt(x * x + y * y + z * z);
}

Vec3D Vec3D::normalize(Vec3D n)
{
    return n / n.norm();
}

Vec3D Vec3D::normalize()
{
    double l = norm();
    x /= l;
    y /= l;
    z /= l;
    return *this;
}

Vec3D Vec3D::transform (std::vector< std::vector<double> > matrix)
{
    double x_ = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
    double y_ = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
    double z_ = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
    return Vec3D(x_, y_, z_);
}