#pragma once

#include "base/assert.h"
#include "tools/string_utils.h"

#include "../config.h"
#include "../../utility/print_utils.h"

template<typename T, size_t N>
class Duct;

template<typename T, size_t N=DEFAULT_BUFFER>
class HeadTailDuct {

  friend Duct<T, N>;

  struct alignas(CACHE_LINE_SIZE) padded {
    T t;
    std::string ToString() const { return emp::to_string(t); }
  };

  struct alignas(CACHE_LINE_SIZE) buffer_t : public std::array<padded, N> { };

  alignas(CACHE_LINE_SIZE) size_t head{0};
  alignas(CACHE_LINE_SIZE) size_t tail{0};
  buffer_t buffer;

public:

  void Initialize(const size_t write_position) { ; }

  //todo rename
  void Push() {

    emp_assert(
      GetPending() < N,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("GetPending(): ",GetPending())
    );

    ++head;
  }

  //todo rename
  void Pop(const size_t count) {

    emp_assert(
      GetPending() >= count,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("GetPending(): ", GetPending()),
      emp::to_string("count: ", count)
    );

    tail+=count;
  }

  size_t GetPending() {
    // TODO handle wraparound case?
    return head - tail;
  }

  size_t GetAvailableCapacity() { return N - GetPending(); }

  T GetElement(const size_t n) const { return buffer[n].t; }

  const void * GetPosition(const size_t n) const { return &buffer[n].t; }

  void SetElement(const size_t n, const T & val) { buffer[n].t = val; }

  std::string GetType() const { return "HeadTailDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("size_t head", head);
    ss << format_member("size_t tail", tail);
    return ss.str();
  }


};
