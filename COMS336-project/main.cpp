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

void setup_common_scene(canbehit_list& world) {
    // Materials
    auto chrome = make_shared<metal>(color(0.8, 0.8, 0.8), 0.1);
    auto glass = make_shared<dielectric>(1.5);
    auto red_matte = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto light = make_shared<diffuse_light>(color(4, 4, 4));
    
    // Main objects
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, glass));        // Center glass sphere
    world.add(make_shared<sphere>(point3(-2, 0.5, 1), 0.5, chrome));     // Left metal sphere
    world.add(make_shared<sphere>(point3(2, 0.5, -1), 0.5, red_matte)); // Right red sphere
    
    // Light source
    world.add(make_shared<quad>(point3(-1, 4, -1), vec3(2,0,0), vec3(0,0,2), light));
    
    // Ground plane
    auto checker = make_shared<checker_texture>(0.5, color(.2, .3, .1), color(.9, .9, .9));
    auto ground = make_shared<lambertian>(checker);
    world.add(make_shared<quad>(point3(-5, 0, -5), vec3(10,0,0), vec3(0,0,10), ground));
}

void figure_2() {
    canbehit_list world;
    setup_common_scene(world);

    camera cam;

    // Basic camera settings
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 4;      // Low sample count
    cam.max_depth = 5;              // Few ray bounces
    
    // Basic positioning
    cam.vfov = 80;                  // Wide angle lens
    cam.lookfrom = point3(-2,2,6);  // Further back
    cam.lookat = point3(0,0,0);     // Looking at scene center
    cam.vup = vec3(0,1,0);

    // No depth of field
    cam.defocus_angle = 0;          // Everything in focus
    cam.focus_dist = 10;

    // Basic background
    cam.background = color(0.7, 0.8, 1.0);

    cam.render(world);
}

void figure_3() {
    canbehit_list world;
    setup_common_scene(world);

    camera cam;

    // Enhanced settings
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;    // High sample count for smooth edges
    cam.max_depth = 50;             // More bounces for better reflections
    
    // Better composition
    cam.vfov = 40;                  // Narrower angle for less distortion
    cam.lookfrom = point3(-2,2,4);  // Closer, more intimate view
    cam.lookat = point3(0,0.5,0);   // Looking at sphere center
    cam.vup = vec3(0,1,0);

    // Artistic depth of field
    cam.defocus_angle = 4.0;        // Subtle background blur
    cam.focus_dist = 4.5;           // Focused on main sphere

    // Dramatic background
    cam.background = color(0.1, 0.1, 0.2);

    cam.render(world);
}

void figure_4() {
    // Scene setup
    canbehit_list world;

    // Single sphere with basic material
    auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.3));  // Simple red diffuse
    world.add(make_shared<sphere>(point3(0, 0, 0), 1.0, sphere_material));

    // Add simple light source
    auto light = make_shared<diffuse_light>(color(15, 15, 15));  // Increased light intensity
    world.add(make_shared<quad>(point3(-2, 2, -2), vec3(4,0,0), vec3(0,0,4), light));

    // Camera setup - minimal settings
    camera cam;

    // Basic image settings
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;    // Added some samples for better lighting
    cam.max_depth = 50;           // Increased for proper light bounces

    // Simple camera position
    cam.vfov = 90;
    cam.lookfrom = point3(0, 0, -3);  // Looking straight at sphere
    cam.lookat = point3(0, 0, 0);     // Looking at center
    cam.vup = vec3(0, 1, 0);

    // No depth of field
    cam.defocus_angle = 0;

    // Simple background
    cam.background = color(0.7, 0.7, 0.7);  // Gray background

    // Render
    cam.render(world);
}

void figure_5() {
    // Scene setup
    canbehit_list world;

    // Single quad with basic material
    auto quad_material = make_shared<lambertian>(color(0.3, 0.7, 0.3));  // Simple green diffuse
    world.add(make_shared<quad>(point3(-1, -1, 0),     // Lower left corner
                               vec3(2, 0, 0),           // Width vector (2 units wide)
                               vec3(0, 2, 0),           // Height vector (2 units tall)
                               quad_material));

    // Add simple light source
    auto light = make_shared<diffuse_light>(color(5, 5, 5));
    world.add(make_shared<quad>(point3(-2, 2, -2), vec3(4,0,0), vec3(0,0,4), light));

    // Camera setup - minimal settings
    camera cam;

    // Basic image settings
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth = 50;

    // Simple camera position
    cam.vfov = 90;
    cam.lookfrom = point3(0, 0, -3);  // Looking straight at quad
    cam.lookat = point3(0, 0, 0);     // Looking at center
    cam.vup = vec3(0, 1, 0);

    // No depth of field
    cam.defocus_angle = 0;

    // Simple background
    cam.background = color(0.7, 0.7, 0.7);  // Gray background

    // Render
    cam.render(world);
}

