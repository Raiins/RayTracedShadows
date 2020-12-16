

/*
Source Code Taken From:
[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
*/

#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, double r) : center(cen), radius(r) {};
    //__________________________________________________________________________________________________
    //Changed this constructor to use color instead of material because for this project we did not need materials and materials really slowed us down
    sphere(point3 cen, double r,  color c) : center(cen), radius(r), color_ptr(c){};
    //__________________________________________________________________________________________________

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    point3 center;
    double radius;
    //__________________________________________________________________________________________________
    //Color public variable
    color color_ptr;
    //__________________________________________________________________________________________________
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;
    //__________________________________________________________________________________________________
    //Changed the hit_record pointer to also include the color of the sphere
    rec.color_ptr = color_ptr;
    //__________________________________________________________________________________________________
    return true;
}

#endif