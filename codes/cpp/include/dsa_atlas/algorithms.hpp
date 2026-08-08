#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <deque>
#include <functional>
#include <iterator>
#include <list>
#include <limits>
#include <queue>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dsa_atlas {

inline void validate_vertex(int vertex, std::size_t vertex_count,
                            const char* name) {
  if (vertex < 0 || static_cast<std::size_t>(vertex) >= vertex_count) {
    throw std::invalid_argument(std::string(name) + " must be a valid vertex");
  }
}

// --8<-- [start:tree-height]
struct TreeNode {
  explicit TreeNode(int node_value, TreeNode* left_child = nullptr,
                    TreeNode* right_child = nullptr)
      : value(node_value), left(left_child), right(right_child) {}

  int value;
  TreeNode* left;
  TreeNode* right;
};

inline int tree_height(const TreeNode* root) {
  // An empty subtree contributes no nodes.
  if (root == nullptr) {
    return 0;
  }
  // Solve both child contracts before combining their answers.
  const int left_height = tree_height(root->left);
  const int right_height = tree_height(root->right);
  return 1 + std::max(left_height, right_height);
}
// --8<-- [end:tree-height]

// --8<-- [start:two-sum]
inline std::vector<int> two_sum(
    const std::vector<int>& values, int target) {
  std::unordered_map<long long, int> position;
  for (int index = 0; index < static_cast<int>(values.size()); ++index) {
    const long long complement =
        static_cast<long long>(target) - static_cast<long long>(values[index]);
    const auto found = position.find(complement);
    // Search before insertion so an element cannot match itself.
    if (found != position.end()) {
      return std::vector<int>{found->second, index};
    }
    // Remember this index for complements that appear later.
    position[static_cast<long long>(values[index])] = index;
  }
  return std::vector<int>();
}
// --8<-- [end:two-sum]

// --8<-- [start:prefix-sum]
inline long long count_subarrays_sum(
    const std::vector<int>& values, long long target) {
  // The empty prefix lets ranges beginning at index zero be counted.
  std::unordered_map<long long, int> seen{{0, 1}};
  long long prefix = 0;
  long long answer = 0;
  for (const int value : values) {
    prefix += value;
    // earlier_prefix = current_prefix - target
    const auto found = seen.find(prefix - target);
    if (found != seen.end()) {
      answer += found->second;
    }
    // Store only after querying so this prefix is not treated as earlier.
    ++seen[prefix];
  }
  return answer;
}
// --8<-- [end:prefix-sum]

// --8<-- [start:sliding-window]
inline std::vector<int> max_sliding_window(
    const std::vector<int>& values, int window) {
  if (window <= 0 || window > static_cast<int>(values.size())) {
    throw std::invalid_argument("window must be between 1 and values.size()");
  }
  std::deque<int> candidates;
  std::vector<int> answer;
  for (int right = 0; right < static_cast<int>(values.size()); ++right) {
    // Expire indices that have left the current window.
    while (!candidates.empty() && candidates.front() <= right - window) {
      candidates.pop_front();
    }
    // Remove dominated values that can never become a future maximum.
    while (!candidates.empty() &&
           values[candidates.back()] <= values[right]) {
      candidates.pop_back();
    }
    candidates.push_back(right);
    if (right + 1 >= window) {
      // The front is the largest live candidate.
      answer.push_back(values[candidates.front()]);
    }
  }
  return answer;
}
// --8<-- [end:sliding-window]

// --8<-- [start:binary-search]
inline int binary_search_index(
    const std::vector<int>& values, int target) {
  // Invariant: a present target remains inside inclusive [low, high].
  int low = 0;
  int high = static_cast<int>(values.size()) - 1;
  while (low <= high) {
    const int middle = low + (high - low) / 2;
    if (values[middle] == target) {
      return middle;
    }
    if (values[middle] < target) {
      // middle and everything left of it are too small.
      low = middle + 1;
    } else {
      // middle and everything right of it are too large.
      high = middle - 1;
    }
  }
  return -1;
}
// --8<-- [end:binary-search]

// --8<-- [start:first-true]
template <class Predicate>
int first_true(int size, Predicate predicate) {
  if (size < 0) {
    throw std::invalid_argument("size must be nonnegative");
  }
  // [low, high) contains the first true position or sentinel size.
  int low = 0;
  int high = size;
  while (low < high) {
    const int middle = low + (high - low) / 2;
    if (predicate(middle)) {
      // middle may be the answer, so keep it.
      high = middle;
    } else {
      // middle is false; discard the false prefix through middle.
      low = middle + 1;
    }
  }
  return low;
}
// --8<-- [end:first-true]

// --8<-- [start:bfs]
inline std::vector<int> bfs_distances(
    const std::vector<std::vector<int>>& graph, int source) {
  validate_vertex(source, graph.size(), "source");
  for (const auto& neighbors : graph) {
    for (const int neighbor : neighbors) {
      validate_vertex(neighbor, graph.size(), "neighbor");
    }
  }
  std::vector<int> distance(graph.size(), -1);
  std::queue<int> frontier;
  // Mark on enqueue so every vertex enters the queue once.
  distance[source] = 0;
  frontier.push(source);
  while (!frontier.empty()) {
    const int node = frontier.front();
    frontier.pop();
    for (const int neighbor : graph[node]) {
      if (distance[neighbor] == -1) {
        // Queue order makes this first distance the shortest.
        distance[neighbor] = distance[node] + 1;
        frontier.push(neighbor);
      }
    }
  }
  return distance;
}
// --8<-- [end:bfs]

using WeightedGraph =
    std::vector<std::vector<std::pair<int, long long>>>;

