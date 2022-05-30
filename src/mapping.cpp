#include <fairyland/fairyland.h>

static bool are_maps_equal(const ForestMap &lhs, const ForestMap &rhs,
                                const Position &rhs_offset) {
  return false;
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