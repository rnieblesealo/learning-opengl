cmake_minimum_required(VERSION 3.5)

project(GLEngine)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE GLEngineSources CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.h")

add_executable(GLEngine "${GLEngineSources}")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(GLEngine PRIVATE glfw glm::glm GLEW::GLEW OpenGL::GL)

target_include_directories(GLEngine PRIVATE "${CMAKE_SOURCE_DIR}/src/include")

target_compile_features(GLEngine PRIVATE cxx_std_17)

# Copy parent dir assets to build;  WARNING: Not ideal for lots of assets!
add_custom_command(
  TARGET GLEngine
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:GLEngine>/assets)

# Also copy shaders dir
add_custom_command(
  TARGET GLEngine
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/glsl
          $<TARGET_FILE_DIR:GLEngine>/glsl)

# CMAKE_SOURCE_DIR is where CMakeLists.txt is; CURRENT variant is where the
# currently processed CMakeLists is

# CMAKE_COMMAND equates to doing "cmake" in the CLI; it's essentially a CLI
# command; above, it's same as "cmake -E copy_directory" which is a CMake-safe
# version of "cp"
