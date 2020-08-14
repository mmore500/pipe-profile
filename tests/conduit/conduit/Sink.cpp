#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "distributed/MultiprocessReporter.hpp"

#include "distributed/MPIGuard.hpp"
#include "conduit/Sink.hpp"
#include "conduit/ImplSpec.hpp"

const uit::MPIGuard guard;

TEST_CASE("Test Sink") {

  // TODO flesh out stub test
  uit::Sink<uit::ImplSpec<char>> sink;
  sink.get<0>();

  auto& [inlet] = sink;

}
