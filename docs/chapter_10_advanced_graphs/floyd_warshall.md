---
description: Derive all-pairs shortest paths from an intermediate-vertex dynamic-programming state.
sample_status: conceptual
---

# Floyd–Warshall

Use Floyd–Warshall when shortest distances are needed between many or all
vertex pairs and `V` is small enough for cubic time and quadratic memory.

## The DP state

Before processing intermediate vertex `k`, `distance[i][j]` is the shortest
known path from `i` to `j` whose internal vertices come only from
`0 .. k - 1`.

When `k` becomes available, every best path either:

- skips `k`, keeping `distance[i][j]`; or
- uses `k`, combining `distance[i][k] + distance[k][j]`.

That gives the recurrence:

```text
distance[i][j] =
    min(distance[i][j], distance[i][k] + distance[k][j])
```

## Annotated blueprint

```text
distance[i][i] = 0
distance[u][v] = min(existing, edge_weight)        # keep best parallel edge

for k in vertices:                                 # newly allowed intermediate
    for i in vertices:
        if distance[i][k] is infinity: continue
        for j in vertices:
            if distance[k][j] is infinity: continue
            distance[i][j] = min(
                distance[i][j],
                distance[i][k] + distance[k][j]    # route through k
            )
```

After all iterations, `distance[v][v] < 0` proves that `v` lies on or can
participate in a negative cycle under the represented reachability.

## When to choose it

- `V` is a few hundred or less, depending on language and limits.
- The graph is dense.
- Many pair queries justify preprocessing.
- Edge weights may be negative, but shortest paths are meaningful only without
  relevant negative cycles.

For sparse graphs with nonnegative weights, repeated Dijkstra may be more
appropriate.

## Complexity and traps

- Time: `O(V³)`.
- Space: `O(V²)`.
- Initialize the diagonal to zero.
- Take the minimum over parallel edges.
- Guard infinity before addition and consider overflow.
- Put `k` in the outer loop; it represents the DP stage.

## Reference

- [Princeton: Floyd–Warshall API](https://algs4.cs.princeton.edu/code/javadoc/edu/princeton/cs/algs4/FloydWarshall.html)
- [Princeton: shortest-path chapter](https://algs4.cs.princeton.edu/44sp/)

!!! note "Implementation status"

    The recurrence is reviewed here, but Floyd–Warshall is not yet a tested
    copy-ready template in the repository.
