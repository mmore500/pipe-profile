#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/backend/impl/AggregatorSpec.hpp"
#include "uit/conduit/proc/backend/impl/InletMemoryAggregator.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.hpp"
#include "uit/mpi/MpiGuard.hpp"
#include "uit/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

TEST_CASE("Test InletMemoryAggregator") {

  // TODO flesh out stub test
  using AggregatorSpec = uit::AggregatorSpec<
    uit::ImplSpec<char>,
    uit::c::IriOiDuct
  >;
  uit::InletMemoryAggregator<AggregatorSpec>{};

}
