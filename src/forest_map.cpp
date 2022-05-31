#include <fairyland/fairyland.h>

static bool are_maps_equal(const ForestMap &lhs, const ForestMap &rhs,
                           const Position &rhs_offset) {
  int x_min = std::max(-lhs.grid_size, rhs_offset.x - lhs.grid_size);
  int x_max = std::min(lhs.grid_size, lhs.grid_size + rhs_offset.x);
  int y_min = std::max(-lhs.grid_size, rhs_offset.y - lhs.grid_size);
  int y_max = std::min(lhs.grid_size, lhs.grid_size + rhs_offset.y);
  for (int x = x_min; x <= x_max; ++x) {
    for (int y = y_min; y <= y_max; ++y) {
      if (lhs[{x, y}] != rhs[{x - rhs_offset.x, y - rhs_offset.y}]) {
        return false;
      }
    }
  }
  return true;
}

std::optional<Position> find_offset_between_maps(const ForestMap &lhs,
                                                 const ForestMap &rhs) {

  constexpr int min_offset{1 - ForestMap::grid_size};
  constexpr int max_offset{ForestMap::grid_size - 1};

  for (int x = min_offset; x <= max_offset; ++x) {
    for (int y = min_offset; y <= max_offset; ++y) {
      bool are_equal = are_maps_equal(lhs, rhs, {x, y});
      if (are_equal) {
        return Position{x, y};
      }
    }
  }

  return std::nullopt;
}