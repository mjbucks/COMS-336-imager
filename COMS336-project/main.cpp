#include <iostream>

#include "camera.h"
#include "commons.h"
#include "canbehit.h"
#include "canbehit_list.h"
#include "sphere.h"
#include "interval.h"

int main() {
    canbehit_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1000;
    cam.samples_per_pixel = 100;

    cam.render(world);
}