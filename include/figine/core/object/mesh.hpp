#pragma once

#include "figine/core/shader/shader_if.hpp"
#include "figine/dep.hpp"

#include <array>
#include <string>
#include <vector>

namespace figine::core {

constexpr size_t MAX_BONE_INFLUENCE = 4;

struct vertex_t {

  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_coordinate;
  glm::vec3 tangent;
  glm::vec3 bitangent;
  std::array<int, MAX_BONE_INFLUENCE> bone_ids;
  std::array<float, MAX_BONE_INFLUENCE> bone_weights;

  inline void set_weight(int id, float weight) {
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
      bone_ids[i] = id;
      bone_weights[i] = weight;
    }
  }

  inline void init_bones() { set_weight(-1, 0); }
};

struct texture_t {
  uint32_t id;
  std::string type;
  std::string path;
};

class mesh_t final {
public:
  mesh_t(const std::vector<vertex_t> &vertices,
         const std::vector<uint32_t> &indices,
         const std::vector<texture_t> &textures);

  void init();
  void draw(const shader_if &shader) const;

  GLuint vao = 0;

private:
  std::vector<vertex_t> _vertices;
  std::vector<uint32_t> _indices;
  std::vector<texture_t> _textures;
};

} // namespace figine::core
