#include <fairyland/fairyland.h>
#include <fairytale/fairy_tail.hpp>

int main() {
  Fairyland fairyland;

  DeepFirstSearch dfs_ivan(fairyland, Character::Ivan);
  DeepFirstSearch dfs_elena(fairyland, Character::Elena);

  for (size_t i = 0; i < 200; ++i) {
    Direction direction_ivan = dfs_ivan.get_move();
    Direction direction_elena = dfs_elena.get_move();
    fairyland.go(direction_ivan, direction_elena);
  }

  ForestMap map_ivan = dfs_ivan.get_map_copy();
  ForestMap map_elena = dfs_elena.get_map_copy();

  map_ivan.print();
  std::cout << "\n";
  map_elena.print();

  return 0;
}
