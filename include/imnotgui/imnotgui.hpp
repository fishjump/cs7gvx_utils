#pragma once

#include <gl.hpp>

namespace imnotgui {

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

} // namespace imnotgui