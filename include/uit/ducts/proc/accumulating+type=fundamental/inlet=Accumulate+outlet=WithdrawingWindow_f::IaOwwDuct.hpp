#pragma once
#ifndef UIT_DUCTS_PROC_ACCUMULATING_TYPE_FUNDAMENTAL_INLET_ACCUMULATE_OUTLET_WITHDRAWINGWINDOW_F__IAOWWDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_ACCUMULATING_TYPE_FUNDAMENTAL_INLET_ACCUMULATE_OUTLET_WITHDRAWINGWINDOW_F__IAOWWDUCT_HPP_INCLUDE

#include <type_traits>

#include "../impl/inlet/accumulating+type=fundamental/f::AccumulateDuct.hpp"
#include "../impl/outlet/accumulating+type=fundamental/f::WithdrawingWindowDuct.hpp"

namespace uit {
namespace f {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IaOwwDuct {

  using InletImpl = uit::f::AccumulateDuct<ImplSpec>;
  using OutletImpl = uit::f::WithdrawingWindowDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace f
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_ACCUMULATING_TYPE_FUNDAMENTAL_INLET_ACCUMULATE_OUTLET_WITHDRAWINGWINDOW_F__IAOWWDUCT_HPP_INCLUDE