// --8<-- [start:dijkstra]
inline std::vector<long long> dijkstra(
    const WeightedGraph& graph, int source) {
  validate_vertex(source, graph.size(), "source");
  for (const auto& edges : graph) {
    for (const auto& edge : edges) {
      const int neighbor = edge.first;
      const long long weight = edge.second;
      validate_vertex(neighbor, graph.size(), "neighbor");
      if (weight < 0) {
        throw std::invalid_argument("Dijkstra requires nonnegative weights");
      }
    }
  }
  constexpr long long infinity = std::numeric_limits<long long>::max();
  using State = std::pair<long long, int>;
  std::priority_queue<State, std::vector<State>, std::greater<State>> frontier;
  std::vector<long long> distance(graph.size(), infinity);
  distance[source] = 0;
  frontier.push({0, source});

  while (!frontier.empty()) {
    const State state = frontier.top();
    const long long current = state.first;
    const int node = state.second;
    frontier.pop();
    // Ignore heap entries superseded by a shorter route.
    if (current != distance[node]) {
      continue;
    }
    for (const auto& edge : graph[node]) {
      const int neighbor = edge.first;
      const long long weight = edge.second;
      if (current > infinity - weight) {
        continue;
      }
      const long long candidate = current + weight;
      if (candidate < distance[neighbor]) {
        // Relax the edge and schedule the improved state.
        distance[neighbor] = candidate;
        frontier.push({candidate, neighbor});
      }
    }
  }
  return distance;
}
// --8<-- [end:dijkstra]

// --8<-- [start:topological-order]
inline std::vector<int> topological_order(
    const std::vector<std::vector<int>>& graph) {
  for (const auto& neighbors : graph) {
    for (const int neighbor : neighbors) {
      validate_vertex(neighbor, graph.size(), "neighbor");
    }
  }
  std::vector<int> indegree(graph.size(), 0);
  for (const auto& neighbors : graph) {
    for (const int neighbor : neighbors) {
      ++indegree[neighbor];
    }
  }
  std::queue<int> ready;
  for (int node = 0; node < static_cast<int>(graph.size()); ++node) {
    if (indegree[node] == 0) {
      // Only vertices with no unmet prerequisite are ready.
      ready.push(node);
    }
  }
  std::vector<int> order;
  while (!ready.empty()) {
    const int node = ready.front();
    ready.pop();
    order.push_back(node);
    for (const int neighbor : graph[node]) {
      if (--indegree[neighbor] == 0) {
        // Its final prerequisite has just been removed.
        ready.push(neighbor);
      }
    }
  }
  if (order.size() != graph.size()) {
    throw std::invalid_argument("directed graph contains a cycle");
  }
  return order;
}
// --8<-- [end:topological-order]

// --8<-- [start:bridges]
inline std::vector<std::pair<int, int>> find_bridges(
    int vertex_count, const std::vector<std::pair<int, int>>& edges) {
  if (vertex_count < 0) {
    throw std::invalid_argument("vertex_count must be nonnegative");
  }
  std::vector<std::vector<std::pair<int, int>>> graph(
      static_cast<std::size_t>(vertex_count));
  for (int id = 0; id < static_cast<int>(edges.size()); ++id) {
    const int left = edges[id].first;
    const int right = edges[id].second;
    validate_vertex(left, graph.size(), "edge endpoint");
    validate_vertex(right, graph.size(), "edge endpoint");
    // Edge IDs distinguish parallel edges from the exact parent edge.
    graph[left].push_back({right, id});
    graph[right].push_back({left, id});
  }
  std::vector<int> entered(vertex_count, -1);
  std::vector<int> low(vertex_count);
  std::vector<std::pair<int, int>> bridges;
  int timer = 0;
  std::function<void(int, int)> search = [&](int node, int parent_edge) {
    entered[node] = low[node] = timer++;
    for (const auto& edge : graph[node]) {
      const int neighbor = edge.first;
      const int edge_id = edge.second;
      if (edge_id == parent_edge) {
        continue;
      }
      if (entered[neighbor] >= 0) {
        // A back edge may connect this subtree to an ancestor.
        low[node] = std::min(low[node], entered[neighbor]);
        continue;
      }
      search(neighbor, edge_id);
      low[node] = std::min(low[node], low[neighbor]);
      if (low[neighbor] > entered[node]) {
        // The child subtree has no route around this tree edge.
        bridges.push_back(std::minmax(node, neighbor));
      }
    }
  };
  for (int node = 0; node < vertex_count; ++node) {
    if (entered[node] < 0) {
      search(node, -1);
    }
  }
  std::sort(bridges.begin(), bridges.end());
  return bridges;
}
// --8<-- [end:bridges]

// --8<-- [start:subsets]
inline std::vector<std::vector<int>> unique_subsets(
    std::vector<int> values) {
  // Sorting places equal sibling choices next to one another.
  std::sort(values.begin(), values.end());
  std::vector<std::vector<int>> answer;
  std::vector<int> path;
  std::function<void(int)> search = [&](int start) {
    // Every current path is one valid subset.
    answer.push_back(path);
    for (int index = start; index < static_cast<int>(values.size()); ++index) {
      if (index > start && values[index] == values[index - 1]) {
        continue;
      }
      // Choose, recurse, then undo before trying the next sibling.
      path.push_back(values[index]);
      search(index + 1);
      path.pop_back();
    }
  };
  search(0);
  return answer;
}
// --8<-- [end:subsets]

// --8<-- [start:knapsack]
inline long long knapsack_01(
    int capacity, const std::vector<std::pair<int, int>>& items) {
  if (capacity < 0) {
    throw std::invalid_argument("capacity must be nonnegative");
  }
  std::vector<long long> best(capacity + 1, 0);
  for (const auto& item : items) {
    const int weight = item.first;
    const int value = item.second;
    if (weight <= 0) {
      throw std::invalid_argument("item weights must be positive");
    }
    // Descend so the source state still belongs to the prior item layer.
    for (int current = capacity; current >= weight; --current) {
      best[current] =
          std::max(best[current], best[current - weight] + value);
    }
  }
  return best[capacity];
}
// --8<-- [end:knapsack]

// --8<-- [start:non-adjacent]
inline long long max_non_adjacent_sum(const std::vector<int>& values) {
  long long skip_previous = 0;
  long long take_previous = 0;
  for (const int value : values) {
    // Taking this value requires skipping the immediately previous value.
    const long long take_current = skip_previous + value;
    // Skipping this value preserves the best earlier result.
    const long long skip_current = std::max(skip_previous, take_previous);
    take_previous = take_current;
    skip_previous = skip_current;
  }
  return std::max(skip_previous, take_previous);
}
// --8<-- [end:non-adjacent]

