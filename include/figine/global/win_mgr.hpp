#pragma once

#include "figine/dep.hpp"

#include <string>

namespace figine::global::win_mgr {

float aspect_ratio();
GLFWwindow *create_window(uint32_t width, uint32_t height,
                          const std::string &title, GLFWmonitor *monitor,
                          GLFWwindow *share);
GLFWwindow *window();

} // namespace figine::global::win_mgr
