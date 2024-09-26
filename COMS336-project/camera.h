#ifndef CAMERA_H
#define CAMERA_H

#include "canbehit.h"

class camera {
    public: 

        double aspect_ratio = 1.0;
        int image_width = 100;

        void render(const canbehit& world) {
            init();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int i = 0; i < image_height; i++) {
                std::clog << "\rScanlines left: " << (image_height - i) << ' ' << std::flush;
                for (int k = 0; k < image_width; k++) {
                    auto pixel_center = pixel00_loc + (k * pixel_du) + (i * pixel_dv);

                    auto ray_dir = pixel_center - center;

                    ray r(center, ray_dir);

                    color pixel_color = ray_color(r, world);

                    write_color(std::cout, pixel_color);
                }
            }
            std::clog << "\rRender complete.";
        }
    
    private:

        int image_height;
        point3 center;
        point3 pixel00_loc;
        vec3 pixel_du;
        vec3 pixel_dv;

        void init() {

            image_height = int(image_width / aspect_ratio);

            image_height = (image_height < 1) ? 1 : image_height;

            center = point3(0, 0, 0);

            auto focul_len = 1.0;
            auto vp_height = 2.0;
            auto vp_width = vp_height * (double(image_width)/image_height);

            auto vp_u = vec3(vp_width, 0, 0);
            auto vp_v = vec3(0, -vp_height, 0);

            pixel_du = vp_u / image_width;
            pixel_dv = vp_v / image_height;

            auto vp_upper_left = center - vec3(0, 0, focul_len) - vp_u/2 - vp_v/2;
            pixel00_loc = vp_upper_left + 0.5 * (pixel_du + pixel_dv);

        }

        color ray_color(const ray& r, const canbehit& world) const {

            hit_record rec;

            if (world.hit(r, interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + color(1, 1, 1));
            }

            vec3 unit_dir = unit_vector(r.direction());

            auto a = 0.5*(unit_dir.y() + 1.0);

            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);

        }
};

#endif