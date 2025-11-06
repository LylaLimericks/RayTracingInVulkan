#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "interval.h"

class hitRecord {
    public:
        point3 p;
        vec3 normal;
        double t;
        bool frontFace;

        void setFaceNormal(const ray& r, const vec3& outwardNormal) {
            // Sets the hit record normal vector
            // Note: we are assuming that outwardNormal is a UNIT vector

            frontFace = dot(r.direction(), outwardNormal) < 0;
            normal = frontFace ? outwardNormal : -outwardNormal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval tInterval, hitRecord& rec) const = 0;
};

#endif