#include "window.h"
#include <iostream>

namespace gle
{
constexpr GLint WINDOW_WIDTH  = 500;
constexpr GLint WINDOW_HEIGHT = 500;

Window::Window()
{
  if (glfwInit() != GLFW_TRUE)
  {
    std::cerr << "Error initializing GLFW" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 3.3
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE);            // Use core profile; essentially disables backwards compat
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Enable forward compatibility

  this->_window = glfwCreateWindow(gle::WINDOW_WIDTH, gle::WINDOW_HEIGHT, "GLEngine", NULL, NULL);

  glfwMakeContextCurrent(this->_window);

  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK)
  {
    std::cerr << "Error initializing GLEW" << std::endl;

    glfwDestroyWindow(this->_window);
    glfwTerminate();

    std::exit(EXIT_FAILURE);
  }

  glfwGetFramebufferSize(this->_window, &this->_fb_width, &this->_fb_height);

  glViewport(0, 0, this->_fb_width, this->_fb_height);

  glfwSetWindowUserPointer(this->_window, this); // Says that this class is the user of its member window

  glEnable(GL_DEPTH_TEST);

  // Create event callbacks
  glfwSetKeyCallback(this->_window, this->_HandleKeys); // Call key handler whenever a key is pressed in this window
  glfwSetCursorPosCallback(this->_window, this->_HandleMouse); // You can guess what this does

  // Ensure cursor stays at screen center and is hidden
  glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window()
{
  glfwDestroyWindow(this->_window);
  glfwTerminate();

  // TODO: Ensure stuff is unset properly as well (glfwMakeContextCurrent...)
}

GLFWwindow *Window::GetHandle() { return this->_window; }

void Window::PollFramebufferSize() { glfwGetFramebufferSize(this->_window, &this->_fb_width, &this->_fb_height); }

GLsizei Window::GetFramebufferWidth() { return this->_fb_width; }

GLsizei Window::GetFramebufferHeight() { return this->_fb_height; }

void Window::_HandleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
  Window *_this = static_cast<Window *>(glfwGetWindowUserPointer(window)); // Get the owner class to circumvent static

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key >= 0 && key <= 1024)
  {
    if (action == GLFW_PRESS)
    {
      _this->_keys[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
      _this->_keys[key] = false;
    }
  }
}

void Window::_HandleMouse(
    GLFWwindow *window,
    double      x_pos,
    double y_pos) // WARNING: These need to be doubles! Function pointer signature must match glfwSetCallback expects
{
  Window *_this = static_cast<Window *>(glfwGetWindowUserPointer(window)); // Get owner class to circumvent static

  if (!_this->_mouse_moved)
  {
    _this->_last_x = x_pos;
    _this->_last_y = y_pos;

    _this->_mouse_moved = true;
  }

  _this->_dx = x_pos - _this->_last_x;
  _this->_dy = (y_pos - _this->_last_y) * -1; // Flip to avoid inverted up, down looking

  _this->_last_x = x_pos;
  _this->_last_y = y_pos;
}

bool const *Window::GetKeys() { return this->_keys.data(); }

float Window::GetDx() { return this->_dx; }
float Window::GetDy() { return this->_dy; }
void  Window::SetDx(float dx) { this->_dx = dx; };
void  Window::SetDy(float dy) { this->_dy = dy; }
} // namespace gle
