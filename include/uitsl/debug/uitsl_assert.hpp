#pragma once
#ifndef UITSL_DEBUG_UITSL_ASSERT_HPP_INCLUDE
#define UITSL_DEBUG_UITSL_ASSERT_HPP_INCLUDE

#include <cstdio>
#include <cstdlib>

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

// adapted from https://stackoverflow.com/a/389901
#define uitsl_assert(x)                                                        \
  if (!(x)) {                                                                  \
      std::cerr << emp::format_string(                                         \
        "Assertion failed in \"%s\", line %d\n", __FILE__, __LINE__            \
      ) << std::endl;                                                          \
      abort();                                                                 \
  }                                                                            \
  else

// This 'else' exists to catch the user's following semicolon

#endif // #ifndef UITSL_DEBUG_UITSL_ASSERT_HPP_INCLUDE
