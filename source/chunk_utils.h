#pragma once

#include "grid_utils.h"

#include "Tile.h"

using grid_t = std::vector<Tile>;
using handle_t = grid_t::iterator;
using chunk_t = std::vector<handle_t>;


chunk_t make_chunk(handle_t begin, handle_t end) {

  std::vector<handle_t> res(std::distance(begin, end));
  std::iota(
    std::begin(res),
    std::end(res),
    begin
  );

  return res;

}

std::vector<chunk_t> make_chunks(grid_t & grid, const size_t num_chunks) {

  const size_t chunk_width = grid.size() / num_chunks;

  assert(grid.size() % num_chunks == 0);

  std::vector<chunk_t> res;

  for (
    auto [chunk_begin, chunk_end] = std::tuple{std::begin(grid), handle_t{}};
    chunk_begin != std::end(grid);
    chunk_begin = chunk_end
  ) {

    chunk_end = (
      std::distance(chunk_end, std::end(grid)) < chunk_width
      ? std::end(grid)
      : std::next(chunk_begin, chunk_width)
    );

    res.push_back(
      make_chunk(chunk_begin, chunk_end)
    );

  }

  assert(res.size() == num_chunks);

  return res;
}

void update_chunk(
  chunk_t & chunk,
  const bool verbose,
  const size_t resistance
) {


  std::random_shuffle(
    std::begin(chunk),
    std::end(chunk)
  );


  for (auto & handle : chunk) {
    for (size_t i = 0; i < resistance; ++i) { volatile int do_not_optimize{}; }
    if (verbose) std::cout << handle->GetState();
    handle->Update();
  }
  if (verbose) std::cout << std::endl;


}
