#include <fairyland/fairyland.h>

#include <algorithm> // std::reverse
#include <queue>
#include <unordered_map>

ForestMap::ForestMap()
    : data(map_size, std::vector<CellStatus>(map_size, CellStatus::Unknown)){};

CellStatus &ForestMap::operator[](const Position &pos) {
  if ((pos.x < -grid_size) || (pos.y < -grid_size) || (pos.x > grid_size) ||
      (pos.y > grid_size)) {
    throw std::runtime_error("Invalid position.");
  }
  return this->data[static_cast<size_t>(pos.y + grid_size)]
                   [static_cast<size_t>(pos.x + grid_size)];
}

const CellStatus &ForestMap::operator[](const Position &pos) const {
  if ((pos.x < -grid_size) || (pos.y < -grid_size) || (pos.x > grid_size) ||
      (pos.y > grid_size)) {
    throw std::runtime_error("Invalid position.");
  }
  return this->data[static_cast<size_t>(pos.y + grid_size)]
                   [static_cast<size_t>(pos.x + grid_size)];
}

void ForestMap::print() const {
  auto convert = [](CellStatus status) {
    switch (status) {
    case CellStatus::Unknown:
      return "?";
    case CellStatus::Free:
      return ".";
    default:
      return "#";
    }
  };

  for (const auto &row : this->data) {
    for (const auto &elem : row) {
      std::cout << convert(elem);
    }
    std::cout << std::endl;
  }
}

struct PositionHash {
  size_t operator()(const Position &pos) const {
    return ((uint64_t)pos.x) << 32 | (uint64_t)pos.y;
  }
};

std::optional<std::vector<Direction>>
ForestMap::find_shortest_path_from_origin(const Position &dst) const {
  std::unordered_map<Position, std::pair<Position, Direction>, PositionHash>
      explored;

  std::queue<Position> queue;

  Position origin{0, 0};
  explored.insert({origin, {origin, Direction::Pass}});
  queue.push(origin);

  // Breadth-first search
  while (!queue.empty()) {
    Position pos = queue.front();
    queue.pop();
    if ((dst.x == pos.x) && (dst.y == pos.y)) {
      break;
    }

    for (Direction direction : moving_directions) {
      Position next = pos;
      next += direction;
      if (((*this)[next] == CellStatus::Free) &&
          (explored.find(next) == explored.end())) {
        explored.insert({next, {pos, direction}});
        queue.push(next);
      }
    }
  }

  // recovering the shortest path
  std::vector<Direction> path;
  auto prev_itr = explored.find(dst);
  while ((prev_itr != explored.end()) &&
         (prev_itr->second.second != Direction::Pass)) {
    path.push_back(prev_itr->second.second);
    prev_itr = explored.find(prev_itr->second.first);
  }

  if (path.empty()) {
    return std::nullopt;
  }
  std::reverse(path.begin(), path.end());
  return path;
}

int ForestMap::smallest_known_x() const {
  for (int x = -ForestMap::grid_size; x <= ForestMap::grid_size; ++x) {
    bool has_known_cell = false;
    for (int y = -ForestMap::grid_size; y <= ForestMap::grid_size; ++y) {
      if ((*this)[{x, y}] != CellStatus::Unknown) {
        has_known_cell = true;
      }
    }
    if (has_known_cell) {
      return x;
    }
  }
  throw std::runtime_error("No cells are known.");
}

int ForestMap::smallest_known_y() const {
  for (int y = -ForestMap::grid_size; y <= ForestMap::grid_size; ++y) {
    bool has_known_cell = false;
    for (int x = -ForestMap::grid_size; x <= ForestMap::grid_size; ++x) {
      if ((*this)[{x, y}] != CellStatus::Unknown) {
        has_known_cell = true;
      }
    }
    if (has_known_cell) {
      return y;
    }
  }
  throw std::runtime_error("No cells are known.");
}

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

std::vector<std::vector<CellStatus>>
combine_maps(const ForestMap &lhs, const ForestMap &rhs,
             const std::optional<Position> &rhs_offset) {
  if (rhs_offset.has_value()) {
  }
}