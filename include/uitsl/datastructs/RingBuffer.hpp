#pragma once
#ifndef UITSL_DATASTRUCTS_RINGBUFFER_HPP_INCLUDE
#define UITSL_DATASTRUCTS_RINGBUFFER_HPP_INCLUDE

#include <algorithm>
#include <stddef.h>
#include <utility>

#include "../../../third-party/Empirical/source/base/array.h"

#include "../nonce/CircularIndex.hpp"

namespace uitsl {

template<typename T, size_t N>
class RingBuffer {

  emp::array<T, N> buffer{};
  uitsl::CircularIndex<N> tail{};
  size_t num_items{};

public:

  size_t GetSize() const { return num_items; }

  bool IsEmpty() const { return GetSize() == 0; }

  bool IsFull() const { return GetSize() == N; }
  
  void DoPushHead() { emp_assert( !IsFull() ); ++num_items; }
  
  bool PushHead() {
    if ( IsFull() ) return false;
    else {
      DoPushHead();
      return true;
    }
  }

  bool PushHead(const T& t) {
    if ( IsFull() ) return false;
    else {
      Get(num_items) = t;
      DoPushHead();
      return true;
    }
  }

  template<typename P>
  bool PushHead(P&& p) {
    if ( IsFull() ) return false;
    else {
      Get(num_items) = std::forward<P>( p );
      DoPushHead();
      return true;
    }
  }

  void DoPopTail(const size_t num_requested=1) {
    emp_assert( num_requested <= GetSize() );
    tail += num_popped;
    num_items -= num_popped;    
  }
  
  size_t PopTail(const size_t num_requested=1) {
    const size_t num_popped = std::min(num_requested, GetSize());
    DoPopTail( num_requested );
    return num_popped;
  }

  void Fill(const T& t=T{}) { while( PushHead(t) ); }

  const T& Get(const size_t i) const { return buffer[tail + i]; }

  T& Get(const size_t i) { return buffer[tail + i]; }

  T& GetHead() { return Get( GetSize() - 1 ); }

  const T& GetHead() const { return Get( GetSize() - 1 ); }

  T& GetTail() { return Get(0); }

  const T& GetTail() const { return Get(0); }

};

} // namespace uitsl

#endif // #ifndef UITSL_DATASTRUCTS_RINGBUFFER_HPP_INCLUDE
