#pragma once

#include "uitsl/algorithm/unset_mask.hpp"

#include "typedef.hpp"

struct Cardinal {

  using input_t = node_t::input_t;
  input_t input;

  using output_t = node_t::output_t;
  output_t output;

  message_t learned_bits{};

public:

  Cardinal(
    const node_t::input_t& input_,
    const node_t::output_t& output_
  ) : input( input_ )
  , output( output_ )
  {}

  void ProcessIncomingBits( message_t& known_bits ) {

    const size_t num_known_bits = known_bits.CountOnes();

    // remove learned bits from known bits
    known_bits = uitsl::unset_mask(known_bits, learned_bits);

    emp_assert(
      learned_bits.CountOnes() == 0
      || known_bits.CountOnes() < num_known_bits
    );

    const message_t incoming_bits = input.JumpGet();

    // learned bits are incoming bits that we didn't already know about
    learned_bits = incoming_bits & ( ~known_bits );

    // add learned bits to known bits
    known_bits |= learned_bits;

  }

  void PushKnownBits( const message_t& known_bits ) {

    // remove learned bits from outgoing bits
    const message_t outgoing_bits
      = uitsl::unset_mask( known_bits, learned_bits );

    output.TryPut( outgoing_bits );

  }

};
