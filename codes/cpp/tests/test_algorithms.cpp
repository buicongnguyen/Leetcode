#include "dsa_atlas/algorithms.hpp"

#include <algorithm>
#include <climits>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

using namespace dsa_atlas;

static_assert(!std::is_copy_constructible<LRUCache>::value,
              "LRUCache must not copy iterators into another list");
static_assert(std::is_move_constructible<LRUCache>::value,
              "LRUCache must remain movable");

namespace {

void expect(bool condition, const std::string& message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

template <class Exception, class Callable>
void expect_throws(Callable callable, const std::string& message) {
  try {
    callable();
  } catch (const Exception&) {
    return;
  }
  throw std::runtime_error(message);
}

}  // namespace

int main() {
  try {
    const std::vector<int> pair = two_sum({3, 3}, 6);
    expect(pair == std::vector<int>({0, 1}),
           "two_sum must use distinct indices");
    expect(two_sum({1, 2}, 9).empty(), "two_sum must report no solution");
    expect(two_sum({INT_MIN}, INT_MAX).empty(),
           "two_sum must avoid overflow matches");

    expect(count_subarrays_sum({1, -1, 1, -1}, 0) == 4,
           "prefix sums must count zero-sum ranges");
    expect((max_sliding_window({1, 3, -1, -3, 5, 3, 6, 7}, 3) ==
            std::vector<int>{3, 3, 5, 5, 6, 7}),
           "sliding-window maxima must match the fixture");

    const std::vector<int> sorted{1, 3, 3, 7};
    expect(binary_search_index(sorted, 7) == 3,
           "binary search must find a present value");
    expect(binary_search_index(sorted, 6) == -1,
           "binary search must reject an absent value");
    expect(first_true(4, [&](int index) { return sorted[index] >= 3; }) == 1,
           "first_true must find the first boundary");
    expect_throws<std::invalid_argument>(
        [] { static_cast<void>(first_true(-1, [](int) { return true; })); },
        "first_true must reject a negative domain");

    expect((bfs_distances({{1}, {0, 2}, {1}, {}}, 0) ==
            std::vector<int>{0, 1, 2, -1}),
           "BFS distances must include unreachable vertices");
    expect_throws<std::invalid_argument>(
        [] { static_cast<void>(bfs_distances({{1}}, 0)); },
        "BFS must reject an invalid neighbor");

    const WeightedGraph graph{
        {{1, 5}, {2, 1}}, {{3, 1}}, {{1, 1}, {3, 9}}, {}};
    expect((dijkstra(graph, 0) == std::vector<long long>{0, 2, 1, 3}),
           "Dijkstra distances must match the fixture");
    expect_throws<std::invalid_argument>(
        [] {
          const WeightedGraph invalid{{}, {{0, -1}}};
          static_cast<void>(dijkstra(invalid, 0));
        },
        "Dijkstra must reject negative edges in disconnected components");

    const auto order = topological_order({{1, 2}, {3}, {3}, {}});
    expect(order.size() == 4, "topological order must include every vertex");
    expect_throws<std::invalid_argument>(
        [] { static_cast<void>(topological_order({{1}, {0}})); },
        "topological order must reject cycles");

    expect((find_bridges(5, {{0, 1}, {1, 2}, {2, 0}, {3, 4}}) ==
            std::vector<std::pair<int, int>>{{3, 4}}),
           "bridge search must handle disconnected graphs");
    expect(find_bridges(2, {{0, 1}, {0, 1}}).empty(),
           "parallel edges must not be bridges");
    expect_throws<std::invalid_argument>(
        [] { static_cast<void>(find_bridges(2, {{0, 2}})); },
        "bridge search must reject invalid endpoints");

    expect(minimum_spanning_tree_weight(
               4, {{0, 1, 4}, {0, 2, 1}, {2, 1, 2}, {1, 3, 1}, {2, 3, 5}}) ==
               4,
           "Kruskal must return the minimum spanning-tree weight");
    expect(minimum_spanning_tree_weight(0, {}) == 0,
           "the empty graph must have zero spanning-tree weight");
    expect_throws<std::invalid_argument>(
        [] {
          static_cast<void>(
              minimum_spanning_tree_weight(3, {{0, 1, 1}}));
        },
        "Kruskal must reject a disconnected graph");

    const auto subsets = unique_subsets({2, 1, 2});
    expect(subsets.size() == 6, "unique subsets must skip duplicates");
    expect(knapsack_01(4, {{2, 3}, {2, 3}, {3, 5}}) == 6,
           "0-1 knapsack must use each item once");
    expect(max_non_adjacent_sum({3, 7, 4, 6, 5}) == 13,
           "1D DP must enforce the non-adjacent choice");
    expect(max_non_adjacent_sum({-8, -2, -5}) == 0,
           "1D DP must permit the empty choice");
    expect(longest_common_subsequence_length("abcde", "ace") == 3,
           "2D sequence DP must find the longest common subsequence");
    expect(longest_common_subsequence_length("", "abc") == 0,
           "2D sequence DP must handle an empty prefix");
    expect(minimum_coins({1, 2, 5}, 11) == 3,
           "unbounded coin change must reuse denominations");
    expect(minimum_coins({2}, 3) == -1,
           "coin change must report impossible amounts");
    expect(cherry_pickup_two_robots(
               {{3, 1, 1}, {2, 5, 1}, {1, 5, 5}, {2, 1, 1}}) == 24,
           "3D DP must track both robot columns");
    expect(count_distinct_digit_numbers(20) == 19,
           "digit DP must reject repeated digits");
    expect(count_distinct_digit_numbers(0) == 0,
           "digit DP must exclude the empty number");

    TreeNode fourth{4};
    TreeNode second{2, &fourth};
    TreeNode third{3};
    TreeNode root{1, &second, &third};
    expect(tree_height(&root) == 3,
           "tree height must follow the recursive subtree contract");
    expect(tree_height(nullptr) == 0, "an empty tree must have height zero");

    DisjointSet groups(4);
    expect(groups.unite(0, 1), "disjoint set must merge separate groups");
    expect(!groups.unite(0, 1), "disjoint set must detect existing groups");
    expect(groups.find(0) != groups.find(2),
           "disjoint set must preserve separate groups");
    expect_throws<std::invalid_argument>(
        [&] { static_cast<void>(groups.find(-1)); },
        "disjoint set must reject invalid items");

    MedianFinder medians;
    expect_throws<std::logic_error>(
        [&] { static_cast<void>(medians.median()); },
        "median must be undefined for an empty stream");
    medians.add(INT_MAX);
    medians.add(INT_MAX);
    expect(medians.median() == static_cast<double>(INT_MAX),
           "median averaging must avoid overflow");

    LRUCache cache(2);
    cache.put(1, 10);
    cache.put(2, 20);
    expect(cache.get(1) == 10, "LRU get must return and refresh a value");
    cache.put(3, 30);
    expect(cache.get(2) == -1, "LRU must evict the least-recent key");
    LRUCache moved_cache(std::move(cache));
    expect(moved_cache.get(1) == 10,
           "moved LRU must preserve iterator ownership");
    LRUCache assigned_cache(1);
    assigned_cache = std::move(moved_cache);
    expect(assigned_cache.get(3) == 30,
           "move-assigned LRU must preserve iterator ownership");
    LRUCache empty_cache(0);
    empty_cache.put(1, 1);
    expect(empty_cache.get(1) == -1, "zero-capacity LRU must retain nothing");

    RandomizedSet random_values(7);
    expect(random_values.insert(10), "randomized set must insert a new value");
    expect(random_values.insert(20), "randomized set must insert another value");
    expect(random_values.insert(30), "randomized set must insert a third value");
    expect(random_values.remove(20), "randomized set must remove a live value");
    expect(!random_values.remove(20), "randomized set must reject a missing value");
    const int random_value = random_values.get_random();
    expect(random_value == 10 || random_value == 30,
           "randomized set must return a live value");

    TimeMap history;
    history.set("mode", "draft", 2);
    history.set("mode", "published", 5);
    expect(history.get("mode", 1).empty(),
           "time map must reject history after the query");
    expect(history.get("mode", 4) == "draft",
           "time map must return the latest value before the query");
    expect(history.get("mode", 5) == "published",
           "time map must include an exact timestamp");

    MinStack minimums;
    minimums.push(3);
    minimums.push(1);
    minimums.push(2);
    expect(minimums.get_min() == 1, "min stack must expose the prefix minimum");
    expect(minimums.pop() == 2, "min stack must pop the latest value");
    expect(minimums.pop() == 1, "min stack must pop the previous value");
    expect(minimums.get_min() == 3,
           "min stack must restore the previous prefix minimum");

    std::cout << "All C++ algorithm tests passed.\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "C++ algorithm test failure: " << error.what() << '\n';
    return 1;
  }
}
