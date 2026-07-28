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
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dsa_atlas {

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
  constexpr long long infinity = std::numeric_limits<long long>::max() / 4;
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
      if (weight < 0) {
        throw std::invalid_argument("Dijkstra requires nonnegative weights");
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

inline std::vector<int> topological_order(
    const std::vector<std::vector<int>>& graph) {
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

inline std::vector<std::pair<int, int>> find_bridges(
    int vertex_count, const std::vector<std::pair<int, int>>& edges) {
  std::vector<std::vector<std::pair<int, int>>> graph(vertex_count);
  for (int id = 0; id < static_cast<int>(edges.size()); ++id) {
    const auto [left, right] = edges[id];
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

class DisjointSet {
 public:
  explicit DisjointSet(int size)
      : parent_(size), component_size_(size, 1) {
    for (int index = 0; index < size; ++index) {
      parent_[index] = index;
    }
  }

  int find(int item) {
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
  std::vector<int> parent_;
  std::vector<int> component_size_;
};

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
