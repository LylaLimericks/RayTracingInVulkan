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

            if (scatterDirection.nearZero()) {
                scatterDirection = rec.normal;
            }

            scattered = ray(rec.p, scatterDirection);
            attentuation = albedo;
            return true;
        }

    private:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& albedo, double roughness) : albedo(albedo), roughness(roughness) {}

        bool scatter(const ray& rIn, const hitRecord& rec, color& attentuation, ray& scattered) const override {
            vec3 reflected = reflect(rIn.direction(), rec.normal);
            reflected = unitVector(reflected) + (roughness * randomUnitVector());
            scattered = ray(rec.p, reflected);
            attentuation = albedo;

            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double roughness;
};

#endif