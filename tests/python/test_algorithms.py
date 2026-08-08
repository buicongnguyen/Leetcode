"""Behavior and regression tests for the Python reference library."""

from __future__ import annotations

import math
import sys
import unittest
from pathlib import Path

PYTHON_CODES = Path(__file__).resolve().parents[2] / "codes" / "python"
sys.path.insert(0, str(PYTHON_CODES))

from dsa_atlas import (  # noqa: E402
    BinaryLiftingTree,
    Dinic,
    DisjointSet,
    FenwickTree,
    LRUCache,
    ListNode,
    MedianFinder,
    MinStack,
    NumberContainers,
    RandomizedSet,
    SegmentTree,
    SnapshotArray,
    TimeMap,
    Trie,
    a_star_grid,
    bellman_ford,
    bfs_distances,
    binary_search,
    cherry_pickup_two_robots,
    count_distinct_digit_numbers,
    count_set_bits,
    count_subarrays_sum,
    dijkstra,
    find_bridges,
    floyd_warshall,
    first_true,
    kmp_search,
    knapsack_01,
    linked_values,
    longest_common_subsequence_length,
    lower_bound,
    max_non_adjacent_sum,
    max_non_overlapping_intervals,
    max_sliding_window,
    minimum_coins,
    minimum_spanning_tree_weight,
    reverse_linked_list,
    strongly_connected_components,
    topological_order,
    TreeNode,
    tree_height,
    two_sum,
    unique_subsets,
)


class CoreToolkitTests(unittest.TestCase):
    def test_linked_list_reversal_preserves_every_node(self) -> None:
        head = ListNode(1, ListNode(2, ListNode(3)))
        self.assertEqual(linked_values(reverse_linked_list(head)), [3, 2, 1])
        self.assertIsNone(reverse_linked_list(None))

    def test_greedy_bit_and_string_templates(self) -> None:
        self.assertEqual(max_non_overlapping_intervals([(1, 3), (2, 4), (3, 5)]), 2)
        with self.assertRaises(ValueError):
            max_non_overlapping_intervals([(3, 1)])
        self.assertEqual(count_set_bits(0b101101), 4)
        with self.assertRaises(ValueError):
            count_set_bits(-1)
        self.assertEqual(kmp_search("abxabcabcaby", "abcaby"), 6)
        self.assertEqual(kmp_search("abc", ""), 0)
        self.assertEqual(kmp_search("abc", "z"), -1)


class AdvancedTreeTests(unittest.TestCase):
    def test_trie_distinguishes_word_from_prefix(self) -> None:
        words = Trie()
        words.insert("cat")
        words.insert("car")
        self.assertTrue(words.contains("cat"))
        self.assertFalse(words.contains("ca"))
        self.assertTrue(words.starts_with("ca"))

    def test_fenwick_and_segment_tree_ranges(self) -> None:
        with self.assertRaises(ValueError):
            FenwickTree(-1)
        fenwick = FenwickTree(4)
        for index, value in enumerate([2, -1, 4, 3]):
            fenwick.add(index, value)
        self.assertEqual(fenwick.prefix_sum(3), 5)
        self.assertEqual(fenwick.range_sum(1, 4), 6)

        segments = SegmentTree([1, 2, 3, 4])
        self.assertEqual(segments.range_sum(1, 3), 5)
        segments.set(2, 10)
        self.assertEqual(segments.range_sum(1, 3), 12)


class AdvancedGraphTests(unittest.TestCase):
    def test_negative_edges_and_all_pairs_shortest_paths(self) -> None:
        edges = [(0, 1, 4), (0, 2, 5), (1, 2, -2)]
        self.assertEqual(bellman_ford(3, edges, 0), [0, 4, 2])
        with self.assertRaises(ValueError):
            bellman_ford(2, [(0, 1, -1), (1, 0, -1)], 0)

        matrix = [[0, 3, math.inf], [math.inf, 0, 2], [1, math.inf, 0]]
        self.assertEqual(floyd_warshall(matrix), [[0, 3, 5], [3, 0, 2], [1, 4, 0]])

    def test_scc_lca_flow_and_a_star(self) -> None:
        components = strongly_connected_components([[1], [2, 3], [0], [4], []])
        self.assertEqual({frozenset(group) for group in components}, {frozenset({0, 1, 2}), frozenset({3}), frozenset({4})})

        ancestors = BinaryLiftingTree([[1, 2], [0, 3, 4], [0], [1], [1]])
        self.assertEqual(ancestors.lca(3, 4), 1)
        self.assertEqual(ancestors.lca(2, 4), 0)
        with self.assertRaises(ValueError):
            BinaryLiftingTree([[0]])

        with self.assertRaises(ValueError):
            Dinic(-1)
        flow = Dinic(4)
        for left, right, capacity in [(0, 1, 3), (0, 2, 2), (1, 2, 1), (1, 3, 2), (2, 3, 3)]:
            flow.add_edge(left, right, capacity)
        self.assertEqual(flow.max_flow(0, 3), 5)

        grid = [[0, 0, 0], [1, 1, 0], [0, 0, 0]]
        self.assertEqual(a_star_grid(grid, (0, 0), (2, 2)), 4)
        self.assertEqual(a_star_grid([[0, 1]], (0, 0), (0, 1)), -1)


