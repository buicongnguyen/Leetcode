#include "dsa_atlas/algorithms.hpp"

#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dsa_atlas;

int main() {
  const auto pair = two_sum({3, 3}, 6);
  assert(pair && pair->first == 0 && pair->second == 1);
  assert(!two_sum({1, 2}, 9));
  assert(!two_sum({INT_MIN}, INT_MAX));

  assert(count_subarrays_sum({1, -1, 1, -1}, 0) == 4);
  assert((max_sliding_window({1, 3, -1, -3, 5, 3, 6, 7}, 3) ==
          std::vector<int>{3, 3, 5, 5, 6, 7}));

  const std::vector<int> sorted{1, 3, 3, 7};
  assert(binary_search_index(sorted, 7) == 3);
  assert(binary_search_index(sorted, 6) == -1);
  assert(first_true(4, [&](int index) { return sorted[index] >= 3; }) == 1);

  assert((bfs_distances({{1}, {0, 2}, {1}, {}}, 0) ==
          std::vector<int>{0, 1, 2, -1}));
  const WeightedGraph graph{
      {{1, 5}, {2, 1}}, {{3, 1}}, {{1, 1}, {3, 9}}, {}};
  assert((dijkstra(graph, 0) == std::vector<long long>{0, 2, 1, 3}));

  const auto order = topological_order({{1, 2}, {3}, {3}, {}});
  assert(order.size() == 4);
  bool cycle_rejected = false;
  try {
    static_cast<void>(topological_order({{1}, {0}}));
  } catch (const std::invalid_argument&) {
    cycle_rejected = true;
  }
  assert(cycle_rejected);

  assert((find_bridges(5, {{0, 1}, {1, 2}, {2, 0}, {3, 4}}) ==
          std::vector<std::pair<int, int>>{{3, 4}}));
  assert(find_bridges(2, {{0, 1}, {0, 1}}).empty());

  const auto subsets = unique_subsets({2, 1, 2});
  assert(subsets.size() == 6);
  assert(knapsack_01(4, {{2, 3}, {2, 3}, {3, 5}}) == 6);

  DisjointSet groups(4);
  assert(groups.unite(0, 1));
  assert(!groups.unite(0, 1));
  assert(groups.find(0) != groups.find(2));

  MedianFinder medians;
  bool empty_rejected = false;
  try {
    static_cast<void>(medians.median());
  } catch (const std::logic_error&) {
    empty_rejected = true;
  }
  assert(empty_rejected);
  medians.add(INT_MAX);
  medians.add(INT_MAX);
  assert(medians.median() == static_cast<double>(INT_MAX));

  std::cout << "All C++ algorithm tests passed.\n";
  return 0;
}
