#include <any>
#include <iostream>
#include <cmath>
#include <cstring>
#include <array>
#include <vector>
#include <random>
#include <eigen3/Eigen/Core>
#include "misc_func.hpp"
#include "objects.hpp"

#define COLOR_DECAY 1
#define BOUNCE_DEPTH 5
Vec Infinite = Vec(INFINITY, INFINITY, INFINITY);

// Ray Class Function

Ray::Ray(Vec org, Vec dir)
{
    origin = org;
    direction = dir;
}

Ray Ray::reflect(Vec normal, Vec position)
{
    Vec Ri = this->direction;
    Vec Rf = Ri - (normal * 2 * Ri.dot(normal));
    return Ray(position, Rf);
}

Ray Ray::refract(Vec normal, Vec position, double n1, double n2)
{
    Vec Ri = this->direction;
    const double n = n1 / n2;
    const double cosI = -normal.dot(Ri);
    const double sinT2 = n * n * (1 - cosI * cosI);
    if (sinT2 > 1.0)
    {
        return Ray(Infinite, Infinite); // TOTAL INTERNAL REFLECTION
    }
    const double cosT = sqrt(1.0 - sinT2);
    return Ray(position, (Ri * n) + normal * (n * cosI - cosT));
}

// Sphere Class Functions

Sphere::Sphere(Vec pos, double r, Color c)
{
    position = pos;
    radius = r;
    color = c;
}

double Sphere::distanceToSphere(Vec p)
{
    return distanceBetween(p, position);
}

Vec Sphere::normal(Vec p)
{
    return (p - position) / (p - position).norm();
}

double Sphere::intersectionDistance(Ray R)
{
    Vec D = R.direction;
    Vec A = R.origin;
    Vec C = position;
    double r = radius;
    Vec pc = C - A;
    if (pc.norm() <= r && D.dot(pc) <= 0)
    {
        return INFINITY;
    }
    double d = pc.dot(D);
    double m2 = ((D * d) - pc).dot((D * d) - pc);
    double disc = (r * r - m2);
    if (disc < 0)
    {
        return INFINITY;
    }
    else
    {
        double d1 = d - sqrt(disc);
        double d2 = d + sqrt(disc);
        if (d1 < 0.01 && d2 < 0.01)
        {
            return INFINITY;
        }
        else if (d1 > 0.01)
        {
            return d1;
        }
        else if (abs(d2 - d1) < 0.01 && d2 > 0.01)
        {
            return d2;
        }
        else if (d1 < 0.01 && d2 > 0.01)
        {
            return d2;
        }
        else
        {
            return NAN;
        }
    }
}

// Raytracing Functions

Color trace(std::vector<Sphere> scene, std::vector<PointLightSource> lightSources, Ray R, int depth)
{
    int i = 0;
    double min = INFINITY;
    int min_index;
    bool hitLight = false;
    do
    {
        double dist = scene[i].intersectionDistance(R);
        min = (dist < min) ? dist : min;
        if (min == dist)
        {
            min_index = i;
        }
        i++;
    } while (i < scene.size());
    if (min == INFINITY)
    {
        return Color(100, 100, 100);
    }
    Vec collisionPoint3D = R.origin + R.direction * min;
    Vec normCollision = scene[min_index].normal(collisionPoint3D);
    Color c = Color();
    for (int i = 0; i < lightSources.size(); i++)
    {
        double brightness = (lightSources[i].position - scene[min_index].position).normalized().dot(normCollision);
        c.r = clamp(c.r + brightness * davg(lightSources[i].color.r, scene[min_index].color.r), 0, 255);
        c.g = clamp(c.g + brightness * davg(lightSources[i].color.g, scene[min_index].color.g), 0, 255);
        c.b = clamp(c.b + brightness * davg(lightSources[i].color.b, scene[min_index].color.b), 0, 255);
    }
    if (depth < BOUNCE_DEPTH)
    {
        if (scene[min_index].refl > 0)
        {
            Ray newRay = R.reflect(normCollision, collisionPoint3D);
            Color reflColor = trace(scene, lightSources, newRay, depth + 1);
            c = Color::mixColor(c, reflColor, scene[min_index].refl);
        }
        if (scene[min_index].refr > 0)
        {
            if (normCollision.dot(R.direction) < 0)
            {
                Ray newRay = R.refract(normCollision, collisionPoint3D, 1, scene[min_index].n);
                if (!(newRay.direction == Infinite))
                {
                    Color refrColor = trace(scene, lightSources, newRay, depth + 1);
                    c = Color::mixColor(c, refrColor, scene[min_index].refr);
                }
            }
            else
            {
                Ray newRay = R.refract(normCollision, collisionPoint3D, scene[min_index].n, 1);
                if (!(newRay.direction == Infinite))
                {
                    Color refrColor = trace(scene, lightSources, newRay, depth + 1);
                    c = Color::mixColor(c, refrColor, scene[min_index].refr);
                }
            }
        }
    }
    return c;
}

// Camera Class Functions

Camera::Camera(Vec dir, int w, int h)
{
    direction = dir;
    width = w;
    height = h;
}

Camera::Camera(Vec pos, Vec dir, int w, int h, double fovw_, double fovh_)
{
    position = pos;
    direction = dir;
    width = w;
    height = h;
    fovw = fovw_;
    fovh = fovh_;
}

Ray Camera::generateRays(int x, int y)
{
    double angleXZ = map(x, 0, width, -fovw / 2, fovw / 2) + (pow(((float)rand()) / ((float)RAND_MAX), 2) - 0.5) * fovw / width;
    double angleYZ = map(y, 0, height, -fovh / 2, fovh / 2) + (pow(((float)rand()) / ((float)RAND_MAX), 2) - 0.5) * fovh / height;
    Eigen::Matrix3d Rx;
    Rx << 1, 0, 0, 0, cos(angleYZ), -sin(angleYZ), 0, sin(angleYZ), cos(angleYZ);
    Eigen::Matrix3d Ry;
    Ry << cos(angleXZ), 0, sin(angleXZ), 0, 1, 0, -sin(angleXZ), 0, cos(angleXZ);

    Vec direction_ = Rx * direction;
    direction_ = Ry * direction_;

    return Ray(position, direction_);
}

// PointLightSource Class Functions

PointLightSource::PointLightSource(Vec a, Color c)
{
    position = a;
    color = c;
}