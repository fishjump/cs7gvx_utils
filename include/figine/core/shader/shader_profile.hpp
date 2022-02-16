#pragma once

#include "figine/dep.hpp"
#include "figine/utils.hpp"

#include <string>
#include <type_traits>
#include <unordered_map>

namespace figine::core {

struct meta_profile_t {
  std::unordered_map<std::string, GLboolean> booleans;
  std::unordered_map<std::string, GLint> integrals;
  std::unordered_map<std::string, GLfloat> floats;
  std::unordered_map<std::string, glm::vec2> vec2s;
  std::unordered_map<std::string, glm::vec3> vec3s;
  std::unordered_map<std::string, glm::vec4> vec4s;
  std::unordered_map<std::string, glm::mat2> mat2s;
  std::unordered_map<std::string, glm::mat3> mat3s;
  std::unordered_map<std::string, glm::mat4> mat4s;

  template <class T> void insert(const std::string &key, const T &value);
};

class shader_profile_t {
public:
  inline virtual const meta_profile_t &meta() const {
    _meta.insert("view_pos", view_pos);

    _meta.insert("projection", projection);
    _meta.insert("view", view);
    _meta.insert("transform", transform);
    return _meta;
  }

  glm::vec3 view_pos;
  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 transform;

protected:
  mutable meta_profile_t _meta;
};

} // namespace figine::core

template <class T>
void figine::core::meta_profile_t::insert(const std::string &key,
                                          const T &value) {
  if constexpr (std::is_same_v<T, GLboolean>) {
    this->booleans[key] = value;
  } else if constexpr (std::is_integral_v<T>) {
    this->integrals[key] = value;
  } else if constexpr (std::is_same_v<T, GLfloat>) {
    this->floats[key] = value;
  } else if constexpr (figine::utils::is_glm_vec_v<T, 2>) {
    this->vec2s[key] = value;
  } else if constexpr (figine::utils::is_glm_vec_v<T, 3>) {
    this->vec3s[key] = value;
  } else if constexpr (figine::utils::is_glm_vec_v<T, 4>) {
    this->vec4s[key] = value;
  } else if constexpr (figine::utils::is_glm_mat_v<T, 2>) {
    this->mat2s[key] = value;
  } else if constexpr (figine::utils::is_glm_mat_v<T, 3>) {
    this->mat3s[key] = value;
  } else if constexpr (figine::utils::is_glm_mat_v<T, 4>) {
    this->mat4s[key] = value;
  } else {
    static_assert(figine::utils::dependent_false_v<T>, "unsupported type");
  }
}