#pragma once

#include "mpi.h"

#include <algorithm>
#include <array>

#include "base/assert.h"
#include "tools/string_utils.h"

#include "CircularIndex.h"
#include "identity.h"

#include "config_utils.h"
#include "mpi_utils.h"
#include "print_utils.h"

template<typename T, size_t N>
class Duct;

// TODO rename ProcOutletDuct
template<typename T, size_t N=DEFAULT_BUFFER>
class IRecvDuct {

  friend Duct<T, N>;

  using pending_t = size_t;
  using buffer_t = std::array<T, N>;
  using index_t = CircularIndex<N>;

  //TODO fix all these damn mutables
  mutable pending_t pending{0};
  mutable buffer_t buffer;

  mutable std::array<MPI_Request, N> receive_requests;
#ifndef NDEBUG
  // most vexing parse :/
  mutable std::vector<char> request_states=std::vector<char>(N, false);
#endif

  MPI_Comm comm;

  const int inlet_proc;
  const int tag;

  mutable index_t receive_position{0};

  void RequestReceive() const {
    emp_assert(
      request_states[receive_position] == false,
      receive_position,
      pending
    );
    verify(MPI_Irecv(
      &buffer[receive_position],
      1,
      MPI_BYTE, // TODO template on T
      inlet_proc,
      tag,
      comm,
      &receive_requests[receive_position]
    ));
#ifndef NDEBUG
    request_states[receive_position] = true;
#endif
    ++receive_position;
  }

  // AcceptReceive Take
  bool ConfirmReceive() const {

    int flag{};

    emp_assert(
      request_states[receive_position],
      [](){ error_message_mutex.lock(); return "locked"; }(),
      receive_position,
      pending,
      format_member("*this", *this)
    );
    verify(MPI_Test(
      &receive_requests[receive_position],
      &flag,
      MPI_STATUS_IGNORE
    ));
#ifndef NDEBUG
    request_states[receive_position] = false;
#endif

    if (flag) {
      RequestReceive();
      ++pending;
    }

    return flag;

  }

public:

  IRecvDuct(
    const int inlet_proc_=MPI_ANY_SOURCE,
    const int tag_=MPI_ANY_TAG,
    MPI_Comm comm_=MPI_COMM_WORLD
  ) : comm(comm_)
  , inlet_proc(inlet_proc_)
  , tag(tag_) {
    for (size_t i = 0; i < N; ++i) RequestReceive();
    emp_assert(
      std::all_of(
        std::begin(request_states),
        std::end(request_states),
        identity
      ),
      [](){ error_message_mutex.lock(); return "locked"; }(),
      format_member("*this", *this)
    );
  }

  void Initialize(const size_t write_position) {
    for (size_t i = 0; i < write_position; ++i) {
      emp_assert(
        request_states[receive_position],
        [](){ error_message_mutex.lock(); return "locked"; }(),
        receive_position,
        format_member("*this", *this)
      );

      verify(MPI_Request_free(&receive_requests[i]));

#ifndef NDEBUG
      request_states[receive_position] = false;
#endif
      RequestReceive();
    }
  }

  //todo rename
  [[noreturn]] void Push() { throw "bad Push on IRecvDuct"; }

  //todo rename
  void Pop(const size_t count) {

    emp_assert(
      pending >= count,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      pending,
      count,
      format_member("*this", *this)
    );

    pending -= count;

  }

  size_t GetPending() const {
    while (ConfirmReceive());
    return pending;
  }

  [[noreturn]] size_t GetAvailableCapacity() const {
    throw "bad GetAvailableCapacity on IRecvDuct";
  }

  T GetElement(const size_t n) const { return buffer[n]; }

  const void * GetPosition(const size_t n) const { return &buffer[n]; }

  [[noreturn]] void SetElement(const size_t n, const T & val) {
    throw "bad SetElement on IRecvDuct";
  }

  std::string GetType() const { return "IRecvDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending", (size_t) pending) << std::endl;
    ss << format_member(
      "MPI_Comm comm",
      [this](){
        int len;
        char data[MPI_MAX_OBJECT_NAME];
        // TODO at least log/warn error codes
        verify(MPI_Comm_get_name(comm, data, &len));
        return std::string{}.assign(data, len);
      }()
    ) << std::endl;
    ss << format_member("int inlet_proc", inlet_proc) << std::endl;
    ss << format_member("int get_rank()", get_rank()) << std::endl;
    ss << format_member("int tag", tag) << std::endl << std::endl;
    ss << format_member("size_t receive_position", receive_position);
    return ss.str();
  }

};
