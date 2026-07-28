#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <deque>
#include <functional>
#include <limits>
#include <optional>
#include <queue>
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
  int value;
  TreeNode* left = nullptr;
  TreeNode* right = nullptr;
};

inline int tree_height(const TreeNode* root) {
  if (root == nullptr) {
    return 0;
  }
  const int left_height = tree_height(root->left);
  const int right_height = tree_height(root->right);
  return 1 + std::max(left_height, right_height);
}
// --8<-- [end:tree-height]

// --8<-- [start:two-sum]
inline std::optional<std::pair<int, int>> two_sum(
    const std::vector<int>& values, int target) {
  std::unordered_map<long long, int> position;
  for (int index = 0; index < static_cast<int>(values.size()); ++index) {
    const long long complement =
        static_cast<long long>(target) - static_cast<long long>(values[index]);
    const auto found = position.find(complement);
    if (found != position.end()) {
      return std::pair<int, int>{found->second, index};
    }
    position[static_cast<long long>(values[index])] = index;
  }
  return std::nullopt;
}
// --8<-- [end:two-sum]

// --8<-- [start:prefix-sum]
inline long long count_subarrays_sum(
    const std::vector<int>& values, long long target) {
  std::unordered_map<long long, int> seen{{0, 1}};
  long long prefix = 0;
  long long answer = 0;
  for (const int value : values) {
    prefix += value;
    const auto found = seen.find(prefix - target);
    if (found != seen.end()) {
      answer += found->second;
    }
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
    while (!candidates.empty() && candidates.front() <= right - window) {
      candidates.pop_front();
    }
    while (!candidates.empty() &&
           values[candidates.back()] <= values[right]) {
      candidates.pop_back();
    }
    candidates.push_back(right);
    if (right + 1 >= window) {
      answer.push_back(values[candidates.front()]);
    }
  }
  return answer;
}
// --8<-- [end:sliding-window]

// --8<-- [start:binary-search]
inline int binary_search_index(
    const std::vector<int>& values, int target) {
  int low = 0;
  int high = static_cast<int>(values.size()) - 1;
  while (low <= high) {
    const int middle = low + (high - low) / 2;
    if (values[middle] == target) {
      return middle;
    }
    if (values[middle] < target) {
      low = middle + 1;
    } else {
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
  int low = 0;
  int high = size;
  while (low < high) {
    const int middle = low + (high - low) / 2;
    if (predicate(middle)) {
      high = middle;
    } else {
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
  distance[source] = 0;
  frontier.push(source);
  while (!frontier.empty()) {
    const int node = frontier.front();
    frontier.pop();
    for (const int neighbor : graph[node]) {
      if (distance[neighbor] == -1) {
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
    for (const auto [neighbor, weight] : edges) {
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
    const auto [current, node] = frontier.top();
    frontier.pop();
    if (current != distance[node]) {
      continue;
    }
    for (const auto [neighbor, weight] : graph[node]) {
      if (current > infinity - weight) {
        continue;
      }
      const long long candidate = current + weight;
      if (candidate < distance[neighbor]) {
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
    const auto [left, right] = edges[id];
    validate_vertex(left, graph.size(), "edge endpoint");
    validate_vertex(right, graph.size(), "edge endpoint");
    graph[left].push_back({right, id});
    graph[right].push_back({left, id});
  }
  std::vector<int> entered(vertex_count, -1);
  std::vector<int> low(vertex_count);
  std::vector<std::pair<int, int>> bridges;
  int timer = 0;
  std::function<void(int, int)> search = [&](int node, int parent_edge) {
    entered[node] = low[node] = timer++;
    for (const auto [neighbor, edge_id] : graph[node]) {
      if (edge_id == parent_edge) {
        continue;
      }
      if (entered[neighbor] >= 0) {
        low[node] = std::min(low[node], entered[neighbor]);
        continue;
      }
      search(neighbor, edge_id);
      low[node] = std::min(low[node], low[neighbor]);
      if (low[neighbor] > entered[node]) {
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
  std::sort(values.begin(), values.end());
  std::vector<std::vector<int>> answer;
  std::vector<int> path;
  std::function<void(int)> search = [&](int start) {
    answer.push_back(path);
    for (int index = start; index < static_cast<int>(values.size()); ++index) {
      if (index > start && values[index] == values[index - 1]) {
        continue;
      }
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
  for (const auto [weight, value] : items) {
    if (weight <= 0) {
      throw std::invalid_argument("item weights must be positive");
    }
    for (int current = capacity; current >= weight; --current) {
      best[current] =
          std::max(best[current], best[current - weight] + value);
    }
  }
  return best[capacity];
}
// --8<-- [end:knapsack]

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

// --8<-- [start:kruskal]
inline long long minimum_spanning_tree_weight(
    int vertex_count,
    const std::vector<std::tuple<int, int, long long>>& edges) {
  if (vertex_count < 0) {
    throw std::invalid_argument("vertex_count must be nonnegative");
  }
  for (const auto [left, right, weight] : edges) {
    static_cast<void>(weight);
    if (left < 0 || right < 0 || left >= vertex_count ||
        right >= vertex_count) {
      throw std::invalid_argument("edge endpoint must be a valid vertex");
    }
  }

  auto ordered = edges;
  std::sort(ordered.begin(), ordered.end(),
            [](const auto& left, const auto& right) {
              return std::get<2>(left) < std::get<2>(right);
            });

  DisjointSet groups(vertex_count);
  long long total = 0;
  int accepted = 0;
  for (const auto [left, right, weight] : ordered) {
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
    lower_.push(value);
    upper_.push(lower_.top());
    lower_.pop();
    if (upper_.size() > lower_.size()) {
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

}  // namespace dsa_atlas