// --8<-- [start:lcs]
inline int longest_common_subsequence_length(
    const std::string& left, const std::string& right) {
  std::vector<int> previous(right.size() + 1, 0);
  for (const char left_character : left) {
    std::vector<int> current(right.size() + 1, 0);
    for (std::size_t column = 1; column <= right.size(); ++column) {
      if (left_character == right[column - 1]) {
        // Matching characters extend both shorter prefixes.
        current[column] = previous[column - 1] + 1;
      } else {
        // Otherwise discard one final character and keep the better option.
        current[column] = std::max(previous[column], current[column - 1]);
      }
    }
    previous = std::move(current);
  }
  return previous.back();
}
// --8<-- [end:lcs]

// --8<-- [start:coin-change]
inline int minimum_coins(const std::vector<int>& coins, int amount) {
  if (amount < 0) {
    throw std::invalid_argument("amount must be nonnegative");
  }
  for (const int coin : coins) {
    if (coin <= 0) {
      throw std::invalid_argument("coin values must be positive");
    }
  }

  const int impossible = amount + 1;
  std::vector<int> best(amount + 1, impossible);
  best[0] = 0;  // Zero coins make amount zero.
  for (int current = 1; current <= amount; ++current) {
    for (const int coin : coins) {
      if (coin <= current) {
        // Append this reusable coin to a solved smaller amount.
        best[current] = std::min(best[current], best[current - coin] + 1);
      }
    }
  }
  return best[amount] == impossible ? -1 : best[amount];
}
// --8<-- [end:coin-change]

// --8<-- [start:cherry-pickup]
inline long long cherry_pickup_two_robots(
    const std::vector<std::vector<int>>& grid) {
  if (grid.empty() || grid.front().empty()) {
    return 0;
  }
  const int rows = static_cast<int>(grid.size());
  const int columns = static_cast<int>(grid.front().size());
  for (const auto& row : grid) {
    if (static_cast<int>(row.size()) != columns) {
      throw std::invalid_argument("grid must be rectangular");
    }
  }

  constexpr long long negative_infinity =
      std::numeric_limits<long long>::lowest() / 4;
  std::vector<std::vector<std::vector<long long>>> memo(
      rows, std::vector<std::vector<long long>>(
                columns, std::vector<long long>(columns, negative_infinity)));
  std::vector<std::vector<std::vector<bool>>> seen(
      rows, std::vector<std::vector<bool>>(
                columns, std::vector<bool>(columns, false)));

  std::function<long long(int, int, int)> solve =
      [&](int row, int first_column, int second_column) -> long long {
    if (first_column < 0 || first_column >= columns ||
        second_column < 0 || second_column >= columns) {
      return negative_infinity;  // Invalid moves must never win a maximum.
    }
    if (seen[row][first_column][second_column]) {
      return memo[row][first_column][second_column];
    }
    seen[row][first_column][second_column] = true;

    long long cherries = grid[row][first_column];
    if (first_column != second_column) {
      cherries += grid[row][second_column];  // Count a shared cell only once.
    }
    if (row == rows - 1) {
      return memo[row][first_column][second_column] = cherries;
    }

    long long best_suffix = negative_infinity;
    for (int first_step = -1; first_step <= 1; ++first_step) {
      for (int second_step = -1; second_step <= 1; ++second_step) {
        // Both robot positions are required to describe the next state.
        best_suffix =
            std::max(best_suffix,
                     solve(row + 1, first_column + first_step,
                           second_column + second_step));
      }
    }
    return memo[row][first_column][second_column] = cherries + best_suffix;
  };

  return solve(0, 0, columns - 1);
}
// --8<-- [end:cherry-pickup]

// --8<-- [start:digit-dp]
inline long long count_distinct_digit_numbers(int limit) {
  if (limit < 0) {
    throw std::invalid_argument("limit must be nonnegative");
  }
  const std::string digits = std::to_string(limit);
  const int positions = static_cast<int>(digits.size());
  std::vector<std::vector<std::vector<long long>>> memo(
      positions,
      std::vector<std::vector<long long>>(
          2, std::vector<long long>(1 << 10, -1)));

  std::function<long long(int, bool, bool, int)> solve =
      [&](int position, bool tight, bool started,
          int used_mask) -> long long {
    if (position == positions) {
      return started ? 1 : 0;  // Exclude the all-leading-zero representation.
    }
    if (!tight && memo[position][started][used_mask] != -1) {
      return memo[position][started][used_mask];
    }

    const int upper = tight ? digits[position] - '0' : 9;
    long long total = 0;
    for (int digit = 0; digit <= upper; ++digit) {
      const bool next_tight = tight && digit == upper;
      if (!started && digit == 0) {
        // Leading zeros do not consume digit zero.
        total += solve(position + 1, next_tight, false, used_mask);
      } else if ((used_mask & (1 << digit)) == 0) {
        // The bitmask remembers every digit chosen for the real number.
        total += solve(position + 1, next_tight, true,
                       used_mask | (1 << digit));
      }
    }
    if (!tight) {
      memo[position][started][used_mask] = total;
    }
    return total;
  };

  return solve(0, true, false, 0);
}
// --8<-- [end:digit-dp]

// --8<-- [start:disjoint-set]
class DisjointSet {
 public:
  explicit DisjointSet(int size)
      : parent_(checked_size(size)), component_size_(checked_size(size), 1) {
    for (int index = 0; index < size; ++index) {
      parent_[index] = index;
    }
  }

  int find(int item) {
    if (item < 0 || static_cast<std::size_t>(item) >= parent_.size()) {
      throw std::invalid_argument("item must be a valid set element");
    }
    while (parent_[item] != item) {
      // Point to the grandparent to shorten future find paths.
      parent_[item] = parent_[parent_[item]];
      item = parent_[item];
    }
    return item;
  }

  bool unite(int left, int right) {
    left = find(left);
    right = find(right);
    if (left == right) {
      return false;
    }
    // Attach the smaller tree below the larger tree.
    if (component_size_[left] < component_size_[right]) {
      std::swap(left, right);
    }
    parent_[right] = left;
    component_size_[left] += component_size_[right];
    return true;
  }

 private:
  static std::size_t checked_size(int size) {
    if (size < 0) {
      throw std::invalid_argument("size must be nonnegative");
    }
    return static_cast<std::size_t>(size);
  }

  std::vector<int> parent_;
  std::vector<int> component_size_;
};
// --8<-- [end:disjoint-set]

// --8<-- [start:lru-cache]
class LRUCache {
 public:
  explicit LRUCache(int capacity) : capacity_(capacity) {
    if (capacity < 0) {
      throw std::invalid_argument("capacity must be nonnegative");
    }
  }

