#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"

class camera {
    public:
        double aspectRatio = 1.0;  // Ratio of image width over height
        int    imageWidth  = 100;  // Rendered image width in pixel count

        void render(const hittable& world) {
            initialize();

            std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

            for (int j = 0; j < imageHeight; j++)
            {
                std::clog << "\rScanlines remaining:" << (imageHeight - j) << ' ' << std::flush;
                for (int i = 0; i < imageWidth; i++)
                {
                    auto pixelCenter = pixel00Loc + (i*pixelDeltaU) + (j*pixelDeltaV);
                    auto rayDirection = pixelCenter - center;
                    ray r(center, rayDirection);

                    color pixelColor = rayColor(r, world);
                    writeColor(std::cout, pixelColor);
                }
            }

            std::clog << "\rDone.                   \n";        
        }

    private:
        int imageHeight;
        point3 center;
        point3 pixel00Loc;
        vec3 pixelDeltaU;
        vec3 pixelDeltaV;

        void initialize() {
            imageHeight = int(imageWidth / aspectRatio);
            imageHeight = (imageHeight < 1) ? 1 : imageHeight;

            center = point3(0, 0, 0);
            auto focalLength = 1.0;
            auto viewportHeight = 2.0;
            auto viewportWidth = viewportHeight * (double(imageWidth)/imageHeight);
            auto cameraCenter = point3(0,0,0);

            // Calculate vectors along and down the viewport
            auto viewportU = vec3(viewportWidth, 0, 0);
            auto viewportV = vec3(0, -viewportHeight, 0);

            // Determine delta for mapping pixels onto viewport
            pixelDeltaU = viewportU / imageWidth;
            pixelDeltaV = viewportV / imageHeight;

            // Calculate upper left pixel location
            auto viewportUpperLeft = cameraCenter - vec3(0, 0, focalLength) - viewportU/2 - viewportV/2;
            pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

        }

        color rayColor(const ray& r, const hittable& world) const {
            hitRecord rec;

            if (world.hit(r, interval(0, infinity), rec)){
                return 0.5 * (rec.normal + color(1,1,1));
            }
            
            vec3 unitDirection = unitVector(r.direction());
            auto a = 0.5*(unitDirection.y() + 1.0);
            return (1.0-a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }
};

#endif