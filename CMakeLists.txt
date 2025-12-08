cmake_minimum_required(VERSION 3.5)

project(GLEngine)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE GLEngineSources CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

add_executable(GLEngine "${GLEngineSources}")

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

include(FetchContent)

FetchContent_Declare(
  microsoft-gsl
  GIT_REPOSITORY "https://github.com/microsoft/GSL"
  GIT_TAG "v4.2.0"
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(microsoft-gsl)

target_link_libraries(GLEngine PRIVATE glfw glm::glm GLEW::GLEW
                                       Microsoft.GSL::GSL)

target_include_directories(GLEngine
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/include")

target_compile_features(GLEngine PRIVATE cxx_std_17)
