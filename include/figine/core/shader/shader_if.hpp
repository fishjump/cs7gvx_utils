#pragma once

#include "figine/dep.hpp"
#include "figine/utils.hpp"

#include <string>

namespace figine::core {

class shader_if {
public:
  shader_if(const uint8_t *vert_code, const uint8_t *frag_code);

  void use() const;
  void build() const;
  GLuint program_id() const;

  template <class U>
  void set_uniform(const std::string &name, const U &t) const {
    if constexpr (std::is_integral_v<U> || std::is_same_v<U, bool>) {
      glUniform1i(glGetUniformLocation(_program_id, name.c_str()), (int)t);
    } else if constexpr (std::is_same_v<U, float_t>) {
      glUniform1f(glGetUniformLocation(_program_id, name.c_str()), (float_t)t);
    } else if constexpr (figine::utils::is_glm_vec_v<U, 2>) {
      glUniform2fv(glGetUniformLocation(_program_id, name.c_str()), 1, &t[0]);
    } else if constexpr (figine::utils::is_glm_vec_v<U, 3>) {
      glUniform3fv(glGetUniformLocation(_program_id, name.c_str()), 1, &t[0]);
    } else if constexpr (figine::utils::is_glm_vec_v<U, 4>) {
      glUniform4fv(glGetUniformLocation(_program_id, name.c_str()), 1, &t[0]);
    } else if constexpr (figine::utils::is_glm_mat_v<U, 2>) {
      glUniformMatrix2fv(glGetUniformLocation(_program_id, name.c_str()), 1,
                         GL_FALSE, &t[0][0]);
    } else if constexpr (figine::utils::is_glm_mat_v<U, 3>) {
      glUniformMatrix3fv(glGetUniformLocation(_program_id, name.c_str()), 1,
                         GL_FALSE, &t[0][0]);
    } else if constexpr (figine::utils::is_glm_mat_v<U, 4>) {
      glUniformMatrix4fv(glGetUniformLocation(_program_id, name.c_str()), 1,
                         GL_FALSE, &t[0][0]);
    } else {
      static_assert(figine::utils::dependent_false_v<U>, "unsupported type");
    }
  }

private:
  mutable GLuint _program_id;
  const uint8_t *const _vert_code;
  const uint8_t *const _frag_code;
};

} // namespace figine::core