class AdditionalCompositeTests(unittest.TestCase):
    def test_number_containers_discards_stale_membership(self) -> None:
        values = NumberContainers()
        values.change(2, 10)
        values.change(1, 10)
        self.assertEqual(values.find(10), 1)
        values.change(1, 20)
        self.assertEqual(values.find(10), 2)
        self.assertEqual(values.find(20), 1)

    def test_snapshot_array_reads_sparse_history(self) -> None:
        with self.assertRaises(ValueError):
            SnapshotArray(-1)
        values = SnapshotArray(2)
        values.set(0, 5)
        values.set(0, 6)
        first = values.snap()
        values.set(0, 9)
        second = values.snap()
        self.assertEqual(values.get(0, first), 6)
        self.assertEqual(values.get(0, second), 9)
        self.assertEqual(values.get(1, first), 0)


class ArrayTests(unittest.TestCase):
    def test_two_sum_uses_distinct_indices(self) -> None:
        self.assertEqual(two_sum([3, 3], 6), (0, 1))
        self.assertIsNone(two_sum([1, 2], 9))

    def test_prefix_sum_handles_negative_values_and_zero_prefix(self) -> None:
        self.assertEqual(count_subarrays_sum([1, -1, 1, -1], 0), 4)
        self.assertEqual(count_subarrays_sum([3], 3), 1)

    def test_sliding_window_validates_boundaries(self) -> None:
        self.assertEqual(max_sliding_window([1, 3, -1, -3, 5, 3, 6, 7], 3), [3, 3, 5, 5, 6, 7])
        self.assertEqual(max_sliding_window([4], 1), [4])
        with self.assertRaises(ValueError):
            max_sliding_window([], 0)


class SearchTests(unittest.TestCase):
    def test_binary_search_and_boundaries(self) -> None:
        values = [1, 3, 3, 7]
        self.assertEqual(values[binary_search(values, 3)], 3)
        self.assertEqual(binary_search(values, 6), -1)
        self.assertEqual(lower_bound(values, 3), 1)
        self.assertEqual(lower_bound(values, 9), len(values))
        self.assertEqual(first_true(0, lambda _: True), 0)
        with self.assertRaises(ValueError):
            first_true(-1, lambda _: True)


class GraphTests(unittest.TestCase):
    def test_bfs_distances_include_unreachable(self) -> None:
        self.assertEqual(bfs_distances([[1], [0, 2], [1], []], 0), [0, 1, 2, -1])
        with self.assertRaises(ValueError):
            bfs_distances([[1]], 0)
        with self.assertRaises(ValueError):
            bfs_distances([[]], -1)

    def test_dijkstra_and_negative_guard(self) -> None:
        graph = [[(1, 5), (2, 1)], [(3, 1)], [(1, 1), (3, 9)], []]
        self.assertEqual(dijkstra(graph, 0), [0, 2, 1, 3])
        disconnected = dijkstra([[], []], 0)
        self.assertTrue(math.isinf(disconnected[1]))
        with self.assertRaises(ValueError):
            dijkstra([[(1, -1)], []], 0)
        with self.assertRaises(ValueError):
            dijkstra([[], [(0, -1)]], 0)

    def test_topological_order_rejects_cycles(self) -> None:
        order = topological_order([[1, 2], [3], [3], []])
        position = {node: index for index, node in enumerate(order)}
        self.assertLess(position[0], position[1])
        self.assertLess(position[0], position[2])
        self.assertLess(position[1], position[3])
        with self.assertRaises(ValueError):
            topological_order([[1], [0]])
        with self.assertRaises(ValueError):
            topological_order([[2], []])

    def test_bridges_handle_disconnected_and_parallel_edges(self) -> None:
        self.assertEqual(find_bridges(5, [(0, 1), (1, 2), (2, 0), (3, 4)]), [(3, 4)])
        self.assertEqual(find_bridges(2, [(0, 1), (0, 1)]), [])
        with self.assertRaises(ValueError):
            find_bridges(2, [(0, 2)])

    def test_kruskal_builds_a_tree_and_rejects_disconnection(self) -> None:
        edges = [(0, 1, 4), (0, 2, 1), (2, 1, 2), (1, 3, 1), (2, 3, 5)]
        self.assertEqual(minimum_spanning_tree_weight(4, edges), 4)
        self.assertEqual(minimum_spanning_tree_weight(0, []), 0)
        with self.assertRaises(ValueError):
            minimum_spanning_tree_weight(3, [(0, 1, 1)])


