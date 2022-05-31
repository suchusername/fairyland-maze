#include <fairyland/fairyland.h>

#include <array> // std::array
#include <iostream>
#include <stack> // std::stack

static constexpr std::array<Direction, 4> directions{
    Direction::Up, Direction::Right, Direction::Down, Direction::Left};

static Position &operator+=(Position &pos, Direction direction) {
  switch (direction) {
  case Direction::Up:
    pos.y--;
    break;
  case Direction::Right:
    pos.x++;
    break;
  case Direction::Down:
    pos.y++;
    break;
  case Direction::Left:
    pos.x--;
    break;
  default:
    break;
  }
  return pos;
}

static Direction inverse_direction(Direction direction) {
  switch (direction) {
  case Direction::Up:
    return Direction::Down;
  case Direction::Right:
    return Direction::Left;
  case Direction::Down:
    return Direction::Up;
  case Direction::Left:
    return Direction::Right;
  default:
    return Direction::Pass;
  }
}

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

DeepFirstSearch::State::State(Direction direction, bool is_free, bool move,
                              bool exploring)
    : direction{direction}, is_free{is_free}, move{move}, exploring{
                                                              exploring} {};

DeepFirstSearch::DeepFirstSearch(const Fairyland &fairyland, Character name)
    : stack{}, map(), current_position{0, 0},
      state(Direction::Pass, true, false, false), fairyland{fairyland},
      name{name} {};

ForestMap DeepFirstSearch::get_map_copy() const { return this->map; }

void DeepFirstSearch::process_state(const State &state) {
  map[current_position] = state.is_free ? CellStatus::Free : CellStatus::Wall;
  stack.emplace(inverse_direction(state.direction), true, state.is_free, false);

  if (state.is_free) {
    for (Direction direction : directions) {
      Position new_position = current_position;
      new_position += direction;
      if (direction != inverse_direction(state.direction)) {
        bool is_free = fairyland.canGo(name, direction);
        stack.emplace(direction, is_free, is_free, true);
      }
    }
  }
}

Direction DeepFirstSearch::get_move() {
  if (map[current_position] == CellStatus::Unknown) {
    process_state(state);
  }

  while (!stack.empty()) {
    state = stack.top();
    stack.pop();

    current_position += state.direction;
    if ((state.exploring) && (map[current_position] != CellStatus::Unknown)) {
      current_position += inverse_direction(state.direction);
    } else if (state.move) {
      return state.direction;
    }

    if (map[current_position] == CellStatus::Unknown) {
      process_state(state);
    }
  }
  return Direction::Pass;
};