  // The map stores iterators into this instance's list, so shallow copying
  // would leave the copy pointing into the original cache.
  LRUCache(const LRUCache&) = delete;
  LRUCache& operator=(const LRUCache&) = delete;
  LRUCache(LRUCache&&) noexcept = default;
  LRUCache& operator=(LRUCache&&) noexcept = default;

  int get(int key) {
    const auto found = nodes_.find(key);
    if (found == nodes_.end()) {
      return -1;
    }
    // A successful read makes this entry most recent.
    touch(found->second);
    return found->second->second;
  }

  void put(int key, int value) {
    const auto found = nodes_.find(key);
    if (found != nodes_.end()) {
      found->second->second = value;
      touch(found->second);
      return;
    }

    // The list front is most recent; the back is the eviction target.
    recency_.push_front({key, value});
    nodes_[key] = recency_.begin();
    if (nodes_.size() > static_cast<std::size_t>(capacity_)) {
      const int oldest_key = recency_.back().first;
      recency_.pop_back();
      nodes_.erase(oldest_key);
    }
  }

 private:
  using Entry = std::pair<int, int>;
  using Iterator = std::list<Entry>::iterator;

  void touch(Iterator entry) {
    // splice moves the existing node without invalidating its iterator.
    recency_.splice(recency_.begin(), recency_, entry);
  }

  int capacity_;
  std::list<Entry> recency_;
  std::unordered_map<int, Iterator> nodes_;
};
// --8<-- [end:lru-cache]

// --8<-- [start:randomized-set]
class RandomizedSet {
 public:
  explicit RandomizedSet(unsigned seed = std::random_device{}())
      : random_(seed) {}

  bool insert(int value) {
    if (position_.count(value) != 0) {
      return false;
    }
    position_[value] = values_.size();
    values_.push_back(value);
    return true;
  }

  bool remove(int value) {
    const auto found = position_.find(value);
    if (found == position_.end()) {
      return false;
    }
    const std::size_t index = found->second;
    const int last = values_.back();
    // Fill the removed slot with the last value before shrinking.
    values_[index] = last;
    position_[last] = index;
    values_.pop_back();
    position_.erase(found);
    return true;
  }

  int get_random() {
    if (values_.empty()) {
      throw std::logic_error("cannot choose from an empty set");
    }
    // Dense array indices make every live value equally reachable.
    std::uniform_int_distribution<std::size_t> choose(
        0, values_.size() - 1);
    return values_[choose(random_)];
  }

 private:
  std::vector<int> values_;
  std::unordered_map<int, std::size_t> position_;
  std::mt19937 random_;
};
// --8<-- [end:randomized-set]

// --8<-- [start:kruskal]
inline long long minimum_spanning_tree_weight(
    int vertex_count,
    const std::vector<std::tuple<int, int, long long>>& edges) {
  if (vertex_count < 0) {
    throw std::invalid_argument("vertex_count must be nonnegative");
  }
  for (const auto& edge : edges) {
    const int left = std::get<0>(edge);
    const int right = std::get<1>(edge);
    const long long weight = std::get<2>(edge);
    static_cast<void>(weight);
    if (left < 0 || right < 0 || left >= vertex_count ||
        right >= vertex_count) {
      throw std::invalid_argument("edge endpoint must be a valid vertex");
    }
  }

  auto ordered = edges;
  std::sort(ordered.begin(), ordered.end(),
            [](const std::tuple<int, int, long long>& left,
               const std::tuple<int, int, long long>& right) {
              return std::get<2>(left) < std::get<2>(right);
            });

  DisjointSet groups(vertex_count);
  long long total = 0;
  int accepted = 0;
  // The cheapest safe edge is justified by the MST cut property.
  for (const auto& edge : ordered) {
    const int left = std::get<0>(edge);
    const int right = std::get<1>(edge);
    const long long weight = std::get<2>(edge);
    if (groups.unite(left, right)) {
      if ((weight > 0 &&
           total > std::numeric_limits<long long>::max() - weight) ||
          (weight < 0 &&
           total < std::numeric_limits<long long>::min() - weight)) {
        throw std::overflow_error("minimum spanning-tree weight overflow");
      }
      total += weight;
      ++accepted;
    }
  }

  const int required = std::max(0, vertex_count - 1);
  if (accepted != required) {
    throw std::invalid_argument("undirected graph is disconnected");
  }
  return total;
}
// --8<-- [end:kruskal]

// --8<-- [start:median]
class MedianFinder {
 public:
  void add(int value) {
    // Route through lower so every lower value stays <= every upper value.
    lower_.push(value);
    upper_.push(lower_.top());
    lower_.pop();
    if (upper_.size() > lower_.size()) {
      // Keep lower the same size as upper or one element larger.
      lower_.push(upper_.top());
      upper_.pop();
    }
  }

  double median() const {
    if (lower_.empty()) {
      throw std::logic_error("median is undefined for an empty stream");
    }
    if (lower_.size() > upper_.size()) {
      return static_cast<double>(lower_.top());
    }
    return (static_cast<double>(lower_.top()) +
            static_cast<double>(upper_.top())) /
           2.0;
  }

 private:
  std::priority_queue<int> lower_;
  std::priority_queue<int, std::vector<int>, std::greater<int>> upper_;
};
// --8<-- [end:median]

// --8<-- [start:time-map]
class TimeMap {
 public:
  void set(const std::string& key, std::string value, int timestamp) {
    auto& entries = history_[key];
    if (!entries.empty() && timestamp <= entries.back().first) {
      throw std::invalid_argument(
          "timestamps for one key must strictly increase");
    }
    // Increasing timestamps keep the history sorted without extra work.
    entries.push_back({timestamp, std::move(value)});
  }

  std::string get(const std::string& key, int timestamp) const {
    const auto found = history_.find(key);
    if (found == history_.end()) {
      return "";
    }
    const auto& entries = found->second;
    // upper_bound(timestamp) - 1 is the newest value not after the query.
    const auto after = std::upper_bound(
        entries.begin(), entries.end(), timestamp,
        [](int query, const std::pair<int, std::string>& entry) {
          return query < entry.first;
        });
    return after == entries.begin() ? "" : std::prev(after)->second;
  }

 private:
  std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>
      history_;
};
// --8<-- [end:time-map]

