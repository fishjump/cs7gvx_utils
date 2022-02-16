#include <figine/figine.hpp>

namespace {

figine::builtin::shader::light_t light = {
    .position = {0.0f, 2.0f, 8.0f},
    .diffuse_color = glm::vec3(0.5f) * glm::vec3(0.5f),
    .ambient_color = glm::vec3(0.5f),
    .specular_color = glm::vec3(1.0f),
};

figine::builtin::shader::material_t material = {
    .shininess = 16,
    .ambient_color = glm::vec3(0.5f),
    .diffuse_color = glm::vec3(0.5f),
    .specular_color = glm::vec3(0.5f)};

figine::builtin::shader::phong_shader_t phong(material, light);
figine::core::camera_t camera({0.0f, 1.0f, 8.0f});
figine::core::object_t object("test/teapot.obj", &phong, &camera);
figine::imnotgui::demo_window_t win;

} // namespace

int main() {
  figine::global::init();
  GLFWwindow *window = figine::global::win_mgr::create_window(
      800, 600, "test window", NULL, NULL);

  phong.build();
  object.init();

  camera.lock({0, 0, 0});

  figine::imnotgui::init(window);
  figine::imnotgui::register_window(&win);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    object.loop();
    figine::imnotgui::render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}