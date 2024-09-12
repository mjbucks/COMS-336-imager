#include <iostream>

#include "color.h"
#include "vec3.h"
#include "ray.h"

bool isSphereHit(const point3& center, double radius, const ray& r){
    vec3 oc = center - r.origin();

    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;

    auto discriminant = b*b - 4*a*c;

    return (discriminant >= 0);
}

color ray_color(const ray& r) {
    if (isSphereHit(point3(0, 0, 1), 0.5, r))
        return color(1, 0, 0);

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);

    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
    auto aspect_ratio = 16.0 / 9.0;
    auto vp_height = 2.0;
    int focal_length = 1.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    auto vp_width = vp_height * (double(image_width)/image_height);
    auto camera_center = point3(0, 0, 0);

    auto vp_u = vec3(vp_width, 0, 0);
    auto vp_v = vec3(0, -vp_height, 0);

    auto pixel_delta_u = vp_u / image_width;
    auto pixel_delta_v = vp_v / image_height;

    auto vp_upper_left = camera_center - vec3(0, 0, focal_length) - vp_u/2 - vp_v/2;
    auto pixel00_loc = vp_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int i = 0; i < image_height; i++) {
        std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
        for(int j = 0; j < image_width; j++) {

            auto pixel_center = pixel00_loc + (j * pixel_delta_u) + (i * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rRender Complete! \n";
}