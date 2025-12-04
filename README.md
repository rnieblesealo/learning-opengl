# Notes

- I am using GSL for easy null checking... `gsl::not_null`

## GLEW

- Allows interfacing with newer OpenGL versions
- Helps load OpenGL extensions, check their compatibility

## GLFW

- Creation of OpenGL context
- Window creation + control
- Input
- Monitor handling 

## Graphics

- There can be many VBOs bound to one VAO
- OpenGL window center is (0, 0); coordinates are from -1 to 1 
- VBO modes:
    - GL static, gl dynamic

### Rendering Pipeline

- Stages for something to draw to screen
- 9 stages total
  1. Vertex specification
    - Essentially defining the vertices (in an array or what have you)
    - VBO: Vertex data itself
    - VAO: *What* vertex has (position, color, normals, etc.)
    > Data associated to these is kept in GPU memory 
  2. Vertex shader (programmable)
  3. Tessellation (programmable)
  4. Geometry shader (programmable)
  5. Vertex post-processing
  6. Primitive assembly
  7. Rasterization
  8. Fragment shader (programmable)
  9. Per-sample operations
- 4 stages programmable via shaders (GLSL/HLSL)
  1. Vertex
  2. Fragment
  3. Tessellation
  4. Compute

### GLSL

- `#version` directive is GLSL version not OpenGL
  - They are now in sync, they haven't always been
  > This is why shaders on Mac require version 4.1 referring to Metal 4.1
