#pragma once

#include "../dep.hpp"

#include <cstdio>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <type_traits>

#define LOG_LEVEL_NO_LOG 0
#define LOG_LEVEL_ERR 1
#define LOG_LEVEL_WRAN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

#define LOG(log_level, fmt, ...)                                               \
  do {                                                                         \
    if constexpr (LOG_LEVEL >= log_level) {                                    \
      printf(fmt "\n", ##__VA_ARGS__);                                         \
    }                                                                          \
  } while (false);

#define LOG_ERR(fmt, ...)                                                      \
  do {                                                                         \
    LOG(LOG_LEVEL_ERR, "[ERR][%s] at %s:%d: " fmt, __FUNCTION__, __FILE__,     \
        __LINE__, ##__VA_ARGS__);                                              \
  } while (false);

#define LOG_WRAN(fmt, ...)                                                     \
  do {                                                                         \
    LOG(LOG_LEVEL_WRAN, "[WRAN][%s] at %s:%d: " fmt, __FUNCTION__, __FILE__,   \
        __LINE__, __VA_ARGS__);                                                \
  } while (false);

#define LOG_INFO(fmt, ...)                                                     \
  do {                                                                         \
    LOG(LOG_LEVEL_INFO, "[INFO][%s] at %s:%d: " fmt, __FUNCTION__, __FILE__,   \
        __LINE__, __VA_ARGS__);                                                \
  } while (false);

#define LOG_DEBUG(fmt, ...)                                                    \
  do {                                                                         \
    LOG(LOG_LEVEL_DEBUG, "[DEBUG][%s] at %s:%d: " fmt, __FUNCTION__, __FILE__, \
        __LINE__, __VA_ARGS__);                                                \
  } while (false);
