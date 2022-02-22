#include "figine/core/object/mesh.hpp"
#include "figine/logging.hpp"

#include <boost/pfr.hpp>

namespace figine::core {
mesh_t::mesh_t(const std::vector<vertex_t> &vertices,
               const std::vector<uint32_t> &indices,
               const std::vector<texture_t> &textures)
    : _vertices(vertices), _indices(indices), _textures(textures) {}

void mesh_t::init() {
  glGenVertexArrays(1, &_vao);
  GLuint _vbo = 0, _ebo = 0;
  glGenBuffers(1, &_vbo);
  glGenBuffers(1, &_ebo);

  glBindVertexArray(_vao);
  defer(glBindVertexArray(NULL));

  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER,
               _vertices.size() *
                   sizeof(figine::utils::value_type_t<decltype(_vertices)>),
               _vertices.cbegin().base(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               _indices.size() *
                   sizeof(figine::utils::value_type_t<decltype(_indices)>),
               _indices.cbegin().base(), GL_STATIC_DRAW);

  const auto b = *_vertices.cbegin();

  boost::pfr::for_each_field(b, [&b](const auto &field, int index) {
    glEnableVertexAttribArray(index);
    if constexpr (std::is_convertible_v<decltype(field),
                                        decltype(b.bone_ids)>) {
      glVertexAttribIPointer(index, sizeof(field) / sizeof(GL_INT), GL_INT,
                             sizeof(b),
                             (void *)figine::utils::offset_diff(field, b));
    } else {
      glVertexAttribPointer(index, sizeof(field) / sizeof(GL_FLOAT), GL_FLOAT,
                            GL_FALSE, sizeof(b),
                            (void *)figine::utils::offset_diff(field, b));
    }
  });
};

void mesh_t::draw(const shader_if &shader) const {
  uint32_t diffuseNr = 1, specularNr = 1, normalNr = 1, heightNr = 1;
  for (size_t i = 0; i < _textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::string number = "", name = _textures[i].type;
    if (name == "texture_diffuse") {
      number = std::to_string(diffuseNr++);
    } else if (name == "texture_specular") {
      number = std::to_string(specularNr++);
    } else if (name == "texture_normal") {
      number = std::to_string(normalNr++);
    } else if (name == "texture_height") {
      number = std::to_string(heightNr++);
    }

    glUniform1i(
        glGetUniformLocation(shader.program_id(), (name + number).c_str()), i);
    glBindTexture(GL_TEXTURE_2D, _textures[i].id);
  }

  glBindVertexArray(_vao);
  defer(glBindVertexArray(NULL));

  glDrawElements(GL_TRIANGLES, (uint32_t)_indices.size(), GL_UNSIGNED_INT, 0);
}

} // namespace figine::core