// --8<-- [start:min-stack]
class MinStack {
 public:
  void push(int value) {
    const int current_minimum =
        entries_.empty() ? value : std::min(value, entries_.back().second);
    // The saved prefix minimum restores automatically after a pop.
    entries_.push_back({value, current_minimum});
  }

  int pop() {
    require_entry("pop from empty MinStack");
    const int value = entries_.back().first;
    entries_.pop_back();
    return value;
  }

  int top() const {
    require_entry("top from empty MinStack");
    return entries_.back().first;
  }

  int get_min() const {
    require_entry("minimum of empty MinStack");
    return entries_.back().second;
  }

 private:
  void require_entry(const char* message) const {
    if (entries_.empty()) {
      throw std::logic_error(message);
    }
  }

  std::vector<std::pair<int, int>> entries_;
};
// --8<-- [end:min-stack]

// --8<-- [start:reverse-linked-list]
struct ListNode {
  explicit ListNode(int node_value, ListNode* next_node = nullptr)
      : value(node_value), next(next_node) {}

  int value;
  ListNode* next;
};

inline ListNode* reverse_linked_list(ListNode* head) {
  ListNode* previous = nullptr;
  ListNode* current = head;
  while (current != nullptr) {
    // Save the unread suffix before changing the only forward link.
    ListNode* following = current->next;
    current->next = previous;
    // Advance both boundaries of the reversed prefix.
    previous = current;
    current = following;
  }
  return previous;
}
// --8<-- [end:reverse-linked-list]

// --8<-- [start:interval-scheduling]
inline int max_non_overlapping_intervals(
    std::vector<std::pair<int, int>> intervals) {
  for (const auto& interval : intervals) {
    if (interval.first > interval.second) {
      throw std::invalid_argument("interval start must not exceed end");
    }
  }
  std::sort(intervals.begin(), intervals.end(),
            [](const std::pair<int, int>& left,
               const std::pair<int, int>& right) {
              return left.second < right.second;
            });
  int selected = 0;
  int previous_end = 0;
  bool has_previous = false;
  for (const auto& interval : intervals) {
    if (!has_previous || interval.first >= previous_end) {
      // The earliest finishing compatible interval leaves most room.
      ++selected;
      previous_end = interval.second;
      has_previous = true;
    }
  }
  return selected;
}
// --8<-- [end:interval-scheduling]

// --8<-- [start:count-set-bits]
inline int count_set_bits(long long value) {
  if (value < 0) {
    throw std::invalid_argument("value must be nonnegative");
  }
  unsigned long long remaining = static_cast<unsigned long long>(value);
  int count = 0;
  while (remaining != 0) {
    // Clearing the lowest set bit makes one unit of progress.
    remaining &= remaining - 1;
    ++count;
  }
  return count;
}
// --8<-- [end:count-set-bits]

// --8<-- [start:kmp-search]
inline int kmp_search(const std::string& text, const std::string& pattern) {
  if (pattern.empty()) {
    return 0;
  }
  std::vector<int> longest_border(pattern.size(), 0);
  int border = 0;
  for (int index = 1; index < static_cast<int>(pattern.size()); ++index) {
    // Reuse the next-shorter border after a mismatch.
    while (border > 0 && pattern[index] != pattern[border]) {
      border = longest_border[border - 1];
    }
    if (pattern[index] == pattern[border]) {
      ++border;
    }
    longest_border[index] = border;
  }

  int matched = 0;
  for (int index = 0; index < static_cast<int>(text.size()); ++index) {
    while (matched > 0 && text[index] != pattern[matched]) {
      matched = longest_border[matched - 1];
    }
    if (text[index] == pattern[matched]) {
      ++matched;
      if (matched == static_cast<int>(pattern.size())) {
        // Convert the ending index into the match's start.
        return index - matched + 1;
      }
    }
  }
  return -1;
}
// --8<-- [end:kmp-search]

// --8<-- [start:trie]
class Trie {
 public:
  Trie() : nodes_(1) {}

  void insert(const std::string& word) {
    int node = 0;
    for (const char character : word) {
      const auto found = nodes_[node].children.find(character);
      if (found == nodes_[node].children.end()) {
        // Create only the missing suffix; existing prefixes are shared.
        const int child = static_cast<int>(nodes_.size());
        nodes_[node].children[character] = child;
        nodes_.push_back(Node());
        node = child;
      } else {
        node = found->second;
      }
    }
    nodes_[node].terminal = true;
  }

  bool contains(const std::string& word) const {
    const int node = walk(word);
    return node >= 0 && nodes_[node].terminal;
  }

  bool starts_with(const std::string& prefix) const {
    return walk(prefix) >= 0;
  }

 private:
  struct Node {
    std::unordered_map<char, int> children;
    bool terminal = false;
  };

  int walk(const std::string& text) const {
    int node = 0;
    for (const char character : text) {
      const auto found = nodes_[node].children.find(character);
      if (found == nodes_[node].children.end()) {
        return -1;
      }
      node = found->second;
    }
    return node;
  }

  std::vector<Node> nodes_;
};
// --8<-- [end:trie]

// --8<-- [start:fenwick-tree]
class FenwickTree {
 public:
  explicit FenwickTree(int size) : size_(size) {
    if (size < 0) {
      throw std::invalid_argument("size must be nonnegative");
    }
    tree_.assign(static_cast<std::size_t>(size) + 1, 0);
  }

  void add(int index, long long delta) {
    require_index(index);
    for (int cursor = index + 1; cursor <= size_;
         cursor += cursor & -cursor) {
      // Each visited node owns a larger range containing index.
      tree_[cursor] += delta;
    }
  }

  long long prefix_sum(int end) const {
    if (end < 0 || end > size_) {
      throw std::out_of_range("prefix end outside Fenwick tree");
    }
    long long total = 0;
    for (int cursor = end; cursor > 0; cursor -= cursor & -cursor) {
      // Removing the lowest set bit visits the parent prefix.
      total += tree_[cursor];
    }
    return total;
  }

  long long range_sum(int left, int right) const {
    if (left < 0 || left > right || right > size_) {
      throw std::out_of_range("range outside Fenwick tree");
    }
    return prefix_sum(right) - prefix_sum(left);
  }

 private:
  void require_index(int index) const {
    if (index < 0 || index >= size_) {
      throw std::out_of_range("index outside Fenwick tree");
    }
  }

