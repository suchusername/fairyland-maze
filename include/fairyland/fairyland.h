#pragma once

#include <fairytale/fairy_tail.hpp>

#include <array>
#include <optional>
#include <stack>
#include <vector>

enum class CellStatus : char {
  Unknown = '?',
  Free = '.',
  Wall = '#',
  Ivan = '@',
  Elena = '&',
};

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

  int smallest_known_x() const;
  int smallest_known_y() const;
  int biggest_known_x() const;
  int biggest_known_y() const;
  std::vector<std::vector<CellStatus>>
  get_subgrid_copy(int x_min, int y_min, int width, int height) const;

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

std::vector<std::vector<CellStatus>>
combine_maps(ForestMap &lhs, const ForestMap &rhs,
             const std::optional<Position> &rhs_offset);

void print_map(const std::vector<std::vector<CellStatus>> &map);