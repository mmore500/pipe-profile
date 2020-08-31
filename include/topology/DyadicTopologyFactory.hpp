#pragma once

#include "../../third-party/Empirical/source/base/vector.h"

#include "TopoEdge.hpp"
#include "Topology.hpp"
#include "TopoNode.hpp"

namespace uit {

uit::Topology make_dyadic_topology(const size_t cardinality) {

  uit::Topology res;

  size_t edge_counter{};

  for (size_t dyad = 0; dyad < cardinality/2; ++dyad) {

      uit::TopoEdge forward_edge{edge_counter++};
      uit::TopoEdge backward_edge{edge_counter++};

      res.push_back(uit::TopoNode{
        {backward_edge.GetOutlet()},
        {forward_edge.GetInlet()}
      });
      res.push_back(uit::TopoNode{
        {forward_edge.GetOutlet()},
        {backward_edge.GetInlet()}
      });

  }

  // for odd cardinality, add a loop pipe
  if (cardinality%2) {
    uit::TopoEdge self_edge{edge_counter++};

    res.push_back(uit::TopoNode{
      {self_edge.GetOutlet()},
      {self_edge.GetInlet()}
    });
  };

  return res;

}

struct DyadicTopologyFactory {
  uit::Topology operator()(const size_t cardinality) const {
    return make_dyadic_topology(cardinality);
  }
  uit::Topology operator()(const emp::vector<size_t> cardinality) const {
    emp_assert(cardinality.size() == 1);
    return make_dyadic_topology(cardinality.front());
  }
  static std::string GetName() { return "Dyadic Topology"; }
  // todo: rename GetSlug, look into making static
  std::string GetSimpleName() const { return "dyadic"; }
};

}