  int size_;
  std::vector<long long> tree_;
};
// --8<-- [end:fenwick-tree]

// --8<-- [start:segment-tree]
class SegmentTree {
 public:
  explicit SegmentTree(const std::vector<int>& values)
      : size_(static_cast<int>(values.size())), tree_(2 * values.size(), 0) {
    std::copy(values.begin(), values.end(), tree_.begin() + size_);
    for (int node = size_ - 1; node > 0; --node) {
      // Each internal node owns the sum of its two children.
      tree_[node] = tree_[2 * node] + tree_[2 * node + 1];
    }
  }

  void set(int index, long long value) {
    if (index < 0 || index >= size_) {
      throw std::out_of_range("index outside segment tree");
    }
    int node = index + size_;
    tree_[node] = value;
    while (node > 1) {
      node /= 2;
      tree_[node] = tree_[2 * node] + tree_[2 * node + 1];
    }
  }

  long long range_sum(int left, int right) const {
    if (left < 0 || left > right || right > size_) {
      throw std::out_of_range("range outside segment tree");
    }
    left += size_;
    right += size_;
    long long total = 0;
    while (left < right) {
      if (left % 2 == 1) {
        total += tree_[left++];
      }
      if (right % 2 == 1) {
        total += tree_[--right];
      }
      // Move from the selected boundary nodes to their parents.
      left /= 2;
      right /= 2;
    }
    return total;
  }

 private:
  int size_;
  std::vector<long long> tree_;
};
// --8<-- [end:segment-tree]

// --8<-- [start:bellman-ford]
inline std::vector<long long> bellman_ford(
    int vertex_count,
    const std::vector<std::tuple<int, int, long long>>& edges, int source) {
  if (vertex_count <= 0) {
    throw std::invalid_argument("vertex_count must be positive");
  }
  validate_vertex(source, static_cast<std::size_t>(vertex_count), "source");
  for (const auto& edge : edges) {
    validate_vertex(std::get<0>(edge), vertex_count, "edge endpoint");
    validate_vertex(std::get<1>(edge), vertex_count, "edge endpoint");
  }
  const long long infinity = std::numeric_limits<long long>::max();
  std::vector<long long> distance(vertex_count, infinity);
  distance[source] = 0;
  for (int pass = 0; pass < vertex_count - 1; ++pass) {
    bool changed = false;
    for (const auto& edge : edges) {
      const int left = std::get<0>(edge);
      const int right = std::get<1>(edge);
      const long long weight = std::get<2>(edge);
      if (distance[left] == infinity) {
        continue;
      }
      if ((weight > 0 && distance[left] > infinity - weight) ||
          (weight < 0 &&
           distance[left] < std::numeric_limits<long long>::min() - weight)) {
        throw std::overflow_error("Bellman-Ford distance overflow");
      }
      const long long candidate = distance[left] + weight;
      if (candidate < distance[right]) {
        // Relax only from a vertex the source can reach.
        distance[right] = candidate;
        changed = true;
      }
    }
    if (!changed) {
      break;
    }
  }
  for (const auto& edge : edges) {
    const int left = std::get<0>(edge);
    const int right = std::get<1>(edge);
    const long long weight = std::get<2>(edge);
    if (distance[left] == infinity) {
      continue;
    }
    if ((weight > 0 && distance[left] > infinity - weight) ||
        (weight < 0 &&
         distance[left] < std::numeric_limits<long long>::min() - weight)) {
      throw std::overflow_error("Bellman-Ford distance overflow");
    }
    if (distance[left] + weight < distance[right]) {
      throw std::invalid_argument("reachable negative cycle");
    }
  }
  return distance;
}
// --8<-- [end:bellman-ford]

// --8<-- [start:floyd-warshall]
inline std::vector<std::vector<long long>> floyd_warshall(
    std::vector<std::vector<long long>> distance) {
  const int size = static_cast<int>(distance.size());
  const long long infinity = std::numeric_limits<long long>::max();
  for (const auto& row : distance) {
    if (static_cast<int>(row.size()) != size) {
      throw std::invalid_argument("distance matrix must be square");
    }
  }
  for (int vertex = 0; vertex < size; ++vertex) {
    distance[vertex][vertex] = std::min(0LL, distance[vertex][vertex]);
  }
  for (int middle = 0; middle < size; ++middle) {
    for (int left = 0; left < size; ++left) {
      if (distance[left][middle] == infinity) {
        continue;
      }
      for (int right = 0; right < size; ++right) {
        if (distance[middle][right] == infinity) {
          continue;
        }
        const long long suffix = distance[middle][right];
        if ((suffix > 0 && distance[left][middle] > infinity - suffix) ||
            (suffix < 0 && distance[left][middle] <
                               std::numeric_limits<long long>::min() - suffix)) {
          throw std::overflow_error("Floyd-Warshall distance overflow");
        }
        const long long candidate =
            distance[left][middle] + distance[middle][right];
        if (candidate < distance[left][right]) {
          // Phase middle permits vertices 0..middle as internal vertices.
          distance[left][right] = candidate;
        }
      }
    }
  }
  for (int vertex = 0; vertex < size; ++vertex) {
    if (distance[vertex][vertex] < 0) {
      throw std::invalid_argument("graph contains a negative cycle");
    }
  }
  return distance;
}
// --8<-- [end:floyd-warshall]

