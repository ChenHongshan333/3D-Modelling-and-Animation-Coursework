# CS3242 3D Modeling and Animation

A collection of coursework for CS3242 3D Modeling and Animation at the National University of Singapore (NUS), Semester 2, AY 2025/2026. This repository demonstrates key concepts in 3D computer graphics including mesh modeling, character animation, and rendering techniques.

**Course**: CS3242 3D Modeling and Animation  
**Institution**: National University of Singapore, School of Computing  
**Semester**: Semester 2, AY 2025/2026

## Table of Contents
- [Background](#background)
- [What's Included](#whats-included)
- [Technologies Used](#technologies-used)
- [Install](#install)
- [Usage](#usage)
- [Controls](#controls)
- [Repository Layout](#repository-layout)
- [Contributing](#contributing)
- [License](#license)
- [Academic Integrity](#academic-integrity)

## Background

This repository showcases two major projects completed for CS3242, demonstrating practical applications of 3D modeling theory, animation principles, and computational geometry. Assignment 1 focuses on storytelling through 3D animation using Blender, while Assignment 2 implements fundamental mesh processing algorithms in C++/OpenGL.

## What's Included

### Assignment 1 - Short Animation (Group Project)

A collaborative 3D animation project created in Blender, exploring character animation, rigging, and visual effects.

**Story Overview**  
Pac-Man accidentally enters a Minecraft world through a mysterious portal and embarks on a journey of cultural adaptation with the help of friendly villagers.

**Key Scenes**
- Scene 1: The Void - Pac-Man discovers glowing cubes and gets pulled into a portal
- Scene 2: Arrival - Pac-Man crashes into a Minecraft village
- Scene 3: The Waterfall - Villagers show Pac-Man around, ending with a waterfall jump
- Scene 4: Integration - Pac-Man transforms into a blocky version of himself

**Technical Implementation**
- Custom character modeling using boolean operations and UV sphere manipulation
- Minecraft villagers using MCprep plugin with skeletal rigging
- Keyframing with Bezier interpolation for smooth motion
- IK (Inverse Kinematics) rigging for character animation
- Particle systems for portal and firework effects
- Dynamic camera work with easing for cinematic shots
- Cycles renderer with custom lighting for day/night transitions

**Deliverables**
- Storyboard
- Major frames documentation
- Final animation video
- Blender project files
- Final report with technical documentation

**Team Members**
- Alicia Chang Chuo Ran
- Bian Sihan
- Chen Hongshan 
- Lee Seung-Yoon

### Assignment 2 - 3D Mesh Modeling (Individual Project)

A C++/OpenGL implementation of mesh processing algorithms and data structures for parsing, analyzing, and rendering 3D triangle meshes.

**Features Implemented**

- **Task 1: Computing Normal Vectors**  
  Calculate face normals for each triangle to enable proper flat shading with lighting

- **Task 2: Angle Statistics**  
  Compute minimum and maximum angles for each triangle and generate histogram statistics with 10-degree buckets (0°-180°)

- **Task 3: Triangle Mesh Operations**  
  Implement fundamental mesh navigation functions:
  - `enext()` - Edge traversal within a triangle
  - `sym()` - Symmetric edge across adjacent triangles
  - `org()` - Origin vertex of an edge
  - `dest()` - Destination vertex of an edge

- **Task 4: Face Adjacency (fnext)**  
  Link all triangles through shared edges to build adjacency data structure

- **Task 5: Component Analysis**  
  Use BFS/DFS to compute the number of disconnected mesh components

- **Task 6: Smooth Shading**  
  Compute vertex normals by averaging surrounding face normals, toggle between flat and smooth (Gouraud/Phong) shading modes

- **Task 7: Boundary Visualization**  
  Identify and highlight boundary edges (edges with only one adjacent triangle) in red

**File Format Support**
- Read and write Wavefront OBJ files
- Parse vertex positions and triangle face indices
- Export modified meshes back to OBJ format

## Technologies Used

### Assignment 1 (Animation)
- Blender 3.x - 3D modeling and animation
- MCprep - Minecraft-style asset integration
- Cycles Renderer - Physically-based rendering

### Assignment 2 (Modeling)
- C++ - Core programming language
- OpenGL - 3D graphics rendering
- GLUT/FreeGLUT - Window management and input handling
- Visual Studio / Xcode - Development environment

## Install

### Requirements

**Assignment 1**
- Blender 3.x or later
- MCprep plugin (optional, for Minecraft assets)

**Assignment 2**
- Windows (Visual Studio 2022) or macOS (Xcode)
- Platform: x64
- OpenGL-compatible graphics card
- FreeGLUT library

### Dependency Setup (Assignment 2)

For Windows users, the repository includes FreeGLUT setup:
- FreeGLUT files are stored in project dependencies
- Visual Studio solution is pre-configured with proper include/lib paths

For macOS users:
- OpenGL and GLUT are included with Xcode
- No additional dependencies required

### Build

**Assignment 1**
1. Open the Blender project file (.blend)
2. Install required add-ons if needed (MCprep)
3. Render or explore the animation scene

**Assignment 2 - Windows (Visual Studio)**
1. Clone the repository
2. Open `Lab_01.sln` in Visual Studio 2022
3. Select configuration: Debug or Release
4. Select platform: x64
5. Build the solution

**Assignment 2 - macOS (Xcode)**
1. Clone the repository
2. Open the Xcode project
3. Build and run

## Usage

### Assignment 1 - Animation
1. Open the Blender project file
2. Press Spacebar to play the animation in the viewport
3. Use Render > Render Animation to export the final video

### Assignment 2 - Modeling

**Windows**
1. Build the project in Visual Studio
2. Place .obj test files in the project directory
3. Run the executable
4. When prompted, enter filename (e.g., "../../cat.obj")

**macOS**
1. Build the project in Xcode
2. Locate the executable directory (Products folder)
3. Copy .obj files to the executable directory
4. Run and enter filename when prompted

## Controls

### Assignment 2 - Mesh Modeling Controls

- **W**: Toggle wireframe mode
- **S**: Toggle smooth/flat shading
- **O**: Export current mesh to OBJ file
- **Mouse Drag**: Rotate camera
- **Mouse Scroll**: Zoom in/out
- **ESC/Q**: Quit

## Repository Layout

```
CS3242-3D-Modeling-and-Animation/
│
├── README.md
├── LICENSE (可选)
│
├── Assignment1-Animation/
│   ├── README.md
│   ├── docs/
│   │   ├── CS3242_StoryBoard.pdf
│   │   ├── CS3242_Assignment_Major_Frames.pdf
│   │   └── CS3242_Animation_Final_Report.pdf
│   ├── animation/
│   │   └── (视频文件 - 看下面的大文件处理)
│   └── blender-files/
│       └── (你的 .blend 文件 - 看下面的大文件处理)
│
└── Assignment2-Modeling/
    ├── README.md
    ├── src/
    │   ├── main.cpp
    │   ├── mesh.h
    │   └── mesh.cpp
    ├── Lab_01.sln
    ├── docs/
    │   └── CS3242AssignmentModelingV2_0.pdf
    └── test-files/
        ├── cat.obj
        └── Alucy.obj
```

## Contributing

This repository is primarily for portfolio and learning purposes.
- PRs are welcome for improvements like documentation or build cleanup
- Please respect academic integrity rules if you are taking the same course

## License

This project is submitted as coursework for CS3242 at NUS and is intended for educational and portfolio purposes only. All rights reserved.

See `LICENSE` for details.

## Academic Integrity

This repository is shared for portfolio and reference purposes. If you are currently taking a similar course, please follow your institution's academic integrity policy and do not copy solutions for graded work.

## Acknowledgments

- Course instructor and teaching staff for guidance and support
- NUS School of Computing for providing course materials
- Blender Foundation for the open-source 3D software
- MCprep developers for Minecraft integration tools
