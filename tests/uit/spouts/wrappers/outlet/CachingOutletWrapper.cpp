#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

#include "uit/ducts/Duct.hpp"
#include "uit/setup/ImplSelect.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/spouts/Outlet.hpp"
#include "uit/spouts/wrappers/CachingSpoutWrapper.hpp"
#include "uit/spouts/wrappers/outlet/CachingOutletWrapper.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test CachingOutletWrapper") {

  using Spec = uit::ImplSpec<
    char,
    uit::ImplSelect<>,
    uit::CachingSpoutWrapper
  >;
  uit::internal::CachingOutletWrapper< uit::Outlet< Spec > >{
    std::make_shared<uit::internal::Duct<Spec>>()
  };

}
