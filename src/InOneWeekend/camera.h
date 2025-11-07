#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"

class camera
{
public:
    double aspectRatio = 1.0; // Ratio of image width over height
    int imageWidth = 100;     // Rendered image width in pixel count
    int samplesPerPixel = 10; // Count of random samples for each pixel
    int maxDepth = 10;        // Maximum number of ray bounces in the scene

    void render(const hittable &world)
    {
        initialize();

        std::cout << "P3\n"
                  << imageWidth << " " << imageHeight << "\n255\n";

        for (int j = 0; j < imageHeight; j++)
        {
            std::clog << "\rScanlines remaining:" << (imageHeight - j) << ' ' << std::flush;
            for (int i = 0; i < imageWidth; i++)
            {
                color pixelColor(0, 0, 0);

                for (int sample = 0; sample < samplesPerPixel; sample++)
                {
                    ray r = getRay(i, j);
                    pixelColor += rayColor(r, maxDepth, world);
                }

                writeColor(std::cout, pixelSamplesScale * pixelColor);
            }
        }

        std::clog << "\rDone.                   \n";
    }

private:
    int imageHeight;
    double pixelSamplesScale; // Color scale factor for a sum of pixel samples
    point3 center;
    point3 pixel00Loc;
    vec3 pixelDeltaU;
    vec3 pixelDeltaV;

    void initialize()
    {
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        pixelSamplesScale = 1.0 / samplesPerPixel;

        center = point3(0, 0, 0);
        auto focalLength = 1.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
        auto cameraCenter = point3(0, 0, 0);

        // Calculate vectors along and down the viewport
        auto viewportU = vec3(viewportWidth, 0, 0);
        auto viewportV = vec3(0, -viewportHeight, 0);

        // Determine delta for mapping pixels onto viewport
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        // Calculate upper left pixel location
        auto viewportUpperLeft = cameraCenter - vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
        pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
    }

    ray getRay(int i, int j) const
    {

        auto offset = sampleSquare();
        auto pixelSample = pixel00Loc + ((i + offset.x()) * pixelDeltaU) + ((j + offset.y()) * pixelDeltaV);

        auto rayOrigin = center;
        auto rayDirection = pixelSample - rayOrigin;

        return ray(rayOrigin, rayDirection);
    }

    vec3 sampleSquare() const
    {
        return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
    }

    color rayColor(const ray &r, int depth, const hittable &world) const
    {
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        hitRecord rec;

        if (world.hit(r, interval(0.001, infinity), rec))
        {
            vec3 direction = rec.normal + randomUnitVector();
            return 0.5 * rayColor(ray(rec.p, direction), depth - 1, world);
        }

        vec3 unitDirection = unitVector(r.direction());
        auto a = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif