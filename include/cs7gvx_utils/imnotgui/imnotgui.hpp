#pragma once

#include <cs7gvx_utils/gl.hpp>

#include "imgui/imconfig.h"
#include "imgui/imgui.h"

namespace cs7gvx_utils::imnotgui {

class window_t {
public:
  window_t();
  virtual void refresh() = 0;
};

class demo_window_t : public window_t {
public:
  void refresh() override;

private:
  bool show_demo_window;
  bool show_another_window;
  bool clear_color;
};

void init(GLFWwindow *window);
void render();
void register_window(window_t *window);

} // namespace cs7gvx_utils::imnotgui