// --8<-- [start:scc]
inline std::vector<std::vector<int>> strongly_connected_components(
    const std::vector<std::vector<int>>& graph) {
  for (const auto& neighbors : graph) {
    for (const int neighbor : neighbors) {
      validate_vertex(neighbor, graph.size(), "neighbor");
    }
  }
  std::vector<bool> visited(graph.size(), false);
  std::vector<int> finish_order;
  for (int start = 0; start < static_cast<int>(graph.size()); ++start) {
    if (visited[start]) {
      continue;
    }
    visited[start] = true;
    std::vector<std::pair<int, std::size_t>> stack(1, {start, 0});
    while (!stack.empty()) {
      const int node = stack.back().first;
      std::size_t& edge_index = stack.back().second;
      if (edge_index < graph[node].size()) {
        const int neighbor = graph[node][edge_index++];
        if (!visited[neighbor]) {
          visited[neighbor] = true;
          stack.push_back({neighbor, 0});
        }
      } else {
        // Postorder records a vertex only after its suffix is complete.
        finish_order.push_back(node);
        stack.pop_back();
      }
    }
  }

  std::vector<std::vector<int>> reversed(graph.size());
  for (int node = 0; node < static_cast<int>(graph.size()); ++node) {
    for (const int neighbor : graph[node]) {
      reversed[neighbor].push_back(node);
    }
  }
  std::vector<int> component_of(graph.size(), -1);
  std::vector<std::vector<int>> components;
  for (auto position = finish_order.rbegin(); position != finish_order.rend();
       ++position) {
    const int start = *position;
    if (component_of[start] != -1) {
      continue;
    }
    const int component_id = static_cast<int>(components.size());
    components.push_back(std::vector<int>());
    std::vector<int> stack(1, start);
    component_of[start] = component_id;
    while (!stack.empty()) {
      const int node = stack.back();
      stack.pop_back();
      components.back().push_back(node);
      for (const int neighbor : reversed[node]) {
        if (component_of[neighbor] == -1) {
          component_of[neighbor] = component_id;
          stack.push_back(neighbor);
        }
      }
    }
    std::sort(components.back().begin(), components.back().end());
  }
  return components;
}
// --8<-- [end:scc]

// --8<-- [start:binary-lifting]
class BinaryLiftingTree {
 public:
  explicit BinaryLiftingTree(const std::vector<std::vector<int>>& tree,
                             int root = 0)
      : depth_(tree.size(), 0) {
    validate_vertex(root, tree.size(), "root");
    for (const auto& neighbors : tree) {
      for (const int neighbor : neighbors) {
        validate_vertex(neighbor, tree.size(), "neighbor");
      }
    }
    std::vector<std::set<int>> neighbor_sets(tree.size());
    std::size_t directed_edge_count = 0;
    for (int node = 0; node < static_cast<int>(tree.size()); ++node) {
      neighbor_sets[node].insert(tree[node].begin(), tree[node].end());
      if (neighbor_sets[node].size() != tree[node].size()) {
        throw std::invalid_argument("input tree cannot contain parallel edges");
      }
      if (neighbor_sets[node].count(node) != 0) {
        throw std::invalid_argument("input tree cannot contain self-loops");
      }
      directed_edge_count += tree[node].size();
    }
    if (directed_edge_count != 2 * (tree.size() - 1)) {
      throw std::invalid_argument(
          "input must contain exactly n - 1 undirected edges");
    }
    for (int node = 0; node < static_cast<int>(tree.size()); ++node) {
      for (const int neighbor : neighbor_sets[node]) {
        if (neighbor_sets[neighbor].count(node) == 0) {
          throw std::invalid_argument(
              "every tree edge must appear in both directions");
        }
      }
    }
    std::vector<int> parent(tree.size(), -1);
    parent[root] = root;
    std::vector<int> stack(1, root);
    while (!stack.empty()) {
      const int node = stack.back();
      stack.pop_back();
      for (const int neighbor : tree[node]) {
        if (neighbor == parent[node]) {
          continue;
        }
        if (parent[neighbor] != -1) {
          throw std::invalid_argument("input must be an undirected tree");
        }
        parent[neighbor] = node;
        depth_[neighbor] = depth_[node] + 1;
        stack.push_back(neighbor);
      }
    }
    if (std::find(parent.begin(), parent.end(), -1) != parent.end()) {
      throw std::invalid_argument("input tree must be connected");
    }

    int levels = 1;
    while ((1LL << levels) <= static_cast<long long>(tree.size())) {
      ++levels;
    }
    up_.push_back(parent);
    for (int level = 1; level < levels; ++level) {
      up_.push_back(std::vector<int>(tree.size()));
      for (int node = 0; node < static_cast<int>(tree.size()); ++node) {
        // Doubling composes two jumps of length 2^(level - 1).
        up_[level][node] = up_[level - 1][up_[level - 1][node]];
      }
    }
  }

  int lca(int left, int right) const {
    validate_vertex(left, depth_.size(), "left");
    validate_vertex(right, depth_.size(), "right");
    if (depth_[left] < depth_[right]) {
      std::swap(left, right);
    }
    int difference = depth_[left] - depth_[right];
    for (int level = 0; level < static_cast<int>(up_.size()); ++level) {
      if ((difference & (1 << level)) != 0) {
        left = up_[level][left];
      }
    }
    if (left == right) {
      return left;
    }
    for (int level = static_cast<int>(up_.size()) - 1; level >= 0; --level) {
      if (up_[level][left] != up_[level][right]) {
        // Lift together while their 2^level ancestors still differ.
        left = up_[level][left];
        right = up_[level][right];
      }
    }
    return up_[0][left];
  }

 private:
  std::vector<int> depth_;
  std::vector<std::vector<int>> up_;
};
// --8<-- [end:binary-lifting]

// --8<-- [start:dinic]
class Dinic {
 public:
  explicit Dinic(int vertex_count) {
    if (vertex_count < 0) {
      throw std::invalid_argument("vertex_count must be nonnegative");
    }
    graph_.resize(static_cast<std::size_t>(vertex_count));
  }

  void add_edge(int left, int right, long long capacity) {
    validate_vertex(left, graph_.size(), "edge endpoint");
    validate_vertex(right, graph_.size(), "edge endpoint");
    if (capacity < 0) {
      throw std::invalid_argument("capacity must be nonnegative");
    }
    const int left_reverse = static_cast<int>(graph_[right].size());
    const int right_reverse = static_cast<int>(graph_[left].size());
    graph_[left].push_back(Edge(right, left_reverse, capacity));
    graph_[right].push_back(Edge(left, right_reverse, 0));
  }

  long long max_flow(int source, int sink) {
    validate_vertex(source, graph_.size(), "source");
    validate_vertex(sink, graph_.size(), "sink");
    if (source == sink) {
      return 0;
    }
    long long total = 0;
    while (build_levels(source, sink)) {
      next_edge_.assign(graph_.size(), 0);
      while (true) {
        const long long sent =
            push(source, sink, std::numeric_limits<long long>::max());
        if (sent == 0) {
          break;
        }
        if (total > std::numeric_limits<long long>::max() - sent) {
          throw std::overflow_error("maximum flow overflow");
        }
        total += sent;
      }
    }
    return total;
  }

