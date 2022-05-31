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

  if (!elena_pos.has_value()) {
    std::cout << "Ivan cannot reach Elena." << std::endl;
  } else {
    auto shortest_path =
        map_ivan.find_shortest_path_from_origin(elena_pos.value()).value();

    bool met{false};
    for (size_t idx = 0; 2 * idx <= shortest_path.size(); ++idx) {
      Direction ivan_move = shortest_path[idx];
      Direction elena_move =
          inverse_direction(shortest_path[shortest_path.size() - 1 - idx]);
      met = fairyland.go(ivan_move, elena_move);
      if (met) {
        break;
      }
    }

    if (met) {
      std::cout << "Ivan and Elena met in " << fairyland.getTurnCount()
                << " moves." << std::endl;
    } else {
      std::cout << "Ivan still cannot reach Elena." << std::endl;
    }
  }

  auto final_map = combine_maps(map_ivan, map_elena, elena_pos);
  print_map(final_map);

  return 0;
}
