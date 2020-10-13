#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/put=growing+get=stepping+type=any/a::UnboundedMoodyCamelDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::UnboundedMoodyCamelDuct,
  uit::a::UnboundedMoodyCamelDuct,
  uit::ThrowDuct
>;

#include "../SteppingThreadDuct.hpp"
#include "../ThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
