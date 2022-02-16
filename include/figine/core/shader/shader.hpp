#pragma once

#include "figine/core/object.hpp"
#include "figine/dep.hpp"
#include "figine/global/win_mgr.hpp"
#include "shader_if.hpp"
#include "shader_profile.hpp"

#include <boost/pfr.hpp>

#include <fstream>
#include <memory>
#include <string>
#include <type_traits>

namespace figine::core {

std::string load_shader_code(const std::string &file);

template <class T> class shader_t : public shader_if {
public:
  shader_t(const uint8_t *vert_code, const uint8_t *frag_code,
           std::shared_ptr<T> profile)
      : shader_if(vert_code, frag_code), profile(profile) {}

  std::shared_ptr<T> profile;

  virtual void init(const object_t *obj) const override {
    // profile->view_pos = obj->camera->position;
    // profile->projection =
    //     glm::perspective(glm::radians(obj->camera->zoom),
    //                      figine::global::win_mgr::aspect_ratio(), 0.1f,
    //                      100.0f);
    // profile->view = obj->camera->view_matrix();
    // profile->transform = obj->transform;
  }

  virtual void update(const object_t *obj) const override {
    profile->view_pos = obj->camera->position;
    profile->projection =
        glm::perspective(glm::radians(obj->camera->zoom),
                         figine::global::win_mgr::aspect_ratio(), 0.1f, 100.0f);
    profile->view = obj->camera->view_matrix();
    profile->transform = obj->transform;
  }

  virtual void apply_profile() const final {
    if (profile == nullptr) {
      return;
    }

    const meta_profile_t &meta = profile->meta();
    boost::pfr::for_each_field(meta, [this](const auto &field) {
      for (const auto &pair : field) {
        this->set_uniform(pair.first, pair.second);
      }
    });
  }
};

} // namespace figine::core
