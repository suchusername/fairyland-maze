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

int ForestMap::biggest_known_x() const {
  for (int x = ForestMap::grid_size; x >= -ForestMap::grid_size; --x) {
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

int ForestMap::biggest_known_y() const {
  for (int y = ForestMap::grid_size; y >= -ForestMap::grid_size; --y) {
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

void ForestMap::print() const { print_map(this->data); }

std::vector<std::vector<CellStatus>>
ForestMap::get_subgrid_copy(int x_min, int y_min, int width, int height) const {
  std::vector<std::vector<CellStatus>> subgrid(
      static_cast<size_t>(height),
      std::vector<CellStatus>(static_cast<size_t>(width)));
  for (int x = x_min; x < x_min + width; ++x) {
    for (int y = y_min; y < y_min + height; ++y) {
      subgrid[static_cast<size_t>(y - y_min)][static_cast<size_t>(x - x_min)] =
          (*this)[{x, y}];
    }
  }
  return subgrid;
}

void print_map(const std::vector<std::vector<CellStatus>> &map) {
  for (const auto &row : map) {
    for (const auto &elem : row) {
      std::cout << static_cast<char>(elem);
    }
    std::cout << std::endl;
  }
}