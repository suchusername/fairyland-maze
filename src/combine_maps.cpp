#include <fairyland/fairyland.h>

static bool are_maps_equal(const ForestMap &lhs, const ForestMap &rhs,
                           const Position &rhs_offset) {
  int x_min =
      std::max(-ForestMap::grid_size, rhs_offset.x - ForestMap::grid_size);
  int x_max =
      std::min(ForestMap::grid_size, ForestMap::grid_size + rhs_offset.x);
  int y_min =
      std::max(-ForestMap::grid_size, rhs_offset.y - ForestMap::grid_size);
  int y_max =
      std::min(ForestMap::grid_size, ForestMap::grid_size + rhs_offset.y);
  for (int x = x_min; x <= x_max; ++x) {
    for (int y = y_min; y <= y_max; ++y) {
      if (lhs[{x, y}] != rhs[{x - rhs_offset.x, y - rhs_offset.y}]) {
        return false;
      }
    }
  }
  return true;
}

std::optional<Position> find_offset_between_equal_maps(const ForestMap &lhs,
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

static bool are_maps_mergable(const ForestMap &lhs, const ForestMap &rhs,
                              const Position &rhs_offset) {
  int x_min =
      std::max(-ForestMap::grid_size, rhs_offset.x - ForestMap::grid_size);
  int x_max =
      std::min(ForestMap::grid_size, ForestMap::grid_size + rhs_offset.x);
  int y_min =
      std::max(-ForestMap::grid_size, rhs_offset.y - ForestMap::grid_size);
  int y_max =
      std::min(ForestMap::grid_size, ForestMap::grid_size + rhs_offset.y);
  for (int x = x_min; x <= x_max; ++x) {
    for (int y = y_min; y <= y_max; ++y) {
      CellStatus lhs_val = lhs[{x, y}];
      CellStatus rhs_val = rhs[{x - rhs_offset.x, y - rhs_offset.y}];
      if (((lhs_val == CellStatus::Free) && (rhs_val != CellStatus::Unknown)) ||
          ((rhs_val == CellStatus::Free) && (lhs_val != CellStatus::Unknown)) ||
          ((lhs_val == CellStatus::Wall) && (rhs_val == CellStatus::Free)) ||
          ((rhs_val == CellStatus::Wall) && (lhs_val == CellStatus::Free))) {
        return false;
      }
    }
  }
  return true;
}

std::optional<Position>
find_offset_between_mergable_maps(const ForestMap &lhs, const ForestMap &rhs) {
  constexpr int min_offset{1 - ForestMap::grid_size};
  constexpr int max_offset{ForestMap::grid_size - 1};

  Position lhs_top_left{lhs.smallest_known_x() + 1, lhs.smallest_known_y() + 1};
  Position lhs_bot_right{lhs.biggest_known_x() - 1, lhs.biggest_known_y() - 1};
  Position rhs_top_left{rhs.smallest_known_x() + 1, rhs.smallest_known_y() + 1};
  Position rhs_bot_right{rhs.biggest_known_x() - 1, rhs.biggest_known_y() - 1};

  for (int x = min_offset; x <= max_offset; ++x) {
    for (int y = min_offset; y <= max_offset; ++y) {
      if ((rhs_bot_right.x + x - lhs_top_left.x >= ForestMap::grid_size) ||
          (rhs_bot_right.y + y - lhs_top_left.y >= ForestMap::grid_size) ||
          (lhs_bot_right.x - x - rhs_top_left.x >= ForestMap::grid_size) ||
          (lhs_bot_right.y - y - rhs_top_left.y >= ForestMap::grid_size)) {
        continue;
      }

      bool are_mergable = are_maps_mergable(lhs, rhs, {x, y});
      if (are_mergable) {
        return Position{x, y};
      }
    }
  }
  return std::nullopt;
}

void write_ivan_and_elena_positions(ForestMap &map_ivan,
                                    const Position &elena_offset) {
  int x_min = map_ivan.smallest_known_x() + 1;
  int y_min = map_ivan.smallest_known_y() + 1;
  Position ivan{-x_min, -y_min};
  Position elena{ivan.x + elena_offset.x, ivan.y + elena_offset.y};
  map_ivan[ivan] = CellStatus::Ivan;
  map_ivan[elena] = CellStatus::Elena;
}

void write_second_map_onto_first(ForestMap &lhs, const ForestMap &rhs,
                                 const std::optional<Position> &rhs_offset) {
  Position offset;
  if (!rhs_offset.has_value()) {
    // maps are not equal => they are guaranteed to be mergable

    offset = find_offset_between_mergable_maps(lhs, rhs).value();

    // writing second map onto first
    for (int lhs_x = -ForestMap::grid_size; lhs_x <= ForestMap::grid_size;
         ++lhs_x) {
      for (int lhs_y = -ForestMap::grid_size; lhs_y <= ForestMap::grid_size;
           ++lhs_y) {
        int rhs_x = lhs_x - offset.x;
        int rhs_y = lhs_y - offset.y;
        if ((std::abs(rhs_x) <= ForestMap::grid_size) &&
            (std::abs(rhs_y) <= ForestMap::grid_size) &&
            (rhs[{rhs_x, rhs_y}] != CellStatus::Unknown)) {
          lhs[{lhs_x, lhs_y}] = rhs[{rhs_x, rhs_y}];
        }
      }
    }
  } else {
    offset = rhs_offset.value();
  }

  write_ivan_and_elena_positions(lhs, offset);
}

std::vector<std::vector<CellStatus>> crop_map(const ForestMap &map) {
  int x_min = map.smallest_known_x() + 1;
  int y_min = map.smallest_known_y() + 1;
  auto cropped_map = map.get_subgrid_copy(x_min, y_min, ForestMap::grid_size,
                                          ForestMap::grid_size);
  return cropped_map;
}