class StateTests(unittest.TestCase):
    def test_tree_height_follows_the_recursive_contract(self) -> None:
        root = TreeNode(1, TreeNode(2, TreeNode(4)), TreeNode(3))
        self.assertEqual(tree_height(root), 3)
        self.assertEqual(tree_height(None), 0)

    def test_unique_subsets_sorts_before_duplicate_skip(self) -> None:
        result = unique_subsets([2, 1, 2])
        self.assertEqual(len(result), 6)
        self.assertEqual(len({tuple(item) for item in result}), 6)

    def test_knapsack_uses_each_item_once(self) -> None:
        self.assertEqual(knapsack_01(4, [(2, 3), (2, 3), (3, 5)]), 6)
        with self.assertRaises(ValueError):
            knapsack_01(4, [(0, 9)])

    def test_one_dimensional_dp_allows_the_empty_choice(self) -> None:
        self.assertEqual(max_non_adjacent_sum([3, 7, 4, 6, 5]), 13)
        self.assertEqual(max_non_adjacent_sum([-8, -2, -5]), 0)

    def test_lcs_handles_matches_and_empty_strings(self) -> None:
        self.assertEqual(longest_common_subsequence_length("abcde", "ace"), 3)
        self.assertEqual(longest_common_subsequence_length("", "abc"), 0)

    def test_unbounded_coin_change_distinguishes_impossible_amounts(self) -> None:
        self.assertEqual(minimum_coins([1, 2, 5], 11), 3)
        self.assertEqual(minimum_coins([2], 3), -1)
        with self.assertRaises(ValueError):
            minimum_coins([0, 1], 3)

    def test_three_dimensional_dp_tracks_both_robots(self) -> None:
        grid = [[3, 1, 1], [2, 5, 1], [1, 5, 5], [2, 1, 1]]
        self.assertEqual(cherry_pickup_two_robots(grid), 24)
        self.assertEqual(
            cherry_pickup_two_robots([[-10**19], [-10**19]]),
            -2 * 10**19,
        )
        with self.assertRaises(ValueError):
            cherry_pickup_two_robots([[1], [2, 3]])

    def test_digit_dp_tracks_tight_started_and_used_digits(self) -> None:
        self.assertEqual(count_distinct_digit_numbers(20), 19)
        self.assertEqual(count_distinct_digit_numbers(5), 5)
        self.assertEqual(count_distinct_digit_numbers(0), 0)

    def test_disjoint_set(self) -> None:
        groups = DisjointSet(4)
        self.assertTrue(groups.union(0, 1))
        self.assertFalse(groups.union(0, 1))
        groups.union(2, 3)
        self.assertNotEqual(groups.find(0), groups.find(2))
        with self.assertRaises(ValueError):
            groups.find(-1)

    def test_streaming_median_extremes_and_empty(self) -> None:
        finder = MedianFinder()
        with self.assertRaises(ValueError):
            finder.median()
        finder.add(2_147_483_647)
        finder.add(2_147_483_647)
        self.assertEqual(finder.median(), 2_147_483_647.0)
        finder.add(-2_147_483_648)
        self.assertEqual(finder.median(), 2_147_483_647.0)

    def test_lru_cache_synchronizes_lookup_and_recency(self) -> None:
        cache = LRUCache(2)
        cache.put(1, 10)
        cache.put(2, 20)
        self.assertEqual(cache.get(1), 10)
        cache.put(3, 30)
        self.assertEqual(cache.get(2), -1)
        self.assertEqual(cache.get(3), 30)
        empty = LRUCache(0)
        empty.put(1, 1)
        self.assertEqual(empty.get(1), -1)

    def test_randomized_set_repairs_swapped_index(self) -> None:
        values = RandomizedSet(seed=7)
        self.assertTrue(values.insert(10))
        self.assertTrue(values.insert(20))
        self.assertTrue(values.insert(30))
        self.assertTrue(values.remove(20))
        self.assertFalse(values.remove(20))
        self.assertIn(values.get_random(), {10, 30})

    def test_time_map_binary_searches_each_key_history(self) -> None:
        values = TimeMap()
        values.set("mode", "draft", 2)
        values.set("mode", "published", 5)
        self.assertEqual(values.get("mode", 1), "")
        self.assertEqual(values.get("mode", 4), "draft")
        self.assertEqual(values.get("mode", 5), "published")
        self.assertEqual(values.get("missing", 9), "")
        with self.assertRaises(ValueError):
            values.set("mode", "invalid", 5)

    def test_min_stack_restores_previous_minimum(self) -> None:
        values = MinStack()
        values.push(3)
        values.push(1)
        values.push(2)
        self.assertEqual(values.get_min(), 1)
        self.assertEqual(values.pop(), 2)
        self.assertEqual(values.pop(), 1)
        self.assertEqual(values.get_min(), 3)


if __name__ == "__main__":
    unittest.main()
    kmp_search,
    linked_values,
    reverse_linked_list,
    strongly_connected_components,
