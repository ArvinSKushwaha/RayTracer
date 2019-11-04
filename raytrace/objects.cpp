#include <any>
#include <iostream>
#include <cmath>
#include <cstring>
#include <array>
#include <vector>
#include "misc_func.hpp"
#include "objects.hpp"

#define COLOR_DECAY 1
#define BOUNCE_DEPTH 5
Vec3D Infinite = INF_VEC;

// Ray Class Function

Ray::Ray (Vec3D org, Vec3D dir)
{
    origin = org;
    direction = dir;
}

Ray Ray::reflect (Vec3D normal, Vec3D position)
{
    Vec3D Ri = this->direction;
    Vec3D Rf = Ri - normal * 2 * (Ri % normal);
    return Ray(position, Rf);
}

Ray Ray::refract (Vec3D normal, Vec3D position, double n1, double n2)
{
    Vec3D Ri = this->direction;
    const double n = n1/n2;
    const double cosI = -normal % Ri;
    const double sinT2 = n*n * (1 - cosI * cosI);
    if (sinT2 > 1.0) return Ray(Infinite, Infinite); // TOTAL INTERNAL REFLECTION
    const double cosT = sqrt(1.0 - sinT2);
    return Ray(position, (Ri * n) + normal * (n * cosI - cosT));

}

// Sphere Class Functions

Sphere::Sphere (Vec3D pos, double r, Color c)
{
    position = pos;
    radius = r;
    color = c;
}

double Sphere::distanceToSphere (Vec3D p)
{
    return distanceBetween(p, position);
}

Vec3D Sphere::normal (Vec3D p)
{
    return (p - position)/(p - position).norm();
}

double Sphere::intersectionDistance (Ray R)
{
    Vec3D D = R.direction;
    Vec3D A = R.origin;
    Vec3D C = position;
    double r = radius;
    Vec3D pc = C - A;
    if(pc.norm() <= r && (D % pc) <= 0)
    {
        return INFINITY;
    }
    double d = pc % D;
    double m2 = ((D * d) - pc) % ((D * d) - pc);
    double disc = (r*r - m2);
    if(disc < 0)
    {
        return INFINITY;
    }
    else
    {
        double d1 = d - sqrt(disc);
        double d2 = d + sqrt(disc);
        if(d1 < 0 && d2 < 0)
        {
            return INFINITY;
        }
        else if(d1 > 0)
        {
            return d1;
        }
        else if(d2 == d1 && d2 > 0)
        {
            return d2;
        }
        else if(d1 < 0 && d2 > 0)
        {
            return d2;
        }
        else{
            return NAN;
        }
    }
}

// Raytracing Functions

Color diffuseTrace (std::vector<Sphere> scene, std::vector<PointLightSource> lightSources, Ray R, int depth)
{
    int i = 0;
    double min = INFINITY;
    int min_index;
    bool hitLight = false;
    do
    {
        double dist = scene[i].intersectionDistance(R);
        min = (dist < min) ? dist : min;
        if(min == dist){
            min_index = i;
        }
        i++;
    } while (i < scene.size());
    if(min == INFINITY)
    {
        return Color();
    }
    Vec3D collisionPoint3D = R.origin + R.direction * min;
    Vec3D normCollision = scene[min_index].normal(collisionPoint3D);
    Color c = Color();
    for(int i = 0; i < lightSources.size(); i++)
    {
        double brightness = Vec3D::normalize(lightSources[i].position - scene[min_index].position) % normCollision;
        c.r = clamp(c.r + brightness * sqrt(lightSources[i].color.r * scene[min_index].color.r), 0, 255);
        c.g = clamp(c.g + brightness * sqrt(lightSources[i].color.g * scene[min_index].color.g), 0, 255);
        c.b = clamp(c.b + brightness * sqrt(lightSources[i].color.b * scene[min_index].color.b), 0, 255);
    }
    if(depth < BOUNCE_DEPTH)
    {
        Color bounce = diffuseTrace(scene, lightSources, R.reflect(normCollision, collisionPoint3D), depth + 1);
        Ray refrRay = Ray(Infinite, Infinite);
        if(normCollision % R.direction > 0)
        {
            refrRay = R.refract(normCollision, collisionPoint3D, 1, scene[min_index].n);
        }
        else
        {
            refrRay = R.refract(normCollision, collisionPoint3D, scene[min_index].n, 1);
        }
        if(refrRay.origin == Infinite && refrRay.direction == Infinite)
        {
            return Color::mixColor(bounce, c, COLOR_DECAY);
        }
        Color refr = diffuseTrace(scene, lightSources, refrRay, depth + 1);
        return c; Color::mixColor(Color::mixColor(bounce, refr, scene[min_index].reflrefr), c, COLOR_DECAY);
    }
    return c;
}

// Camera Class Functions

Camera::Camera (Vec3D dir, int w, int h)
{
    direction = dir;
    width = w;
    height = h;
}

Camera::Camera (Vec3D pos, Vec3D dir, int w, int h, double fovw_, double fovh_)
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
    double angleXZ = map(x, 0, width, -fovw/2, fovw/2);
    double angleYZ = map(y, 0, height, -fovh/2, fovh/2);
    std::vector< std::vector<double> > Rx = {{1, 0, 0}, {0, cos(angleYZ), -sin(angleYZ)}, {0, sin(angleYZ), cos(angleYZ)}};
    std::vector< std::vector<double> > Ry = {{cos(angleXZ), 0, sin(angleXZ)}, {0, 1, 0}, {-sin(angleXZ), 0, cos(angleXZ)}};

    Vec3D direction_ = direction.transform(Rx);
    direction_ = direction_.transform(Ry);

    return Ray(position, direction_);
}

// PointLightSource Class Functions

PointLightSource::PointLightSource (Vec3D a, Color c)
{
    position = a;
    color = c;
}