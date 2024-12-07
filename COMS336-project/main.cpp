#include <iostream>

#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "commons.h"
#include "canbehit.h"
#include "canbehit_list.h"
#include "material.h"
#include "sphere.h"
#include "interval.h"
#include "texture.h"
#include "quad.h"
#include "triangle.h"
#include "mesh.h"

void bouncing_spheres() {
    canbehit_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = canbehit_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);
}

void checkered_spheres() {
    canbehit_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void max_pizza() {
    auto max_pizza_texture = make_shared<image_texture>("max_pizza.jpg");
    auto max_pizza_surface = make_shared<lambertian>(max_pizza_texture);
    auto max_pizza_spehere = make_shared<sphere>(point3(0,0,-50), 10, max_pizza_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,12);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(canbehit_list(max_pizza_spehere));
}

void perlin_spheres() {
    canbehit_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void quads() {
    canbehit_list world;

    // Materials
    auto max_pizza_texture = make_shared<image_texture>("max_pizza.jpg");
    auto max_pizza_surface = make_shared<lambertian>(max_pizza_texture);
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), max_pizza_surface));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void simple_light() {
    canbehit_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    world.add(make_shared<sphere>(point3(0,7,0), 2, difflight));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_box() {
    canbehit_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(255, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<canbehit> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<canbehit> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_smoke() {
    canbehit_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    world.add(make_shared<quad>(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<canbehit> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<canbehit> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    world.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    world.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    canbehit_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(box(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    canbehit_list world;

    world.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    world.add(make_shared<quad>(point3(123,554,147), vec3(300,0,0), vec3(0,0,265), light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0,0,0), 5000, make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("max_pizza.jpg"));
    world.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.2);
    world.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    canbehit_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    world.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2), 15),
            vec3(-100,270,395)
        )
    );

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = image_width;
    cam.samples_per_pixel = samples_per_pixel;
    cam.max_depth         = max_depth;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(478, 278, -600);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void triangle_test() {
    canbehit_list world;

    // Create materials with more vibrant colors
    auto purple = make_shared<lambertian>(color(0.8, 0.3, 0.9));
    auto orange = make_shared<lambertian>(color(0.9, 0.4, 0.1));
    auto teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));
    auto gold = make_shared<metal>(color(0.8, 0.6, 0.2), 0.3);
    auto light = make_shared<diffuse_light>(color(4, 4, 4));

    // Create several triangles in different positions and orientations
    world.add(make_shared<triangle>(
        point3(-2, 0, -2),
        point3(-1, 0, -2),
        point3(-1.5, 2, -1.5),
        purple
    ));
    
    world.add(make_shared<triangle>(
        point3(1, 0, -1),
        point3(2, 0, -1),
        point3(1.5, 1.5, 0),
        orange
    ));
    
    world.add(make_shared<triangle>(
        point3(-1, 0, 1),
        point3(0, 0, 2),
        point3(-0.5, 2, 1.5),
        teal
    ));

    world.add(make_shared<triangle>(
        point3(0.5, 0.5, 0),
        point3(1.5, 0.5, 0),
        point3(1, 2, 1),
        gold
    ));

    // Add two light sources for better illumination
    world.add(make_shared<quad>(point3(-2, 5, -2), vec3(4,0,0), vec3(0,0,4), light));
    world.add(make_shared<quad>(point3(2, 4, 2), vec3(-3,0,0), vec3(0,0,-3), light));

    // Add a dark ground plane
    auto dark_gray = make_shared<lambertian>(color(0.2, 0.2, 0.2));
    world.add(make_shared<quad>(point3(-10, -0.01, -10), vec3(20,0,0), vec3(0,0,20), dark_gray));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    
    // Darker background
    cam.background        = color(0.1, 0.1, 0.15);

    cam.vfov     = 45;
    cam.lookfrom = point3(4, 3, 4);
    cam.lookat   = point3(0, 1, 0);
    cam.vup      = vec3(0,1,0);

    // Add some depth of field for artistic effect
    cam.defocus_angle = 0.4;
    cam.focus_dist    = 5.5;

    cam.render(world);
}

void mesh_test() {
    canbehit_list world;

    // Create materials
    auto red_metal = make_shared<metal>(color(0.7, 0.3, 0.3), 0.1);
    auto blue_diffuse = make_shared<lambertian>(color(0.2, 0.3, 0.7));
    auto ground_material = make_shared<lambertian>(color(0.2, 0.2, 0.2));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    // Load meshes
    // Assuming you have a models directory with these files
    world.add(make_shared<mesh>("meshes/Nefertiti.obj", red_metal));
    
    // Add a light source above
    world.add(make_shared<quad>(point3(-1, 8, -1), vec3(2,0,0), vec3(0,0,2), light));

    // Add a ground plane
    world.add(make_shared<quad>(point3(-15, -2, -15), vec3(30,0,0), vec3(0,0,30), ground_material));

    camera cam;

    // Camera settings for better visualization
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 250;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.1, 0.1, 0.15);  // Dark background

    // Position camera to see the meshes clearly
    cam.vfov     = 40;
    cam.lookfrom = point3(6, 4, 6);
    cam.lookat   = point3(0, 0, 0);
    cam.vup      = vec3(0,1,0);

    // Add slight depth of field for artistic effect
    cam.defocus_angle = 0.2;
    cam.focus_dist    = 10.0;

    cam.render(world);
}

