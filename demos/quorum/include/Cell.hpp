#pragma once

#include <algorithm>
#include <memory>

#include "../../../third-party/Empirical/source/base/vector.h"

#include "uitsl/algorithm/unset_mask.hpp"
#include "uitsl/debug/safe_compare.hpp"

#include "Cardinal.hpp"
#include "typedef.hpp"

class Cell {

  size_t received_message_counter{};
  size_t sent_message_counter{};

  emp::vector< Cardinal > cardinals;

  using outputs_t = node_t::outputs_t;
  outputs_t outputs;

  message_t own_bits;

  message_t known_bits;

  message_t cur_blacklist{};
  message_t prev_blacklist{};

  void UpdateKnownBits() {
    for (auto& cardinal : cardinals) cardinal.ProcessIncomingBits( known_bits, cur_blacklist, prev_blacklist );
  }

  void PushKnownBits() {
    for (auto& cardinal : cardinals) cardinal.PushKnownBits( known_bits );
  }

public:

  Cell(const node_t& node) {

    std::transform(
      std::begin( node.GetInputs() ),
      std::end( node.GetInputs() ),
      std::begin( node.GetOutputs() ),
      std::back_inserter( cardinals ),
      []( const auto& input, const auto& output ){
        return Cardinal{ input, output };
      }
    );

    static emp::Random rand{};
    if ( rand.P( 1.0 ) ) own_bits.Set( rand.GetUInt( own_bits.GetSize() ) );

    known_bits = own_bits;
    emp_assert( (own_bits & known_bits).CountOnes() == 1 );

  }

  Cell & operator=(const Cell&) = delete;
  Cell(const Cell&) = delete;

  void CheckDistinctLearnedBits() {
    emp_assert( uitsl::safe_equal( std::accumulate(
      std::begin(cardinals),
      std::end(cardinals),
      message_t{},
      [](const auto& message, const auto& c2){
        return message | c2.learned_bits;
      }
    ).CountOnes(), std::accumulate(
      std::begin(cardinals),
      std::end(cardinals),
      0,
      [](const auto& count, const auto& c2){
        return count + c2.learned_bits.CountOnes();
      }
    ) ) );
  }

  void CheckOwnBits() {
    if ( own_bits.CountOnes() ) {
      emp_assert( (own_bits & known_bits).CountOnes() == 1 );
    }
  }

  void Update() {

    if (std::all_of(
      std::begin( cardinals ),
      std::end( cardinals ),
      []( const auto& cardinal ){ return cardinal.half_trip_counter > 3; }
    ) ) {
      for ( auto& cardinal : cardinals ) cardinal.half_trip_counter = 0;
      std::swap( cur_blacklist, prev_blacklist );
      cur_blacklist = {};
    }

    CheckOwnBits();
    CheckDistinctLearnedBits();

    UpdateKnownBits();

    CheckOwnBits();
    CheckDistinctLearnedBits();

    PushKnownBits();

    CheckOwnBits();
    CheckDistinctLearnedBits();

  }

  void Deactivate() {
    const size_t num_known_bits = known_bits.CountOnes();
    known_bits = uitsl::unset_mask( known_bits, own_bits );
    emp_assert(
      own_bits.CountOnes() == 0
      || known_bits.CountOnes() < num_known_bits
    );
    own_bits.Clear();
  }

  size_t GetNumMessagesSent() const { return sent_message_counter; }

  size_t GetNumMessagesReceived() const { return received_message_counter; }

  size_t GetNumKnownBits() const { return known_bits.CountOnes(); };

};
