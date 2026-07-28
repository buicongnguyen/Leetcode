"""Behavior and regression tests for the Python reference library."""

from __future__ import annotations

import math
import sys
import unittest
from pathlib import Path

PYTHON_CODES = Path(__file__).resolve().parents[2] / "codes" / "python"
sys.path.insert(0, str(PYTHON_CODES))

from dsa_atlas import (  # noqa: E402
    DisjointSet,
    MedianFinder,
    bfs_distances,
    binary_search,
    count_subarrays_sum,
    dijkstra,
    find_bridges,
    first_true,
    knapsack_01,
    lower_bound,
    max_sliding_window,
    minimum_spanning_tree_weight,
    topological_order,
    TreeNode,
    tree_height,
    two_sum,
    unique_subsets,
)


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


if __name__ == "__main__":
    unittest.main()
