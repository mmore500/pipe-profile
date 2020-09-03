#include <ratio>
#include <thread>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/intra/accumulating/AccumulatingDuct.hpp"
#include "uit/conduit/proc/accumulating/AccumulateWithdrawingDuct.hpp"
#include "uit/distributed/assign_utils.hpp"
#include "uit/distributed/MPIGuard.hpp"
#include "uit/distributed/MultiprocessReporter.hpp"
#include "uit/mesh/Mesh.hpp"
#include "uit/mesh/MeshNodeInput.hpp"
#include "uit/mesh/MeshNodeOutput.hpp"
#include "uit/topology/DyadicTopologyFactory.hpp"
#include "uit/topology/ProConTopologyFactory.hpp"
#include "uit/topology/RingTopologyFactory.hpp"

#define REPEAT for (size_t rep = 0; rep < std::deca{}.num; ++rep)

using ImplSel = uit::ImplSelect<
  uit::AccumulatingDuct,
  uit::ThrowDuct,
  uit::AccumulateWithdrawingDuct
>;

using MSG_T = int;
using Spec = uit::ImplSpec<MSG_T, DEFAULT_BUFFER, ImplSel>;

const uit::MPIGuard guard;


decltype(auto) make_dyadic_bundle() {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();
  REQUIRE( bundles.size() == 1 );

  return std::tuple{ bundles[0].GetInput(0), bundles[0].GetOutput(0) };

};

decltype(auto) make_producer_consumer_bundle() {

  uit::Mesh<Spec> mesh{
    uit::ProConTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh(0);

  REQUIRE( bundles.size() == 1 );
  REQUIRE(
    (bundles[0].GetInputOrNullopt(0) || bundles[0].GetOutputOrNullopt(0))
  );

  return std::tuple{
    bundles[0].GetInputOrNullopt(0),
    bundles[0].GetOutputOrNullopt(0)
  };

};

decltype(auto) make_ring_bundle() {
  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(uit::get_nprocs()),
    uit::AssignIntegrated<uit::thread_id_t>{},
    uit::AssignAvailableProcs{}
  };

  auto bundles = mesh.GetSubmesh();

  REQUIRE( bundles.size() == 1);

  return std::tuple{ bundles[0].GetInput(0), bundles[0].GetOutput(0) };

}

TEST_CASE("Is initial Get() result value-intialized?") { REPEAT {

  auto [input, output] = make_ring_bundle();

  REQUIRE( input.Get() == MSG_T{} );
  REQUIRE( input.JumpGet() == MSG_T{} );

} }

TEST_CASE("Unmatched gets") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) {
    REQUIRE( input.JumpGet() == MSG_T{} );
  }

  UIT_Barrier( MPI_COMM_WORLD );

  output.Put(42);
  while( input.JumpGet() != 42);

  REQUIRE( input.Get() == 42 );

  REQUIRE( input.JumpGet() == 0 );

  UIT_Barrier( MPI_COMM_WORLD );

} }

TEST_CASE("Unmatched puts") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  for (MSG_T i = 0; i <= 2 * DEFAULT_BUFFER; ++i) output.TryPut(1);

  UIT_Barrier( MPI_COMM_WORLD ); // todo why

} }

TEST_CASE("Validity") { REPEAT {

  auto [input, output] = make_dyadic_bundle();

  int sum{};
  // 1/2 n * (n + 1)
  const int expected_sum = (std::kilo{}.num - 1) * std::kilo{}.num / 2;
  for (MSG_T msg = 0; msg < std::kilo{}.num; ++msg) {

    output.TryPut(msg);

    const MSG_T received = input.JumpGet();
    REQUIRE( received <= expected_sum );
    REQUIRE( received >= 0 );
    sum += received;

  }

  while (sum != expected_sum) {
    const MSG_T received = input.JumpGet();
    REQUIRE( received <= expected_sum );
    REQUIRE( received >= 0);
    sum += received;
  }

  REQUIRE( sum == expected_sum );

  for (size_t i = 0; i < 10 * std::kilo{}.num; ++i) {
    REQUIRE( input.JumpGet() == 0 );
  }

  UIT_Barrier(MPI_COMM_WORLD); // todo why

} }
