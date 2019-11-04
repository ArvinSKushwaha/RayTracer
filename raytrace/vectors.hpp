#include <any>
#include <iostream>
#include <cmath>
#include <cstring>
#include <array>
#include <vector>

#define INF_VEC Vec3D(INFINITY, INFINITY, INFINITY)

class Vec3D
{
    public:
        double x, y, z;
        Vec3D (double, double, double);
        Vec3D ();
        double dot (Vec3D);
        Vec3D cross (Vec3D);
        Vec3D add (Vec3D);
        Vec3D add (double);
        Vec3D multiply (Vec3D);
        Vec3D multiply (double);
        Vec3D operator() (double, double, double);
        Vec3D operator-= (Vec3D);
        Vec3D operator+= (Vec3D);
        Vec3D operator- (Vec3D);
        Vec3D operator- ();
        Vec3D operator+ (Vec3D);
        Vec3D operator* (Vec3D);
        Vec3D operator* (double);
        Vec3D operator/ (double);
        double operator% (Vec3D);
        bool operator== (Vec3D);
        std::string cstr();
        static std::string cstr(Vec3D);
        static Vec3D sinVec (Vec3D);
        static Vec3D cosVec (Vec3D);
        static Vec3D tanVec (Vec3D);
        static double norm (Vec3D);
        double norm ();
        Vec3D normalize ();
        static Vec3D normalize (Vec3D);
        Vec3D transform (std::vector< std::vector<double> >);
};