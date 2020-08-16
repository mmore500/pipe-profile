#pragma once

#include <stddef.h>
#include <utility>
#include <variant>

#include "../../third-party/Empirical/source/base/assert.h"
#include "../../third-party/Empirical/source/meta/TypePack.h"
#include "../../third-party/Empirical/source/tools/string_utils.h"

#include "../distributed/mpi_utils.hpp"
#include "../utility/print_utils.hpp"

#include "config.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class Duct {

  using ducts_t = typename emp::TypePack<
    typename ImplSpec::IntraDuct,
    typename ImplSpec::ThreadDuct,
    typename ImplSpec::ProcInletDuct,
    typename ImplSpec::ProcOutletDuct
  >::make_unique;

  typename ducts_t::template apply<std::variant> impl;

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  /**
   * TODO.
   *
   * @return
   */
  std::string GetName() const {
    return emp::to_string(
      get_proc_id(),
      ":",
      GetUID()
    );
  }

public:

  /// TODO.
  using uid_t = std::uintptr_t;

  /**
   * TODO.
   *
   * @tparam WhichDuct TODO
   * @tparam Args TODO
   * @param args TODO
   */
  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    impl.template emplace<WhichDuct>(std::forward<Args>(args)...);
  }

  //todo refactor to remove this
  //keep write/read position *within* duct
  /**
   * TODO.
   *
   * @param write_position
   */
  void Initialize(const size_t write_position) {

    std::visit(
      [=](auto & arg) { arg.Initialize(write_position); },
      impl
    );

  }

  //todo rename
  /**
   * TODO.
   */
  void Push() {

    std::visit(
      [](auto & arg) { arg.Push(); },
      impl
    );

  }

  //todo rename
  /**
   * TODO.
   *
   * @param count TODO.
   */
  void Pop(const size_t count) {

    std::visit(
      [count](auto & arg) { arg.Pop(count); },
      impl
    );

  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetPending() {
    return std::visit(
      [](auto & arg) -> size_t { return arg.GetPending(); },
      impl
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetAvailableCapacity() {
    return std::visit(
      [](auto & arg) -> size_t { return arg.GetAvailableCapacity(); },
      impl
    );
  }

  // todo rename
  /**
   * TODO.
   *
   * @param n TODO.
   * @return TODO.
   */
  T GetElement(const size_t n) const {
    return std::visit(
      [n](auto & arg) -> T { return arg.GetElement(n); },
      impl
    );
  }

  /**
   * TODO.
   *
   * @param n TODO.
   * @return TODO.
   */
  const void * GetPosition(const size_t n) const {
    return std::visit(
      [n](auto & arg) -> const void * { return arg.GetPosition(n); },
      impl
    );
  }

  /**
   * TODO.
   *
   * @param n TODO.
   * @param val TODO.
   */
  void SetElement(const size_t n, const T & val) {
    std::visit(
      [n, &val](auto & arg) { arg.SetElement(n, val); },
      impl
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string GetType() const {
    return std::visit(
      [](auto & arg) -> std::string { return arg.GetType(); },
      impl
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  uid_t GetUID() const { return reinterpret_cast<uid_t>(this); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("GetName()", GetName()) << std::endl;
    ss << format_member(
      "std::variant impl",
      std::visit(
        [](auto & arg) -> std::string { return arg.ToString(); },
        impl
      )
    );
    return ss.str();
  }

};

} // namespace uit
