#include "../DuctBenchmarkRegistration.h"
#include "utility/ScopeGuard.h"
#include "distributed/MPIGuard.h"

uit::ForEach<
  ThreadCountPayload,
  2
> range{};

// initialize
const uit::MPIGuard mpi_guard;
const uit::ScopeGuard register_benchmarks( [](){ range.item<1>(); } );

int main(int argc, char** argv) {

  // suppress json output for non-root procs
  int one{1};
  benchmark::Initialize(
    uit::is_root() ? &argc : &one,
    argv
  );

  benchmark::RunSpecifiedBenchmarks();

}