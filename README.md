![Screenshot01](https://github.com/user-attachments/assets/4012a609-881c-409a-921f-43b290e826d8)

# ForceField3D

**3D visualization of electric field**

This application intended to visualize electric field (lines of force, equipotential surface) in 3D.

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
    cd ForceField3D
```

- Build:
```shell
    cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j
```

## Run:
- On Linux:
```shell
    ./bin/ForceField3D
```
- On Windows:
```shell
    bin\ForceField3D.exe
```

## Usage
- e - Switch equipotential surface on/off
- f - Switch lines of force on/off
- p - Add positive charge at cursor position or increase existing charge value by one unit
- n - Add negative charge at cursor position or decrease existing charge value by one unit
- s - Save current state
- l - Load state
- r - Reset state to starter
- Del - Delete all charges
- Esc - Exit program
- Left click + mouse move on charge - Drag the charge
- Left click + mouse move on scene - Drag the scene
- Right click + mouse move on scene - Rotate the scene
