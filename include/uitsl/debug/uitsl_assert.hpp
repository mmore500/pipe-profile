#pragma once
#ifndef UITSL_DEBUG_UITSL_ASSERT_HPP_INCLUDE
#define UITSL_DEBUG_UITSL_ASSERT_HPP_INCLUDE

#include <cstdio>
#include <cstdlib>

#include "../../../third-party/Empirical/source/base/assert.h"

// adapted from https://stackoverflow.com/a/389901
#define uitsl_assert(x)                                                        \
  if (!(x)) {                                                                  \
      char buf[2048];                                                          \
      std::cerr << std::snprintf(                                              \
        buf, 2048, "Assertion failed in \"%s\", line %d\n"                     \
        __FILE__, __LINE__                                                     \
      );                                                                       \
      abort();                                                                 \
  }                                                                            \
  else

// This 'else' exists to catch the user's following semicolon

#endif // #ifndef UITSL_DEBUG_UITSL_ASSERT_HPP_INCLUDE
