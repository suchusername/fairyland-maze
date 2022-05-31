#pragma once

#include <fairytale/fairy_tail.hpp>

#include <array>
#include <optional>
#include <stack>
#include <vector>

enum class CellStatus { Unknown, Free, Wall };

constexpr std::array<Direction, 4> moving_directions{
    Direction::Up, Direction::Right, Direction::Down, Direction::Left};

Direction inverse_direction(Direction direction);

struct Position {
  int x;
  int y;

  bool operator==(const Position &other) const;
  Position &operator+=(Direction direction);
};

class ForestMap {
public:
  ForestMap();

  CellStatus &operator[](const Position &pos);
  const CellStatus &operator[](const Position &pos) const;

  std::optional<std::vector<Direction>>
  find_shortest_path_from_origin(const Position &dst) const;

  void print() const;

public:
  static constexpr int grid_size{10};
  static constexpr int map_size{2 * grid_size + 1};

private:
  std::vector<std::vector<CellStatus>> data;
};

class DepthFirstSearch {
public:
  DepthFirstSearch(const Fairyland &fairyland, Character name);
  ForestMap get_map_copy() const;

  Direction get_move();

private:
  struct State {
    State(Direction direction, bool is_free, bool move, bool exploring);
    Direction direction;
    bool is_free;
    bool move;
    bool exploring;
  };

  void process_state(const State &state);

private:
  std::stack<State> stack;
  ForestMap map;
  Position current_position;
  State state;

  const Fairyland &fairyland;
  const Character name;
};

std::optional<Position> find_offset_between_maps(const ForestMap &lhs,
                                                 const ForestMap &rhs);