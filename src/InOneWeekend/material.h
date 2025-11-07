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


        bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override {
            auto scatterDirection = rec.normal + randomUnitVector();

            if (scatterDirection.nearZero()) {
                scatterDirection = rec.normal;
            }

            scattered = ray(rec.p, scatterDirection);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& albedo, double roughness) : albedo(albedo), roughness(roughness) {}

        bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(rIn.direction(), rec.normal);
            reflected = unitVector(reflected) + (roughness * randomUnitVector());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;

            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double roughness;
};

class dielectric : public material {
    public:
        dielectric(double refractiveIndex) : refractiveIndex(refractiveIndex) {}

        bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.frontFace ? (1.0 / refractiveIndex) : refractiveIndex;

            vec3 unitDirection = unitVector(rIn.direction());
            vec3 refracted = refract(unitDirection, rec.normal, ri);

            scattered = ray(rec.p, refracted);

            return true;
        }

    private:
        double refractiveIndex;
};

#endif