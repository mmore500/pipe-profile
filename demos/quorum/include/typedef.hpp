#pragma once

#include <memory>

#include "netuit/mesh/Mesh.hpp"
#include "netuit/mesh/MeshNode.hpp"

#include "../third-party/Empirical/source/tools/BitSet.h"

using message_t = emp::BitSet<64>;

#include "ImplSpec.hpp"

using submesh_t = typename netuit::Mesh<ImplSpec>::submesh_t;

using node_t = netuit::MeshNode<ImplSpec>;
