---
description: A searchable practice catalog mapping problem signals to chapters, patterns, and a first reasoning question.
---

# Problem catalog

Use this catalog after learning a pattern. Search the page for a signal such as
“shortest,” “prefix,” “capacity,” or “version.” Before opening a solution, write
the state, invariant, and target complexity.

Difficulty labels are orientation only: `E` easy, `M` medium, `H` hard.

## Arrays, hashing, and windows

| Problem | Level | Signal → pattern | Ask first |
| --- | --- | --- | --- |
| [Two Sum](https://leetcode.com/problems/two-sum/) | E | complement → hash map | What belongs to the scanned past? |
| [Group Anagrams](https://leetcode.com/problems/group-anagrams/) | M | canonical key → hash groups | Which representation makes equivalents equal? |
| [Subarray Sum Equals K](https://leetcode.com/problems/subarray-sum-equals-k/) | M | signed subarray → prefix frequency | Which earlier prefix completes the target? |
| [Longest Substring Without Repeating Characters](https://leetcode.com/problems/longest-substring-without-repeating-characters/) | M | monotone validity → window | What forces the left edge forward? |
| [Sliding Window Maximum](https://leetcode.com/problems/sliding-window-maximum/) | H | fixed-window extreme → deque | Which candidates are permanently dominated? |

Read [Chapter 3](../chapter_03_arrays_hashing/index.md) and
[Chapter 4](../chapter_04_linear_structures/index.md).

## Linked lists and composite ownership

| Problem | Level | Signal → pattern | Ask first |
| --- | --- | --- | --- |
| [Reverse Linked List](https://leetcode.com/problems/reverse-linked-list/) | E | pointer rewiring → reversed prefix | Did I save the unread suffix? |
| [Linked List Cycle II](https://leetcode.com/problems/linked-list-cycle-ii/) | M | cycle entry → slow/fast | What distance relation holds at the meeting? |
| [LRU Cache](https://leetcode.com/problems/lru-cache/) | M | lookup + recency → map + list | Which structure owns identity and which owns order? |
| [LFU Cache](https://leetcode.com/problems/lfu-cache/) | H | lookup + frequency + recency | What moves atomically after access? |
| [Snapshot Array](https://leetcode.com/problems/snapshot-array/) | M | versions + sparse writes → histories + binary search | What is the newest visible change? |

Read [linked lists](../chapter_12_core_toolkit/linked_lists.md) and
[Chapter 11](../chapter_11_composite_structures/index.md).

## Trees, heaps, and range indexes

| Problem | Level | Signal → pattern | Ask first |
| --- | --- | --- | --- |
| [Binary Tree Level Order Traversal](https://leetcode.com/problems/binary-tree-level-order-traversal/) | M | layers → BFS queue | When does one level end? |
| [Validate Binary Search Tree](https://leetcode.com/problems/validate-binary-search-tree/) | M | global ordering → bounds | Which ancestor bounds constrain this node? |
| [Kth Largest Element in an Array](https://leetcode.com/problems/kth-largest-element-in-an-array/) | M | top k → heap/selection | Must all values be sorted? |
| [Implement Trie](https://leetcode.com/problems/implement-trie-prefix-tree/) | M | prefix lookup → Trie | Where is word termination stored? |
| [Range Sum Query – Mutable](https://leetcode.com/problems/range-sum-query-mutable/) | M | updates + ranges → Fenwick/segment | Is update additive or assignment? |

Read [Chapter 5](../chapter_05_trees_heaps/index.md) and
[Chapter 13](../chapter_13_advanced_trees/index.md).

## Graph traversal and structure

| Problem | Level | Signal → pattern | Ask first |
| --- | --- | --- | --- |
| [Number of Islands](https://leetcode.com/problems/number-of-islands/) | M | components → DFS/BFS | What creates an edge between cells? |
| [Rotting Oranges](https://leetcode.com/problems/rotting-oranges/) | M | simultaneous waves → multi-source BFS | Which sources enter at time zero? |
| [Course Schedule](https://leetcode.com/problems/course-schedule/) | M | prerequisites → topological sort | Is the request an order or only cycle detection? |
| [Network Delay Time](https://leetcode.com/problems/network-delay-time/) | M | nonnegative shortest paths → Dijkstra | Are all edge weights nonnegative? |
| [Critical Connections in a Network](https://leetcode.com/problems/critical-connections-in-a-network/) | H | edge failure → bridges | Can the child subtree reach an ancestor? |

Read [Chapter 6](../chapter_06_graphs/index.md).

## Advanced graphs

| Problem | Level | Signal → pattern | Ask first |
| --- | --- | --- | --- |
| [Min Cost to Connect All Points](https://leetcode.com/problems/min-cost-to-connect-all-points/) | M | cheapest connectivity → MST | Is a path cost or whole-network cost requested? |
| [Cheapest Flights Within K Stops](https://leetcode.com/problems/cheapest-flights-within-k-stops/) | M | bounded edges → layered Bellman–Ford/DP | Which iteration count represents allowed edges? |
| [Find the City With the Smallest Number of Neighbors](https://leetcode.com/problems/find-the-city-with-the-smallest-number-of-neighbors-at-a-threshold-distance/) | M | all sources + small graph → Floyd/Dijkstra | Which `V` and density bound wins? |
| [Maximum Employees to Be Invited to a Meeting](https://leetcode.com/problems/maximum-employees-to-be-invited-to-a-meeting/) | H | functional-graph cycles → SCC/cycle decomposition | Which vertices share a cycle? |
| [Maximum Number of Accepted Invitations](https://leetcode.com/problems/maximum-number-of-accepted-invitations/) | M | bipartite assignment → matching/flow | What are the left and right partitions? |

Read [Chapter 10](../chapter_10_advanced_graphs/index.md).

## Binary search and greedy

| Problem | Level | Signal → pattern | Ask first |
| --- | --- | --- | --- |
| [Search in Rotated Sorted Array](https://leetcode.com/problems/search-in-rotated-sorted-array/) | M | one sorted half → exact binary search | Which half is provably ordered? |
| [Koko Eating Bananas](https://leetcode.com/problems/koko-eating-bananas/) | M | minimum feasible rate → first true | Is feasibility monotone? |
| [Find Minimum in Rotated Sorted Array](https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/) | M | transition boundary → first true | Which side contains the pivot? |
| [Non-overlapping Intervals](https://leetcode.com/problems/non-overlapping-intervals/) | M | maximize compatible count → earliest finish | Can an optimum exchange its first interval? |
| [Jump Game](https://leetcode.com/problems/jump-game/) | M | reachable prefix → stay-ahead greedy | What is the farthest certified index? |

Read [Chapter 7](../chapter_07_search_sort/index.md) and
[greedy algorithms](../chapter_12_core_toolkit/greedy.md).

## Backtracking and state search

| Problem | Level | Signal → pattern | Ask first |
| --- | --- | --- | --- |
| [Subsets II](https://leetcode.com/problems/subsets-ii/) | M | choose/skip + duplicates → sorted backtracking | Which equal choice is redundant at this depth? |
| [Combination Sum](https://leetcode.com/problems/combination-sum/) | M | reuse candidates → backtracking | Does the next call keep or advance the index? |
| [Permutations](https://leetcode.com/problems/permutations/) | M | ordered choices → used set | What defines one decision-tree level? |
| [Word Search](https://leetcode.com/problems/word-search/) | M | path with undo → grid backtracking | Which state must be restored? |
| [N-Queens](https://leetcode.com/problems/n-queens/) | H | constraint placement → backtracking sets | Which conflicts can be checked in `O(1)`? |

Read [Chapter 8](../chapter_08_backtracking/index.md).

## Dynamic programming

| Problem | Level | Signal → state shape | Ask first |
| --- | --- | --- | --- |
| [House Robber](https://leetcode.com/problems/house-robber/) | M | local exclusion → `dp[i]` | What does the prefix answer promise? |
| [Coin Change](https://leetcode.com/problems/coin-change/) | M | reusable amounts → `dp[amount]` | Is reuse allowed in the same stage? |
| [Longest Common Subsequence](https://leetcode.com/problems/longest-common-subsequence/) | M | two prefixes → `dp[i][j]` | What happens on match versus mismatch? |
| [Cherry Pickup II](https://leetcode.com/problems/cherry-pickup-ii/) | H | row + two agents → `dp[row][c1][c2]` | Which coordinates determine the future? |
| [Burst Balloons](https://leetcode.com/problems/burst-balloons/) | H | choose last in interval → interval DP | Which choice makes the two sides independent? |

Read [Chapter 9](../chapter_09_dynamic_programming/index.md).

## External practice sets

- [LeetCode 75 study plan](https://leetcode.com/studyplan/leetcode-75/)
- [LeetCode Top Interview 150](https://leetcode.com/studyplan/top-interview-150/)
- [HackerRank Interview Preparation Kit](https://www.hackerrank.com/interview/interview-preparation-kit)

Use the platform for the prompt and judge; use this book for choosing the model,
stating the invariant, and reviewing the failure modes.
