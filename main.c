#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WIDTH 1280
#define HEIGHT 720
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0xff000000
#define COLOR_RAY   0xffd9b44e
#define NUM_OF_RAYS 700
#define RAY_SIZE 10


struct Circle {
    double x;
    double y;
    double r;
};

struct Ray {
    double x_start, y_start;
    double angle;
};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color) {
    double radius_squared = pow(circle.r, 2);
    for (double x = circle.x-circle.r; x <= circle.x + circle.r; x++) {
        for (double y = circle.y-circle.r; y <= circle.y + circle.r; y++) {
            double distance_squared = pow((x - circle.x), 2) + pow((y - circle.y), 2);

            if (distance_squared < radius_squared) {
                SDL_Rect pixel = (SDL_Rect) {x, y, 1, 1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

void GenerateRays(struct Circle circle, struct Ray rays[NUM_OF_RAYS]) {
    for (int i = 0; i < NUM_OF_RAYS; i++) {
        double angle = ( (double) i / NUM_OF_RAYS) * 2 * M_PI;
        struct Ray ray = { circle.x, circle.y, angle};
        rays[i] = ray;

    }
}

void FillRays(SDL_Surface* surface, struct Ray rays[NUM_OF_RAYS], Uint32 color, struct Circle object) {
    double radius_squared = pow(object.r, 2);
    double step = 1; // Finer step size for more detail
    double max_distance = 1000; // Max distance for fading

    for (int i = 0; i < NUM_OF_RAYS; i++) {
        struct Ray ray = rays[i];

        int end_of_screen = 0;

        double x_draw = ray.x_start;
        double y_draw = ray.y_start;
        while (!end_of_screen) {
            x_draw += step * cos(ray.angle);
            y_draw += step * sin(ray.angle);

            // Calculate distance and intensity for fading
            double distance_squared = pow((x_draw - ray.x_start), 2) + pow((y_draw - ray.y_start), 2);
            double distance = sqrt(distance_squared);
            double intensity = 1.0 - (distance / max_distance);
            intensity = intensity < 0 ? 0 : intensity;
            Uint32 faded_color = (color & 0x00FFFFFF) | ((Uint8)(255 * intensity) << 24);

            SDL_Rect ray_point = { (int)x_draw, (int)y_draw, RAY_SIZE, RAY_SIZE };
            SDL_FillRect(surface, &ray_point, faded_color);

            if (x_draw < 0 || y_draw < 0 || x_draw > WIDTH || y_draw > HEIGHT) {
                end_of_screen = 1;
            }

            // Check if ray hit the object
            double distance_to_object_squared = pow((x_draw - object.x), 2) + pow((y_draw - object.y), 2);
            if (distance_to_object_squared < radius_squared) {
                break;
            }
        }
    }
}


int main( int argc, char* args[] ) {
    SDL_Window* window = SDL_CreateWindow("RayTracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface( window );

    struct Circle circle = (struct Circle) {200, 200, 20};
    struct Circle second_circle = (struct Circle) {900, 300, 100};
    struct Ray rays[NUM_OF_RAYS];
    SDL_Rect erase_rect = (SDL_Rect) {0, 0, WIDTH, HEIGHT};

    double obstacle_speed_y = 5;
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_MOUSEMOTION && event.motion.state != 0) {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                GenerateRays(circle, rays);
            }
        }
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
        GenerateRays(circle, rays);
        FillRays(surface, rays, COLOR_RAY, second_circle);
        FillCircle(surface, circle, COLOR_WHITE);
        FillCircle(surface, second_circle, COLOR_WHITE);

        second_circle.y += obstacle_speed_y;
        second_circle.y += obstacle_speed_y;
        if (second_circle.y - second_circle.r < 0) {
            second_circle.y = second_circle.r;
            obstacle_speed_y *= -1;
        }
        if (second_circle.y + second_circle.r > HEIGHT) {
            second_circle.y = HEIGHT - second_circle.r;
            obstacle_speed_y *= -1;
        }


        SDL_UpdateWindowSurface( window );
        SDL_Delay(10);
    }
}
