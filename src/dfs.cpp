#include <fairyland/fairyland.h>

#include <array> // std::array
#include <iostream>
#include <stack> // std::stack

static constexpr std::array<Direction, 4> directions{
    Direction::Up, Direction::Right, Direction::Down, Direction::Left};

struct DirectionStatus {
  DirectionStatus(Direction direction, bool is_free, bool move, bool exploring)
      : direction{direction}, is_free{is_free}, move{move}, exploring{
                                                                exploring} {};

  Direction direction;
  bool is_free;
  bool move;
  bool exploring;
};

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

static void move_character(Fairyland &fairyland, Character name,
                           Direction direction) {
  switch (name) {
  case Character::Ivan:
    fairyland.go(direction, Direction::Pass);
    return;
  case Character::Elena:
    fairyland.go(Direction::Pass, direction);
    return;
  default:
    throw std::runtime_error("Invalid character name.");
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
  return this->data[static_cast<size_t>(pos.x + grid_size)]
                   [static_cast<size_t>(pos.y + grid_size)];
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

ForestMap deep_first_search(Fairyland &fairyland, Character name) {
  auto map = ForestMap();
  Position current_position{0, 0};

  std::stack<DirectionStatus> stack;
  stack.emplace(Direction::Pass, true, false, false);

  while (!stack.empty()) {
    DirectionStatus state = stack.top();
    stack.pop();
    // std::cout << "(" << current_position.x << ", " << current_position.y
    //           << "), direction=" << static_cast<char>(state.direction)
    //           << ", is_free=" << state.is_free << ", move=" << state.move
    //           << "\n";

    current_position += state.direction;
    if ((state.exploring) && (map[current_position] != CellStatus::Unknown)) {
      current_position += inverse_direction(state.direction);
    } else if (state.move) {
      move_character(fairyland, name, state.direction);
    }

    if (map[current_position] == CellStatus::Unknown) {
      map[current_position] =
          state.is_free ? CellStatus::Free : CellStatus::Wall;
      stack.emplace(inverse_direction(state.direction), true, state.is_free,
                    false);

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
  }

  return map;
}