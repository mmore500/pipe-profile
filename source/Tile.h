#pragma once

#include <assert.h>
#include <sstream>

#include "Inlet.h"
#include "Outlet.h"
#include "print_utils.h"

class Tile {

  Inlet<char> inlet;
  Outlet<char> outlet;

  char state;

  char Transition(const char& neighbor_state) {

    // sequence a: x -> X -> x
    // sequence b: o -> O -> o
    // sequence c: _ -> _
    switch (neighbor_state) {
      case 'o':
        return 'O';
      case 'O':
        return 'O';
      case 'x':
        return 'X';
      case 'X':
        return 'x';
      case '_':
        return '_';
      default:
        error_message_mutex.lock();
        std::cerr
          << format_member("neighbor_state", neighbor_state)
          << std::endl;
        std::cerr << format_member("next Tile", *next) << std::endl;
        std::cerr << format_member("this Tile", *this) << std::endl;
        std::cerr << format_member("prev Tile", *prev) << std::endl;
        exit(1);
    }

  }

  void DoSetState(const char& state_) {state = state_;}

  void FlushState() {
    inlet.MaybePut(state);
  }

public:
  Tile(Inlet<char> inlet_, Outlet<char> outlet_)
  : inlet(inlet_), outlet(outlet_)
  { SetState('_'); }

  Tile *next;
  Tile *prev;
  size_t id;

  void Update() {
    const char neighbor_state = outlet.GetCurrent();
    const char next_state = Transition(neighbor_state);
    SetState(next_state);
  }

  const char& GetState() const { return state; }

  void SetState(const char& state) {
    DoSetState(state);
    FlushState();
  }

  size_t GetReadCount() const { return outlet.ReadOdometer(); }

  size_t GetWriteCount() const { return inlet.ReadOdometer(); }

  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("id", id) << std::endl;
    ss << format_member("Inlet<char> inlet", inlet) << std::endl;
    ss << format_member("Outlet<char> outlet", outlet) << std::endl;
    ss << format_member("char state", state);
    return ss.str();
  }

};