void final_iowa_state_scene() {
    canbehit_list world;

    // Materials
    auto neon_red = make_shared<diffuse_light>(color(15, 0.2, 0.2));
    auto glass = make_shared<dielectric>(1.5);
    auto chrome = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);
    auto wood = make_shared<lambertian>(color(0.6, 0.3, 0.15));
    auto bar_surface = make_shared<metal>(color(0.4, 0.4, 0.4), 0.1);
    
    // Create perlin noise for atmospheric effect
    auto smoke_texture = make_shared<noise_texture>(4);
    
    // Bar counter (large quad with metallic surface)
    world.add(make_shared<quad>(point3(-8, 0, -4), vec3(16,0,0), vec3(0,0,8), bar_surface));
    
    // Back wall with subtle texture
    auto wall_texture = make_shared<noise_texture>(2);
    world.add(make_shared<quad>(point3(-8, 0, 5), vec3(16,0,0), vec3(0,8,0), 
        make_shared<lambertian>(wall_texture)));
    
    // Iowa State logo components (using quads for neon effect)
    // Left: "I" shape
    world.add(make_shared<quad>(point3(3.5, 2, 4.5), vec3(0.5,0,0), vec3(0,3,0), neon_red));
    
    // Middle: "S" shape (improved connections)
    // Top horizontal
    world.add(make_shared<quad>(point3(1.75, 4.5, 4.5), vec3(-1.5,0,0), vec3(0,0.4,0), neon_red));
    // Top vertical
    world.add(make_shared<quad>(point3(1.75, 4.5, 4.5), vec3(-0.4,0,0), vec3(0,-1.2,0), neon_red));
    // Middle horizontal
    world.add(make_shared<quad>(point3(1.75, 3.3, 4.5), vec3(-1.5,0,0), vec3(0,0.4,0), neon_red));
    // Bottom vertical
    world.add(make_shared<quad>(point3(0.25, 3.3, 4.5), vec3(-0.4,0,0), vec3(0,-1.2,0), neon_red));
    // Bottom horizontal
    world.add(make_shared<quad>(point3(1.75, 2.1, 4.5), vec3(-1.5,0,0), vec3(0,0.4,0), neon_red));
    
    // Right: "U" shape
    world.add(make_shared<quad>(point3(-1, 4.5, 4.5), vec3(-0.5,0,0), vec3(0,-2.4,0), neon_red));  // Left vertical
    world.add(make_shared<quad>(point3(-1, 2.1, 4.5), vec3(-1.5,0,0), vec3(0,0.4,0), neon_red));   // Bottom
    world.add(make_shared<quad>(point3(-2.5, 4.5, 4.5), vec3(-0.5,0,0), vec3(0,-2.4,0), neon_red)); // Right vertical
    
    // Add volumetric fog effect in front of the neon
    world.add(make_shared<constant_medium>(
        make_shared<sphere>(point3(0, 3, 4.8), 4, shared_ptr<material>()),
        0.01, color(0.8, 0.8, 0.8)
    ));
    
    // Add some glass decorations (some moving, some stationary)
    for(int i = 0; i < 5; i++) {
        auto center1 = point3(-6 + i*3, 0.5, 0);
        if (i % 2 == 0) {
            world.add(make_shared<sphere>(center1, 0.4, glass));  // Stationary
        } else {
            auto center2 = center1 + vec3(0, 0.3, 0);
            world.add(make_shared<sphere>(center1, center2, 0.4, glass));  // Moving
        }
    }
    
    // Add some moving spherical ice cubes in the glasses
    for(int i = 0; i < 5; i++) {
        auto center1 = point3(-5.8 + i*3, 0.8, 0);
        auto center2 = center1 + vec3(0.3, 0.2, 0.1);
        world.add(make_shared<sphere>(center1, center2, 0.2, chrome));
    }
    
    // Add some ambient lighting
    auto dim_light = make_shared<diffuse_light>(color(0.6, 0.6, 0.6));
    world.add(make_shared<quad>(point3(-8, 8, -4), vec3(16,0,0), vec3(0,0,12), dim_light));
    
    // Camera settings for better view
    camera cam;
    
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 800;  // Higher resolution for final render
    cam.samples_per_pixel = 500;  // More samples for better quality
    cam.max_depth         = 50;
    cam.background        = color(0.02, 0.02, 0.02);  // Very dark background
    
    // Position camera to see everything
    cam.vfov     = 60;  // Wider field of view
    cam.lookfrom = point3(0, 3, -10);  // Moved back and up
    cam.lookat   = point3(0, 3, 4.5);  // Looking at the center of the scene
    cam.vup      = vec3(0,1,0);
    
    // Reduced depth of field effect
    cam.defocus_angle = 0.2;
    cam.focus_dist    = 10.0;
    
    cam.render(world);
}

int main() {
    switch (11) {
        case 1:  bouncing_spheres();   break;
        case 2:  checkered_spheres();  break;
        case 3:  max_pizza();          break;
        case 4:  perlin_spheres();     break;
        case 5:  quads();              break;
        case 6:  simple_light();       break;
        case 7:  cornell_box();        break;
        case 8:  triangle_test();      break;
        case 9:  cornell_smoke();      break;
        case 10: mesh_test();          break;
        case 11: final_iowa_state_scene(); break;
        case 12: final_scene(800, 10000, 40); break;
        default: final_scene(400,   250,  4); break;
    }
}