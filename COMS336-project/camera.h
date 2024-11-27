#ifndef CAMERA_H
#define CAMERA_H

#include "canbehit.h"
#include "material.h"

class camera {
    public: 

        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10;
        int max_depth = 10;

        double vfov = 90;

        point3 lookfrom = point3(0, 0, 0);
        point3 lookat = point3(0, 0, -1);
        vec3 vup = vec3(0, 1, 0);

        double defocus_angle = 0;
        double focus_dist = 10;

        void render(const canbehit& world) {
            init();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int i = 0; i < image_height; i++) {
                std::clog << "\rScanlines left: " << (image_height - i) << ' ' << std::flush;
                for (int k = 0; k < image_width; k++) {
                    color pixel_color(0, 0, 0);

                    for(int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(k, i);
                        pixel_color += ray_color(r, max_depth, world);
                    }

                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }
            std::clog << "\rRender complete.";
        }
    
    private:

        int image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel00_loc;
        vec3 pixel_du;
        vec3 pixel_dv;
        vec3 u, v, w;
        vec3 defocus_disk_u;
        vec3 defocus_disk_v;

        void init() {

            image_height = int(image_width / aspect_ratio);

            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto vp_height = 2 * h * focus_dist;
            auto vp_width = vp_height * (double(image_width)/image_height);

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            auto vp_u = vp_width * u;
            auto vp_v = vp_height * -v;

            pixel_du = vp_u / image_width;
            pixel_dv = vp_v / image_height;

            auto vp_upper_left = center - (focus_dist * w) - vp_u/2 - vp_v/2;
            pixel00_loc = vp_upper_left + 0.5 * (pixel_du + pixel_dv);

            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));

            defocus_disk_u = u * defocus_radius;

            defocus_disk_v = v * defocus_radius;

        }

        ray get_ray(int k, int i) {
            auto offset = sample_square();

            auto pixel_sample = pixel00_loc + ((k + offset.x()) * pixel_du) + ((i + offset.y()) * pixel_dv);

            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();;
            auto ray_dir = pixel_sample - ray_origin;

            auto ray_time = random_double();

            return ray(ray_origin, ray_dir, ray_time);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color ray_color(const ray& r, int depth, const canbehit& world) const {

            if (depth <= 0)
                return color(0, 0, 0);

            hit_record rec;

            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                color att;

                if (rec.mat->scatter(r, rec, att, scattered))
                    return att * ray_color(scattered, depth-1, world);
                
                return color(0, 0, 0);
                
            }

            vec3 unit_dir = unit_vector(r.direction());

            auto a = 0.5*(unit_dir.y() + 1.0);

            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);

        }
};

#endif