#pragma once

#include <sstream>
#include <string>

#include "../../../third-party/Empirical/source/base/optional.h"
#include "../../../third-party/Empirical/source/base/vector.h"

#include "Cell.hpp"

class CellCollection {

  emp::vector<emp::optional<Cell>> cells;

public:

  CellCollection(
    const submesh_t& submesh,
    const size_t num_nodes
  ) : cells( submesh.size() ) {
    // this approach is a workaround for deleted copy constructor on Cell
    for (size_t i = 0; i < submesh.size(); ++i) {
      cells[i].emplace( submesh[i] );
    }
  }

  void Update() { for (auto& cell : cells) cell->Update(); }

  size_t GetNumMessagesSent() const {
    return std::accumulate(
      std::begin(cells),
      std::end(cells),
      0,
      [](const size_t a, const auto& cell){
        return a + cell->GetNumMessagesSent();
      }
    );
  }

  size_t GetNumMessagesReceived() const {
    return std::accumulate(
      std::begin(cells),
      std::end(cells),
      0,
      [](const size_t a, const auto& cell){
        return a + cell->GetNumMessagesReceived();
      }
    );
  }

  size_t GetSize() const { return cells.size(); }

  void DeactivateHalf() {
    std::for_each(
      std::begin( cells ) + cells.size() / 2,
      std::end( cells ),
      []( auto& cell ){ cell->Deactivate(); }
    );
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << "num_known_bits ";
    for (const auto& cell : cells) ss << cell->GetNumKnownBits() << " ";
    return ss.str();
  }

};
