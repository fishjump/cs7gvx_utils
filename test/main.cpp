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

figine::builtin::shader::phong_shader_t phong_shader;
figine::core::camera_t camera({0.0f, 1.0f, 8.0f});
figine::core::object_t object("test/teapot.obj", &camera);
figine::imnotgui::demo_window_t win;

} // namespace

int main() {
  figine::global::init();
  GLFWwindow *window = figine::global::win_mgr::create_window(
      800, 600, "test window", NULL, NULL);

  phong_shader.build();
  object.init();

  camera.lock({0, 0, 0});

  figine::imnotgui::init(window);
  figine::imnotgui::register_window(&win);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    phong_shader.use();
    phong_shader.set_uniform("light.position", light.position);
    phong_shader.set_uniform("light.ambient_color", light.ambient_color);
    phong_shader.set_uniform("light.diffuse_color", light.diffuse_color);
    phong_shader.set_uniform("light.specular_color", light.specular_color);

    phong_shader.set_uniform("material.shininess", material.shininess);
    phong_shader.set_uniform("material.ambient_color", material.ambient_color);
    phong_shader.set_uniform("material.diffuse_color", material.diffuse_color);
    phong_shader.set_uniform("material.specular_color",
                             material.specular_color);

    object.loop(phong_shader);
    figine::imnotgui::render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}