void figure_6() {
    // Scene setup
    canbehit_list world;

    // Single triangle with basic material
    auto triangle_material = make_shared<lambertian>(color(0.3, 0.7, 0.3));  // Simple green diffuse
    
    // Define triangle vertices
    point3 v0(-1, -1, 0);    // Bottom left
    point3 v1(1, -1, 0);     // Bottom right
    point3 v2(0, 1, 0);      // Top center
    
    world.add(make_shared<triangle>(v0, v1, v2, triangle_material));

    // Add simple light source
    auto light = make_shared<diffuse_light>(color(5, 5, 5));
    world.add(make_shared<quad>(point3(-2, 2, -2), vec3(4,0,0), vec3(0,0,4), light));

    // Camera setup - minimal settings
    camera cam;

    // Basic image settings
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth = 50;

    // Simple camera position
    cam.vfov = 90;
    cam.lookfrom = point3(0, 0, -3);  // Looking straight at triangle
    cam.lookat = point3(0, 0, 0);     // Looking at center
    cam.vup = vec3(0, 1, 0);

    // No depth of field
    cam.defocus_angle = 0;

    // Simple background
    cam.background = color(0.7, 0.7, 0.7);  // Gray background

    // Render
    cam.render(world);
}

void figure_7() {
    // Scene setup
    canbehit_list world;

    // Blue material for the mesh
    auto mesh_material = make_shared<lambertian>(color(0.3, 0.3, 0.8));  // Changed to blue

    // Add Nefertiti mesh directly
    world.add(make_shared<mesh>("meshes/Nefertiti.obj", mesh_material));

    // Add light source (toned down)
    auto light = make_shared<diffuse_light>(color(7, 7, 7));  // Reduced intensity
    world.add(make_shared<quad>(point3(-2, 4, -2), vec3(4,0,0), vec3(0,0,4), light));

    // Camera setup
    camera cam;

    // Basic image settings
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth = 50;

    // Camera position (moved back)
    cam.vfov = 40;
    cam.lookfrom = point3(0, 1, 8);    // Moved from 5 to 8 to zoom out
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    // No depth of field
    cam.defocus_angle = 0;

    // Background
    cam.background = color(0.7, 0.7, 0.7);

    // Render
    cam.render(world);
}

void figure_8() {
    // Scene setup
    canbehit_list world;

    // Create the four different textures
    auto solid_texture = make_shared<solid_color>(color(0.2, 0.3, 0.7));
    auto checker_text = make_shared<checker_texture>(0.5, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    auto image_text = make_shared<image_texture>("images/max_pizza.jpg");
    auto noise_text = make_shared<noise_texture>(1.5);

    // Create materials using these textures
    auto solid_mat = make_shared<lambertian>(solid_texture);
    auto checker_mat = make_shared<lambertian>(checker_text);
    auto image_mat = make_shared<lambertian>(image_text);
    auto noise_mat = make_shared<lambertian>(noise_text);

    // Add four spheres with different textures - spread them out less
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.7, solid_mat));     // Left back
    world.add(make_shared<sphere>(point3(1, 0, -1), 0.7, checker_mat));    // Right back
    world.add(make_shared<sphere>(point3(-1, 0, 1), 0.7, image_mat));      // Left front
    world.add(make_shared<sphere>(point3(1, 0, 1), 0.7, noise_mat));       // Right front

    // Add light source - make it brighter and wider
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    world.add(make_shared<quad>(point3(-4, 4, -4), vec3(8,0,0), vec3(0,0,8), light));

    // Camera setup
    camera cam;

    // Basic image settings
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;  // More samples for better quality
    cam.max_depth = 50;

    // Camera position - adjusted to see all spheres
    cam.vfov = 70;  // Wider angle
    cam.lookfrom = point3(0, 3, 3);  // Move back and up a bit
    cam.lookat = point3(0, 0, 0);    // Look at center
    cam.vup = vec3(0, 1, 0);         // Normal up vector

    // No depth of field
    cam.defocus_angle = 0;

    // Background
    cam.background = color(0.2, 0.2, 0.2);  // Darker background for contrast

    // Render
    cam.render(world);
}

