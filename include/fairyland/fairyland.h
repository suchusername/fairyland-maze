#pragma once

#include <fairytale/fairy_tail.hpp>

enum class CellStatus { Unknown, Free, Wall };

struct Position {
  int x;
  int y;
};

class ForestMap {
public:
  ForestMap();

  CellStatus &operator[](const Position &pos);
  const CellStatus &operator[](const Position &pos) const;

  void print() const;

private:
  static constexpr int grid_size{10};
  static constexpr int map_size{2 * grid_size + 1};
  std::vector<std::vector<CellStatus>> data;
};

ForestMap deep_first_search(Fairyland &fairyland, Character name);
