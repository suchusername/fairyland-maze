#include <fairyland/fairyland.h>
#include <fairytale/fairy_tail.hpp>

int main() {
  Fairyland fairyland;

  DepthFirstSearch dfs_ivan(fairyland, Character::Ivan);
  DepthFirstSearch dfs_elena(fairyland, Character::Elena);

  for (size_t i = 0; i < 200; ++i) {
    Direction direction_ivan = dfs_ivan.get_move();
    Direction direction_elena = dfs_elena.get_move();
    fairyland.go(direction_ivan, direction_elena);
  }

  ForestMap map_ivan = dfs_ivan.get_map_copy();
  ForestMap map_elena = dfs_elena.get_map_copy();

  auto elena_pos = find_offset_between_maps(map_ivan, map_elena);

  if (elena_pos.has_value()) {
    std::cout << elena_pos->x << " " << elena_pos->y << std::endl;
  } else {
    std::cout << "no\n";
  }

  auto shortest_path =
      map_ivan.find_shortest_path_from_origin(elena_pos.value());
  for (Direction direction : shortest_path.value()) {
    std::cout << (char)direction;
  }
  std::cout << "\n";

  // map_ivan.print();
  // std::cout << "\n";
  // map_elena.print();

  return 0;
}
