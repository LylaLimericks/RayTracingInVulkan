#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "hittable.h"
#include "ray.h"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const {
            return false;
        }
};

class lambertian : public material {
    public:
        lambertian(const color& albedo): albedo(albedo) {}


        bool scatter(const ray& rIn, const hitRecord& rec, color& attentuation, ray& scattered) const override {
            auto scatterDirection = rec.normal + randomUnitVector();
            scattered = ray(rec.p, scatterDirection);
            attentuation = albedo;
            return true;
        }

    private:
        color albedo;
};

#endif