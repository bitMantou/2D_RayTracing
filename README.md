# Ray Tracing in C

![Ray Tracing Simulation](ray_tracing.gif)

## Overview

This project is a simple implementation of ray tracing using SDL2 in C. The goal is to visualize light rays interacting with obstacles in a 2D space. It serves as an introduction to fundamental graphics programming concepts such as ray casting, collision detection, and shading techniques.

## Ray Tracing Concept

Ray tracing is a rendering technique used to simulate the way light interacts with objects. This project follows a basic approach:
- **Ray Emission**: Rays originate from a moving source and are cast outward in different directions.
- **Object Interaction**: Rays continue traveling until they collide with an obstacle, creating a shadowed region.
- **Fading Effect**: The intensity of the rays decreases with distance to simulate light falloff.

## Implementation Details

This simulation implements:

- **Ray Casting**: Uses trigonometric functions to compute ray trajectories.
- **Object Collision**: Detects ray intersections with circular obstacles.
- **Light Intensity Fading**: Implements a simple fading effect based on ray distance.
- **Dynamic Obstacle Motion**: A moving obstacle affects the rays in real time.

## Technical Features

### Ray Emission

The simulation generates rays originating from a central source. Each ray follows a trajectory given by:

```
x = x_start + step * cos(angle)
y = y_start + step * sin(angle)
```

### Collision Detection

Rays stop when they hit an object. This is determined by checking whether a point lies within a circular obstacle:

```
distance² = (x - object_x)² + (y - object_y)²
if distance² < radius²: collision detected
```

### Dynamic Obstacles

A circular obstacle moves vertically, bouncing off screen boundaries. This demonstrates real-time interactions between light and objects.

## Building and Running

### Prerequisites

Ensure you have SDL2 installed.

#### Ubuntu/Debian
```sh
sudo apt update && sudo apt install libsdl2-dev
```

#### Arch Linux
```sh
sudo pacman -S sdl2
```

#### macOS
```sh
brew install sdl2
```

### Build Instructions

```sh
gcc -o raytracing main.c -lSDL2 -lm
./raytracing
```

## Future Improvements

- Implement reflection and refraction for more realistic light behavior.
- Optimize ray-object intersection calculations for better performance.
- Add textures and shading for a more visually appealing effect.
- Expand the simulation to support multiple light sources.

## License

This project is open-source and available under the MIT License.
