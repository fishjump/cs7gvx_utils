#include "figine/global.hpp"
#include "figine/logging.hpp"

namespace {

GLFWwindow *_window = nullptr;
uint32_t _width = 0;
uint32_t _height = 0;

} // namespace

namespace figine::global::win_mgr {

float aspect_ratio() { return (float)_width / (float)_height; }

GLFWwindow *create_window(uint32_t width, uint32_t height,
                          const std::string &title, GLFWmonitor *monitor,
                          GLFWwindow *share) {
  _window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (_window == nullptr) {
    LOG_ERR("Failed to create GLFW window");
    return nullptr;
  }

  glfwMakeContextCurrent(_window);
  // glfwSetFramebufferSizeCallback(_window, cs7gv5::framebuffer_size_callback);
  // glfwSetCursorPosCallback(_window, cs7gv5::mouse_callback);
  // glfwSetScrollCallback(_window, cs7gv5::scroll_callback);
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_ERR("failed to initialize glad");
    return nullptr;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  _width = width;
  _height = height;

  return _window;
}

GLFWwindow *window() { return _window; }

} // namespace figine::global::win_mgr
