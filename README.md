# ForceField3D

**3D visualization of electric field**

This application intended to visualize electric field (force lines, equipotential surface) in 3D.

## Features
- Interactive
- Realtime
- Easy to use

## Get and build
- Dependency:
    - cmake
    - c++ compiler
    - OpenGL libraries (eg. libglu1-mesa-dev on Ubuntu)

- Get source:
```shell
    git clone --recursive https://github.com/50gramm/ForceField3D.git
```

- Build:
```shell
    cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build/ -j
```

## Run:
- On Linux:
```shell
    ./bin/ForceField3D
```
- On Windows:
```shell
    bin/ForceField3D.exe
```

## Usage
- e - Switch equipotential surface on/off
- p - Add positive charge at cursor position or increase existing charge value by one unit
- n - Add negative charge at cursor position or decrease existing charge value by one unit
- Esc - Exit program
- Left click + mouse move on charge - Drag the charge
- Left click + mouse move on scene - Drag the scene
- Right click + mouse move on scene - Rotate the scene
