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

}  // namespace dsa_atlas
