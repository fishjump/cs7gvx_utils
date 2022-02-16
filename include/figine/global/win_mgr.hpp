#pragma once

#include "figine/dep.hpp"

#include <string>

namespace figine::global::win_mgr {

extern uint32_t width;
extern uint32_t height;

float aspect_ratio();
GLFWwindow *window();
GLFWwindow *create_window(uint32_t width, uint32_t height,
                          const std::string &title, GLFWmonitor *monitor,
                          GLFWwindow *share);

} // namespace figine::global::win_mgr
