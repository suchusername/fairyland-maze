#include <fairyland/fairyland.h>

#include <iostream>
#include <stack> // std::stack

bool Position::operator==(const Position &other) const {
  return (this->x == other.x) && (this->y == other.y);
}

Position &Position::operator+=(Direction direction) {
  switch (direction) {
  case Direction::Up:
    this->y--;
    break;
  case Direction::Right:
    this->x++;
    break;
  case Direction::Down:
    this->y++;
    break;
  case Direction::Left:
    this->x--;
    break;
  default:
    break;
  }
  return *this;
}

Direction inverse_direction(Direction direction) {
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

DepthFirstSearch::State::State(Direction direction, bool is_free, bool move,
                               bool exploring)
    : direction{direction}, is_free{is_free}, move{move}, exploring{
                                                              exploring} {};

DepthFirstSearch::DepthFirstSearch(const Fairyland &fairyland, Character name)
    : stack{}, map(), current_position{0, 0},
      state(Direction::Pass, true, false, false), fairyland{fairyland},
      name{name} {};

ForestMap DepthFirstSearch::get_map_copy() const { return this->map; }

void DepthFirstSearch::process_state(const State &state) {
  map[current_position] = state.is_free ? CellStatus::Free : CellStatus::Wall;
  stack.emplace(inverse_direction(state.direction), true, state.is_free, false);

  if (state.is_free) {
    for (Direction direction : moving_directions) {
      Position new_position = current_position;
      new_position += direction;
      if (direction != inverse_direction(state.direction)) {
        bool is_free = fairyland.canGo(name, direction);
        stack.emplace(direction, is_free, is_free, true);
      }
    }
  }
}

Direction DepthFirstSearch::get_move() {
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
