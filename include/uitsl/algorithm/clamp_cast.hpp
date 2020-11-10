#pragma once
#ifndef UITSL_ALGORITHM_CLAMP_CAST_HPP_INCLUDE
#define UITSL_ALGORITHM_CLAMP_CAST_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <limits>

#include "nan_to_zero.hpp"

namespace uitsl {

template<typename To, typename From>
To clamp_cast( const From from ) {


  #ifndef __EMSCRIPTEN__
  constexpr
  #else
  const
  #endif
  double lower_bound = static_cast<double>( std::nexttoward(
    std::numeric_limits<To>::min(), std::numeric_limits<double>::infinity()
  ) );


  #ifndef __EMSCRIPTEN__
  constexpr
  #else
  const
  #endif
  double upper_bound = static_cast<double>( std::nexttoward(
    std::numeric_limits<To>::max(), -std::numeric_limits<double>::infinity()
  ) );

  return static_cast<To>( std::clamp(
    static_cast<double>( uitsl::nan_to_zero( from ) ),
    lower_bound,
    upper_bound
  ) );
}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_CLAMP_CAST_HPP_INCLUDE
