#pragma once
#ifndef UITSL_DATASTRUCTS_PODLEAFNODE_HPP_INCLUDE
#define UITSL_DATASTRUCTS_PODLEAFNODE_HPP_INCLUDE

#include "../../../third-party/Empirical/source/base/array.h"

namespace uitsl {

template<typename T, size_t N=1>
class PodLeafNode {

  emp::array<T, N> data{};

public:

  PodLeafNode() = default;

  PodLeafNode(const T& other) {
    emp_assert( N == 1 );
    data[0] = other;
  }

  static constexpr size_t GetSize() { return N; }

  /*
   * Access value by index at compile time.
   */
  template<size_t Query>
  constexpr T& GetByIndex() { return data.at(Query); }

  /*
   * Access value by index at compile time.
   */
  template<size_t Query>
  constexpr T& Get() { return data.at(Query); }

  /*
   * Access value by index at compile time.
   */
  template<size_t Query>
  constexpr const T& Get() const { return data.at(Query); }

  /*
   * Access value by index at runtime.
   */
  constexpr T& Get(const size_t query=0) { return data.at(query); }

  /*
   * Access value by index at runtime.
   */
  constexpr const T& Get(const size_t query=0) const { return data.at(query); }

  /*
   * Access value.
   * Template oncluded for compatibility with other components
   */
  template<typename Query=T>
  constexpr T& Get() {
    static_assert( std::is_same<Query, T>::value );
    return data.at(0);
  }

  /*
   * Access value.
   * Template oncluded for compatibility with other components
   */
  template<typename Query=T>
  constexpr const T& Get() const {
    static_assert( std::is_same<Query, T>::value );
    return data.at(0);
  }


  /*
   * Does leaf array contain type?
   */
  template<typename Query>
  static constexpr bool HasType() { return std::is_same<T, Query>(); }


  /*
   * Equality operator overload.
   */
  bool operator==(const PodLeafNode& other) const {
    return data == other.data;
  }

  /*
   * Set data to value-initialized state.
   */
  void Reset() { data.fill( T{} ); }

  operator T&() { emp_assert( N == 1 ); return Get(); }

  operator const T&() const { emp_assert( N == 1 ); return Get(); }

  PodLeafNode& operator=(const T& other) {
    emp_assert( N == 1 );
    data[0] = other;
    return *this;
  }

  auto begin() { return std::begin( data ); }

  auto end() { return std::end( data ); }

  auto begin() const { return std::begin( data ); }

  auto end() const { return std::end( data ); }

  const emp::array<T, N>& GetBuffer() const { return data; }

};

} // namespace uitsl

#endif // #ifndef UITSL_DATASTRUCTS_PODLEAFNODE_HPP_INCLUDE