void figure_9() {
    // Scene setup
    canbehit_list world;

    // Create materials
    auto diffuse = make_shared<lambertian>(color(0.7, 0.3, 0.3));       // Red diffuse
    auto specular = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);      // Perfect mirror
    auto dielectr = make_shared<dielectric>(1.5);                      // Glass
    auto emissive = make_shared<diffuse_light>(color(4, 3, 2));         // Glowing orange

    // Add four spheres with different materials
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.7, diffuse));     // Left back
    world.add(make_shared<sphere>(point3(1, 0, -1), 0.7, specular));     // Right back
    world.add(make_shared<sphere>(point3(-1, 0, 1), 0.7, dielectr));   // Left front
    world.add(make_shared<sphere>(point3(1, 0, 1), 0.7, emissive));      // Right front

    // Add light source (dimmer since we have an emissive sphere)
    auto light = make_shared<diffuse_light>(color(10, 10, 10));
    world.add(make_shared<quad>(point3(-4, 4, -4), vec3(8,0,0), vec3(0,0,8), light));

    // Camera setup
    camera cam;

    // Basic image settings
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    // Camera position
    cam.vfov = 70;
    cam.lookfrom = point3(0, 3, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    // No depth of field
    cam.defocus_angle = 0;

    // Dark background to show emissive material better
    cam.background = color(0.1, 0.1, 0.1);

    // Render
    cam.render(world);
}

void figure_10() {
    // Scene setup
    canbehit_list world;

    // Create materials
    auto still_mat = make_shared<lambertian>(color(0.2, 0.8, 0.2));     // Green for still sphere
    auto motion_mat = make_shared<metal>(color(0.8, 0.2, 0.2), 0.0);    // Red metal for moving sphere

    // Add still sphere
    world.add(make_shared<sphere>(point3(-1, 0, 0), 0.7, still_mat));

    // Add moving sphere (motion blur from time 0 to 1)
    world.add(make_shared<sphere>(point3(1, 0, -1), point3(1, 0, 1), 0.7, motion_mat));

    // Add light source
    auto light = make_shared<diffuse_light>(color(10, 10, 10));
    world.add(make_shared<quad>(point3(-4, 4, -4), vec3(8,0,0), vec3(0,0,8), light));

    // Camera setup
    camera cam;

    // Basic image settings
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;  // More samples to capture motion blur
    cam.max_depth = 50;

    // Camera position
    cam.vfov = 60;
    cam.lookfrom = point3(0, 2, 5);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    // No depth of field
    cam.defocus_angle = 0;

    // Background
    cam.background = color(0.2, 0.2, 0.2);

    // Render
    cam.render(world);
}

