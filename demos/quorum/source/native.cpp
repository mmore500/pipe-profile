#include <chrono>
#include <iostream>
#include <map>
#include <sstream>

#include "netuit/assign/AssignAvailableProcs.hpp"
#include "uitsl/containers/safe/unordered_map.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/distributed/do_successively.hpp"
#include "uitsl/mpi/MpiMultithreadGuard.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"
#include "uitsl/utility/assign_utils.hpp"

#include "netuit/arrange/ToroidalTopologyFactory.hpp"

#include "Job.hpp"

const uitsl::MpiMultithreadGuard guard{};

constexpr size_t num_threads = 4;
const size_t num_procs = uitsl::safe_cast<size_t>( uitsl::get_nprocs() );

constexpr size_t nodes_per_job = 16;
const size_t num_nodes = num_procs * num_threads * nodes_per_job;

int main() {

  if ( uitsl::is_root() ) std::cout << ">>> begin <<<" << std::endl << std::endl;

  netuit::Mesh<ImplSpec> mesh{
    netuit::ToroidalTopologyFactory{}(
      { std::sqrt(num_nodes), std::sqrt(num_nodes) }
    ),
    uitsl::AssignRoundRobin<uitsl::thread_id_t>{ num_threads, nodes_per_job },
    uitsl::AssignContiguously<uitsl::proc_id_t>{ num_procs, num_nodes }
  };

  uitsl::safe::unordered_map<size_t, std::string> res;

  uitsl::ThreadTeam team;
  for (size_t thread = 0; thread < num_threads; ++thread) team.Add(
    [&mesh, &res, thread](){

      Job job{ mesh.GetSubmesh(thread), num_nodes };

      std::stringstream ss;

      job.Run();

      ss << "process " << uitsl::get_proc_id() << std::endl;
      ss << "thread " << thread << std::endl;
      ss << job.ToString() << std::endl;

      job.DeactivateHalf();

      job.Run();

      ss << "process " << uitsl::get_proc_id() << std::endl;
      ss << "thread " << thread << std::endl;
      ss << job.ToString() << std::endl;


      res[thread] = ss.str();

    }
  );
  team.Join();

  uitsl::do_successively(
    [&](){
      for (const auto& [k, v] : std::map{ std::begin(res), std::end(res) }) {
        std::cout << v;
      }
    },
    uitsl::print_separator
  );


  if ( uitsl::is_root() ) std::cout << ">>> end <<<" << std::endl;

  return 0;
}
