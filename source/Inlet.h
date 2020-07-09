#pragma once

#include <iostream>
#include <utility>
#include <memory>
#include <atomic>

#include "print_utils.h"
#include "Duct.h"

template<typename T, size_t N=1024>
class Inlet {

  using buffer_t = std::array<T, N>;

  std::shared_ptr<Duct<T,N>> duct;
  size_t write_position{0};
  size_t odometer{0};

  const pending_t & GetPending() const { return duct->GetPending(); }

  buffer_t & GetBuffer() { return duct->GetBuffer(); }

  const buffer_t & GetBuffer() const { return duct->GetBuffer(); }

  void Advance() {
    write_position = (write_position + 1) % N;
    duct->Push();
    ++odometer;
  }

  void DoPut(const T& val) {
    GetBuffer().at(write_position) = val;
    Advance();
  }

public:
  Inlet(
    std::shared_ptr<Duct<T,N>> duct_
  ) : duct(duct_) { ; }

  // potentially blocking
  void Put(const T& val) {

    while (GetPending() == N - 1);

    DoPut(val);

  }

  // non-blocking
  bool MaybePut(const T& val) {

    if (GetPending() == N - 1) return false;

    DoPut(val);

    return true;

  }

  size_t ReadOdometer() const { return odometer; }

  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("std::shared_ptr<Duct<T,N>> duct", *duct) << std::endl;
    ss << format_member(
      "GetBuffer().at(write_position - 1)",
      GetBuffer().at((write_position + N - 1) % N)
    ) << std::endl;
    ss << format_member(
      "GetBuffer().at(write_position)",
      GetBuffer().at(write_position)
    ) << std::endl;
    ss << format_member(
      "GetBuffer().at(write_position + 1)",
      GetBuffer().at((write_position + 1) % N)
    ) << std::endl;

    ss << format_member("size_t write_position", write_position) << std::endl;
    ss << format_member("size_t odometer", odometer);
    return ss.str();
  }


};
