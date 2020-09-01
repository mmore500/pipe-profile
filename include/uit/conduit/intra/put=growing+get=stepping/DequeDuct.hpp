#pragma once

#include <deque>
#include <stddef.h>

#include "../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../third-party/Empirical/source/base/errors.h"
#include "../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../utility/print_utils.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class DequeDuct {

  using T = typename ImplSpec::T;

  std::deque<T> queue{ T{} };

  size_t CountUnconsumedGets() const {
    const size_t available = queue.size();
    emp_assert( available );
    return available - 1;
  }

public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) { queue.push_back( val ); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() const { return true; }

  /**
   * TODO.
   *
   * @param n TODO.
   */
  size_t TryConsumeGets(const size_t requested) {
    const size_t num_consumed = std::min( requested, CountUnconsumedGets() );
    queue.erase(
      std::begin(queue),
      std::next(std::begin(queue), num_consumed)
    );
    return num_consumed;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() { return queue.front(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "DequeDuct"; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    return ss.str();
  }

};

} // namespace uit