void figure_1() {
    // Scene setup
    canbehit_list world;

    // Create materials
    auto red_mat = make_shared<lambertian>(color(0.8, 0.2, 0.2));    // Bright red for truck
    auto grass_mat = make_shared<lambertian>(color(0.2, 0.6, 0.2));  // Green for grass
    auto road_mat = make_shared<lambertian>(color(0.3, 0.3, 0.3));   // Gray for road
    auto building_mat = make_shared<lambertian>(color(0.6, 0.6, 0.6));  // Light gray for building
    auto sun_mat = make_shared<diffuse_light>(color(30, 16, 6));    // Orange-yellow sun
    auto trunk_mat = make_shared<lambertian>(color(0.4, 0.2, 0.1));  // Brown for tree trunk
    auto leaves_mat = make_shared<lambertian>(color(0.2, 0.5, 0.1));  // Dark green for leaves

    // Add truck mesh
    auto truck = make_shared<mesh>("meshes/Cybertruck.obj", red_mat);
    world.add(truck);

    // Add building (tall box) behind the truck
    shared_ptr<canbehit> building = box(point3(0,0,0), point3(8,15,4), building_mat);
    auto moved_building = make_shared<translate>(building, vec3(-15, 0, -4));
    world.add(moved_building);
    auto moved_building2 = make_shared<translate>(building, vec3(-15, 0, -14));
    world.add(moved_building2);
    auto moved_building3 = make_shared<translate>(building, vec3(-15, 0, 6));
    world.add(moved_building3);

    // Add tree (trunk and leaves)
    shared_ptr<canbehit> trunk = box(point3(0,0,0), point3(1,4,1), trunk_mat);
    auto moved_trunk = make_shared<translate>(trunk, vec3(-8, 0, 4));
    world.add(moved_trunk);

    // Add tree leaves (sphere on top of trunk)
    auto leaves = make_shared<sphere>(point3(-8, 5, 4), 2.5, leaves_mat);
    world.add(leaves);

    // Add tree (trunk and leaves)
    shared_ptr<canbehit> trunk2 = box(point3(0,0,0), point3(.5,2.5,.5), trunk_mat);
    auto moved_trunk2 = make_shared<translate>(trunk2, vec3(-4, 0, 8));
    world.add(moved_trunk2);

    // Add tree leaves (sphere on top of trunk)
    auto leaves2 = make_shared<sphere>(point3(-3.5, 2.5, 8), 1.5, leaves_mat);
    world.add(leaves2);

    // Add tree (trunk and leaves)
    shared_ptr<canbehit> trunk3 = box(point3(0,0,0), point3(1,3,1), trunk_mat);
    auto moved_trunk3 = make_shared<translate>(trunk3, vec3(-6, 0, -8));
    world.add(moved_trunk3);

    // Add tree leaves (sphere on top of trunk)
    auto leaves3 = make_shared<sphere>(point3(-6, 4, -8), 2, leaves_mat);
    world.add(leaves3);

    // Add multiple overlapping smoke volumes for puffier effect
    auto smoke_boundary1 = make_shared<sphere>(point3(-1.5, 0.3, -5), 1.0, 
                                             make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(smoke_boundary1, 1.5, color(0.5, 0.5, 0.5)));

    auto smoke_boundary2 = make_shared<sphere>(point3(-1.7, 0.4, -4.5), 0.8, 
                                             make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(smoke_boundary2, 2.0, color(0.6, 0.6, 0.6)));

    auto smoke_boundary3 = make_shared<sphere>(point3(-1.3, 0.2, -5.5), 0.7, 
                                             make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(smoke_boundary3, 1.8, color(0.4, 0.4, 0.4)));

    // Add "fire" spheres behind truck with motion and color variation
    for(int i = 0; i < 12; i++) {
        double x_offset = random_double(-0.3, 0.3);
        double y_offset = random_double(-0.2, 0.2);
        double z_offset = random_double(2.5, 4.5);
        double size = random_double(0.05, 0.15);
        
        auto fire_color = color(
            random_double(3, 5),
            random_double(0.4, 1.6),
            random_double(0.2, 0.4)
        );
        
        auto fire_mat = make_shared<diffuse_light>(fire_color);
        
        point3 center1(-1.5 + x_offset, 0.3 + y_offset, -z_offset);
        point3 center2(-1.5 + x_offset - 0.2,
                      0.3 + y_offset + random_double(-0.1, 0.1),
                      -z_offset + random_double(-0.2, 0.2));
        
        world.add(make_shared<sphere>(center1, center2, size, fire_mat));
    }

    // Add large grass ground plane
    world.add(make_shared<quad>(point3(-50, -0.1, -50), vec3(100,0,0), vec3(0,0,100), grass_mat));
    // Add road
    world.add(make_shared<quad>(point3(-3, -0.05, -50), vec3(6,0,0), vec3(0,0,100), road_mat));

    // Add sunset sun - repositioned to be visible in camera view
    world.add(make_shared<sphere>(point3(-20, 4, -8), 2.0, sun_mat));

    // Camera setup
    camera cam;

    // Basic image settings
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1536;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    // Camera position
    cam.vfov = 40;
    cam.lookfrom = point3(12, 2, 0);
    cam.lookat = point3(0, 2, 0);
    cam.vup = vec3(0, 1, 0);

    // No depth of field
    cam.defocus_angle = 0;

    // Darker blue for sunset sky
    cam.background = color(0.15, 0.25, 0.45);

    // Render
    cam.render(world);
}

int main() {
    switch (1) {
        case 1:  figure_1();   break;
        case 2:  figure_2();  break;
        case 3:  figure_3(); break;
        case 4:  figure_4(); break;
        case 5:  figure_5(); break;
        case 6:  figure_6(); break;
        case 7:  figure_7(); break;
        case 8:  figure_8(); break;
        case 9:  figure_9(); break;
        case 10:  figure_10(); break;

        default: figure_1(); break;
    }
}