 private:
  struct Edge {
    Edge(int edge_to, int reverse_index, long long residual_capacity)
        : to(edge_to), reverse(reverse_index), capacity(residual_capacity) {}
    int to;
    int reverse;
    long long capacity;
  };

  bool build_levels(int source, int sink) {
    level_.assign(graph_.size(), -1);
    level_[source] = 0;
    std::queue<int> queue;
    queue.push(source);
    while (!queue.empty()) {
      const int node = queue.front();
      queue.pop();
      for (const auto& edge : graph_[node]) {
        if (edge.capacity > 0 && level_[edge.to] == -1) {
          level_[edge.to] = level_[node] + 1;
          queue.push(edge.to);
        }
      }
    }
    return level_[sink] != -1;
  }

  long long push(int node, int sink, long long available) {
    if (node == sink) {
      return available;
    }
    for (int& index = next_edge_[node];
         index < static_cast<int>(graph_[node].size()); ++index) {
      Edge& edge = graph_[node][index];
      if (edge.capacity == 0 || level_[edge.to] != level_[node] + 1) {
        continue;
      }
      const long long sent = push(edge.to, sink, std::min(available, edge.capacity));
      if (sent > 0) {
        // Forward and reverse residual capacities change together.
        edge.capacity -= sent;
        graph_[edge.to][edge.reverse].capacity += sent;
        return sent;
      }
    }
    return 0;
  }

  std::vector<std::vector<Edge>> graph_;
  std::vector<int> level_;
  std::vector<int> next_edge_;
};
// --8<-- [end:dinic]

// --8<-- [start:a-star-grid]
inline int a_star_grid(const std::vector<std::vector<int>>& grid,
                       std::pair<int, int> start,
                       std::pair<int, int> goal) {
  if (grid.empty() || grid.front().empty()) {
    throw std::invalid_argument("grid must be nonempty and rectangular");
  }
  const int rows = static_cast<int>(grid.size());
  const int columns = static_cast<int>(grid.front().size());
  for (const auto& row : grid) {
    if (static_cast<int>(row.size()) != columns) {
      throw std::invalid_argument("grid must be nonempty and rectangular");
    }
  }
  const auto valid_cell = [&](const std::pair<int, int>& cell) {
    return cell.first >= 0 && cell.first < rows && cell.second >= 0 &&
           cell.second < columns;
  };
  if (!valid_cell(start) || !valid_cell(goal)) {
    throw std::invalid_argument("endpoint outside grid");
  }
  if (grid[start.first][start.second] != 0 ||
      grid[goal.first][goal.second] != 0) {
    return -1;
  }

  typedef std::tuple<int, int, int, int> State;
  std::priority_queue<State, std::vector<State>, std::greater<State>> frontier;
  std::vector<std::vector<int>> best(
      rows, std::vector<int>(columns, std::numeric_limits<int>::max()));
  best[start.first][start.second] = 0;
  frontier.push(State(std::abs(start.first - goal.first) +
                          std::abs(start.second - goal.second),
                      0, start.first, start.second));
  const int moves[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  while (!frontier.empty()) {
    const State state = frontier.top();
    frontier.pop();
    const int distance = std::get<1>(state);
    const int row = std::get<2>(state);
    const int column = std::get<3>(state);
    if (distance != best[row][column]) {
      continue;
    }
    if (row == goal.first && column == goal.second) {
      return distance;
    }
    for (int move = 0; move < 4; ++move) {
      const int next_row = row + moves[move][0];
      const int next_column = column + moves[move][1];
      if (next_row < 0 || next_row >= rows || next_column < 0 ||
          next_column >= columns || grid[next_row][next_column] != 0) {
        continue;
      }
      const int candidate = distance + 1;
      if (candidate < best[next_row][next_column]) {
        best[next_row][next_column] = candidate;
        // Manhattan distance is admissible for four-direction unit moves.
        const int heuristic = std::abs(next_row - goal.first) +
                              std::abs(next_column - goal.second);
        frontier.push(
            State(candidate + heuristic, candidate, next_row, next_column));
      }
    }
  }
  return -1;
}
// --8<-- [end:a-star-grid]

// --8<-- [start:number-containers]
class NumberContainers {
 public:
  void change(int index, int value) {
    if (index < 0) {
      throw std::invalid_argument("index must be nonnegative");
    }
    const auto existing = index_value_.find(index);
    if (existing != index_value_.end()) {
      value_indices_[existing->second].erase(index);
    }
    index_value_[index] = value;
    value_indices_[value].insert(index);
  }

  int find(int value) const {
    const auto found = value_indices_.find(value);
    return found == value_indices_.end() || found->second.empty()
               ? -1
               : *found->second.begin();
  }

 private:
  std::unordered_map<int, int> index_value_;
  std::unordered_map<int, std::set<int>> value_indices_;
};
// --8<-- [end:number-containers]

// --8<-- [start:snapshot-array]
class SnapshotArray {
 public:
  explicit SnapshotArray(int length) {
    if (length < 0) {
      throw std::invalid_argument("length must be nonnegative");
    }
    history_.assign(static_cast<std::size_t>(length),
                    std::vector<std::pair<int, int>>(1, {0, 0}));
  }

  void set(int index, int value) {
    require_index(index);
    auto& entries = history_[index];
    if (entries.back().first == current_snapshot_) {
      // Coalesce repeated writes in the current in-progress version.
      entries.back().second = value;
    } else {
      entries.push_back({current_snapshot_, value});
    }
  }

  int snap() { return current_snapshot_++; }

  int get(int index, int snapshot) const {
    require_index(index);
    if (snapshot < 0 || snapshot >= current_snapshot_) {
      throw std::invalid_argument("snapshot has not been created");
    }
    const auto& entries = history_[index];
    // upper_bound(snapshot) - 1 is the newest visible change.
    const auto after = std::upper_bound(
        entries.begin(), entries.end(), snapshot,
        [](int query, const std::pair<int, int>& entry) {
          return query < entry.first;
        });
    return std::prev(after)->second;
  }

 private:
  void require_index(int index) const {
    if (index < 0 || index >= static_cast<int>(history_.size())) {
      throw std::out_of_range("index outside snapshot array");
    }
  }

  int current_snapshot_ = 0;
  std::vector<std::vector<std::pair<int, int>>> history_;
};
// --8<-- [end:snapshot-array]

}  // namespace dsa_atlas
