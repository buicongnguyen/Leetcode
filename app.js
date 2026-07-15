const templates = [
  {
    title: "Frequency map / complement lookup", category: "Arrays", complexity: "O(n) time · O(n) space",
    when: "Counting, deduplication, anagrams, two-sum, or remembering prior states.",
    invariant: "The map contains exactly the elements already processed.",
    cpp: `unordered_map<int, int> pos;
for (int i = 0; i < (int)nums.size(); ++i) {
    int need = target - nums[i];
    if (pos.count(need)) return {pos[need], i};
    pos[nums[i]] = i;
}`,
    python: `pos = {}
for i, x in enumerate(nums):
    need = target - x
    if need in pos:
        return [pos[need], i]
    pos[x] = i`
  },
  {
    title: "Prefix sum / subarray count", category: "Arrays", complexity: "O(n) time · O(n) space",
    when: "Many range sums or counting subarrays whose sum satisfies an equation.",
    invariant: "seen stores frequencies of prefix sums strictly before the current position.",
    cpp: `unordered_map<long long, int> seen{{0, 1}};
long long sum = 0, answer = 0;
for (int x : nums) {
    sum += x;
    answer += seen[sum - k];
    ++seen[sum];
}`,
    python: `seen = {0: 1}
total = answer = 0
for x in nums:
    total += x
    answer += seen.get(total - k, 0)
    seen[total] = seen.get(total, 0) + 1`
  },
  {
    title: "Two pointers", category: "Arrays", complexity: "O(n) time · O(1) space",
    when: "Sorted pairs, palindrome checks, partitions, or in-place compaction.",
    invariant: "Every pair excluded outside [left, right] is already proven impossible.",
    cpp: `int left = 0, right = (int)a.size() - 1;
while (left < right) {
    long long sum = 1LL * a[left] + a[right];
    if (sum == target) return true;
    if (sum < target) ++left;
    else --right;
}
return false;`,
    python: `left, right = 0, len(a) - 1
while left < right:
    total = a[left] + a[right]
    if total == target:
        return True
    if total < target: left += 1
    else: right -= 1
return False`
  },
  {
    title: "Sliding window", category: "Arrays", complexity: "O(n) time · O(k) space",
    when: "A longest or shortest contiguous segment with a monotone validity rule.",
    invariant: "After shrinking, [left, right] is valid and left cannot move farther without losing a candidate.",
    cpp: `int left = 0, best = 0;
unordered_map<char, int> count;
for (int right = 0; right < (int)s.size(); ++right) {
    ++count[s[right]];
    while (!valid(count)) {
        if (--count[s[left]] == 0) count.erase(s[left]);
        ++left;
    }
    best = max(best, right - left + 1);
}`,
    python: `left = best = 0
count = {}
for right, ch in enumerate(s):
    count[ch] = count.get(ch, 0) + 1
    while not valid(count):
        count[s[left]] -= 1
        if count[s[left]] == 0: del count[s[left]]
        left += 1
    best = max(best, right - left + 1)`
  },
  {
    title: "Monotonic stack", category: "Linear", complexity: "O(n) time · O(n) space",
    when: "Next greater/smaller, temperatures, histogram boundaries, or dominated candidates.",
    invariant: "Indices in the stack have decreasing values and await their first greater value.",
    cpp: `vector<int> answer(n, -1), stack;
for (int i = 0; i < n; ++i) {
    while (!stack.empty() && a[stack.back()] < a[i]) {
        answer[stack.back()] = i;
        stack.pop_back();
    }
    stack.push_back(i);
}`,
    python: `answer = [-1] * len(a)
stack = []
for i, x in enumerate(a):
    while stack and a[stack[-1]] < x:
        answer[stack.pop()] = i
    stack.append(i)`
  },
  {
    title: "Monotonic deque", category: "Linear", complexity: "O(n) time · O(k) space",
    when: "The minimum or maximum of every fixed-size window.",
    invariant: "Deque indices are live and their values are decreasing from front to back.",
    cpp: `deque<int> dq;
vector<int> answer;
for (int right = 0; right < n; ++right) {
    while (!dq.empty() && dq.front() <= right - k) dq.pop_front();
    while (!dq.empty() && a[dq.back()] <= a[right]) dq.pop_back();
    dq.push_back(right);
    if (right + 1 >= k) answer.push_back(a[dq.front()]);
}`,
    python: `from collections import deque
dq, answer = deque(), []
for right, x in enumerate(a):
    while dq and dq[0] <= right - k: dq.popleft()
    while dq and a[dq[-1]] <= x: dq.pop()
    dq.append(right)
    if right + 1 >= k: answer.append(a[dq[0]])`
  },
  {
    title: "Linked-list reversal", category: "Linear", complexity: "O(n) time · O(1) space",
    when: "In-place reversal, palindrome halves, or k-group pointer rewiring.",
    invariant: "prev is the fully reversed prefix; cur begins the untouched suffix.",
    cpp: `ListNode* reverse(ListNode* head) {
    ListNode *prev = nullptr, *cur = head;
    while (cur) {
        ListNode* next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}`,
    python: `def reverse(head):
    prev, cur = None, head
    while cur:
        nxt = cur.next
        cur.next = prev
        prev, cur = cur, nxt
    return prev`
  },
  {
    title: "Heap / top-k", category: "Linear", complexity: "O(n log k) time · O(k) space",
    when: "Repeated minimum/maximum, k best items, or streaming selection.",
    invariant: "The min-heap contains the k largest items seen so far.",
    cpp: `priority_queue<int, vector<int>, greater<int>> heap;
for (int x : nums) {
    heap.push(x);
    if ((int)heap.size() > k) heap.pop();
}
int kthLargest = heap.top();`,
    python: `from heapq import heappush, heappop
heap = []
for x in nums:
    heappush(heap, x)
    if len(heap) > k:
        heappop(heap)
kth_largest = heap[0]`
  },
  {
    title: "Merge intervals", category: "Search & Sort", complexity: "O(n log n) time",
    when: "Overlapping ranges, schedules, coverage, or interval normalization.",
    invariant: "out is sorted, disjoint, and exactly covers every interval processed.",
    cpp: `sort(intervals.begin(), intervals.end());
vector<vector<int>> out;
for (auto& in : intervals) {
    if (out.empty() || out.back()[1] < in[0]) out.push_back(in);
    else out.back()[1] = max(out.back()[1], in[1]);
}`,
    python: `intervals.sort()
out = []
for start, end in intervals:
    if not out or out[-1][1] < start:
        out.append([start, end])
    else:
        out[-1][1] = max(out[-1][1], end)`
  },
  {
    title: "Binary search: first true", category: "Search & Sort", complexity: "O(log n) time",
    when: "A sorted domain or a false…false, true…true predicate.",
    invariant: "The first true answer always remains inside [lo, hi).",
    cpp: `int lo = 0, hi = n;
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (ok(mid)) hi = mid;
    else lo = mid + 1;
}
return lo;`,
    python: `lo, hi = 0, n
while lo < hi:
    mid = lo + (hi - lo) // 2
    if ok(mid): hi = mid
    else: lo = mid + 1
return lo`
  },
  {
    title: "Backtracking", category: "Search & Sort", complexity: "Usually exponential",
    when: "All subsets, arrangements, partitions, placements, or reversible choices.",
    invariant: "path contains exactly the choices on the current recursion branch.",
    cpp: `vector<vector<int>> answer;
vector<int> path;
void dfs(int start) {
    answer.push_back(path);
    for (int i = start; i < n; ++i) {
        if (i > start && a[i] == a[i - 1]) continue;
        path.push_back(a[i]);
        dfs(i + 1);
        path.pop_back();
    }
}`,
    python: `answer, path = [], []
def dfs(start):
    answer.append(path.copy())
    for i in range(start, len(a)):
        if i > start and a[i] == a[i - 1]: continue
        path.append(a[i])
        dfs(i + 1)
        path.pop()`
  },
  {
    title: "Binary tree DFS", category: "Trees", complexity: "O(n) time · O(h) stack",
    when: "Subtree aggregation, path properties, traversal, or tree depth.",
    invariant: "dfs(node) returns the defined result for the complete subtree rooted at node.",
    cpp: `int dfs(TreeNode* node) {
    if (!node) return 0;
    int left = dfs(node->left);
    int right = dfs(node->right);
    return 1 + max(left, right);
}`,
    python: `def dfs(node):
    if not node:
        return 0
    left = dfs(node.left)
    right = dfs(node.right)
    return 1 + max(left, right)`
  },
  {
    title: "Graph DFS", category: "Graphs", complexity: "O(V + E) time",
    when: "Components, reachability, or exhaustive graph exploration.",
    invariant: "Every marked node is reachable from the current component's start.",
    cpp: `vector<char> seen(n, false);
function<void(int)> dfs = [&](int u) {
    seen[u] = true;
    for (int v : graph[u])
        if (!seen[v]) dfs(v);
};`,
    python: `seen = [False] * n
def dfs(start):
    stack = [start]
    seen[start] = True
    while stack:
        u = stack.pop()
        for v in graph[u]:
            if not seen[v]:
                seen[v] = True
                stack.append(v)`
  },
  {
    title: "BFS shortest path", category: "Graphs", complexity: "O(V + E) time",
    when: "Minimum steps in an unweighted graph or grid.",
    invariant: "Nodes leave the queue in nondecreasing distance from the source.",
    cpp: `queue<int> q;
vector<int> dist(n, -1);
q.push(src); dist[src] = 0;
while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int v : graph[u]) if (dist[v] == -1) {
        dist[v] = dist[u] + 1;
        q.push(v);
    }
}`,
    python: `from collections import deque
q = deque([src])
dist = [-1] * n
dist[src] = 0
while q:
    u = q.popleft()
    for v in graph[u]:
        if dist[v] == -1:
            dist[v] = dist[u] + 1
            q.append(v)`
  },
  {
    title: "Topological sort", category: "Graphs", complexity: "O(V + E) time",
    when: "Prerequisites, dependency order, or cycle detection in a directed graph.",
    invariant: "The queue contains precisely the remaining nodes with zero indegree.",
    cpp: `vector<int> indegree(n);
for (int u = 0; u < n; ++u)
    for (int v : graph[u]) ++indegree[v];
queue<int> q;
for (int i = 0; i < n; ++i) if (!indegree[i]) q.push(i);
vector<int> order;
while (!q.empty()) {
    int u = q.front(); q.pop(); order.push_back(u);
    for (int v : graph[u]) if (--indegree[v] == 0) q.push(v);
}`,
    python: `from collections import deque
indegree = [0] * n
for u in range(n):
    for v in graph[u]: indegree[v] += 1
q = deque(i for i in range(n) if indegree[i] == 0)
order = []
while q:
    u = q.popleft(); order.append(u)
    for v in graph[u]:
        indegree[v] -= 1
        if indegree[v] == 0: q.append(v)`
  },
  {
    title: "Dijkstra", category: "Graphs", complexity: "O((V + E) log V)",
    when: "Single-source shortest paths with nonnegative edge weights.",
    invariant: "When a non-stale node is popped, its shortest distance is final.",
    cpp: `const long long INF = (1LL << 60);
vector<long long> dist(n, INF); dist[src] = 0;
priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
pq.push({0, src});
while (!pq.empty()) {
    auto [d, u] = pq.top(); pq.pop();
    if (d != dist[u]) continue;
    for (auto [v, w] : graph[u]) if (d + w < dist[v]) {
        dist[v] = d + w;
        pq.push({dist[v], v});
    }
}`,
    python: `from heapq import heappush, heappop
dist = [float("inf")] * n
dist[src] = 0
pq = [(0, src)]
while pq:
    d, u = heappop(pq)
    if d != dist[u]: continue
    for v, w in graph[u]:
        nd = d + w
        if nd < dist[v]:
            dist[v] = nd
            heappush(pq, (nd, v))`
  },
  {
    title: "Union-find (DSU)", category: "Graphs", complexity: "α(n) amortized",
    when: "Dynamic connectivity, merged groups, redundant edges, or Kruskal MST.",
    invariant: "Every component is represented by one root; size belongs only to roots.",
    cpp: `struct DSU {
    vector<int> parent, size;
    DSU(int n): parent(n), size(n, 1) { iota(parent.begin(), parent.end(), 0); }
    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a; size[a] += size[b];
        return true;
    }
};`,
    python: `class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
        self.size = [1] * n
    def find(self, x):
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]
    def union(self, a, b):
        a, b = self.find(a), self.find(b)
        if a == b: return False
        if self.size[a] < self.size[b]: a, b = b, a
        self.parent[b] = a; self.size[a] += self.size[b]
        return True`
  },
  {
    title: "Top-down DP", category: "Dynamic Programming", complexity: "states × transitions",
    when: "Overlapping states, sparse state space, and a natural recurrence.",
    invariant: "dp(state) equals the complete optimal answer from exactly that state.",
    cpp: `vector<long long> memo(n + 1, -1);
function<long long(int)> dp = [&](int i) -> long long {
    if (i <= 1) return i;
    long long& answer = memo[i];
    if (answer != -1) return answer;
    return answer = dp(i - 1) + dp(i - 2);
};`,
    python: `from functools import lru_cache

@lru_cache(None)
def dp(i):
    if i <= 1:
        return i
    return dp(i - 1) + dp(i - 2)`
  },
  {
    title: "0/1 knapsack", category: "Dynamic Programming", complexity: "O(nW) time · O(W) space",
    when: "Each item may be chosen at most once and capacity is bounded.",
    invariant: "After item i, dp[c] is the best value using only items through i.",
    cpp: `vector<long long> dp(capacity + 1, 0);
for (auto [weight, value] : items)
    for (int c = capacity; c >= weight; --c)
        dp[c] = max(dp[c], dp[c - weight] + value);`,
    python: `dp = [0] * (capacity + 1)
for weight, value in items:
    for c in range(capacity, weight - 1, -1):
        dp[c] = max(dp[c], dp[c - weight] + value)`
  },
  {
    title: "Bitmask subsets", category: "Math & Bits", complexity: "O(n · 2ⁿ) time",
    when: "Compact flags, subset enumeration, or bitmask dynamic programming.",
    invariant: "Bit i is set exactly when item i is selected.",
    cpp: `for (int mask = 0; mask < (1 << n); ++mask) {
    for (int i = 0; i < n; ++i) {
        if ((mask >> i) & 1) {
            // choose item i
        }
    }
}`,
    python: `for mask in range(1 << n):
    for i in range(n):
        if (mask >> i) & 1:
            pass  # choose item i`
  },
  {
    title: "Kruskal minimum spanning tree", category: "Advanced Graphs", level: "Advanced", complexity: "O(E log E) time",
    when: "Minimum total cost to connect every vertex in an undirected weighted graph.",
    invariant: "Accepted edges are always safe edges and never create a cycle.",
    cpp: `struct DSU {
    vector<int> p, size;
    DSU(int n): p(n), size(n, 1) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);
        p[b] = a; size[a] += size[b];
        return true;
    }
};

sort(edges.begin(), edges.end()); // {weight, u, v}
DSU dsu(n);
long long cost = 0;
int used = 0;
for (auto [weight, u, v] : edges) {
    if (dsu.unite(u, v)) {
        cost += weight;
        ++used;
    }
}
if (used != n - 1) throw runtime_error("disconnected");`,
    python: `class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
        self.size = [1] * n
    def find(self, x):
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]
    def union(self, a, b):
        a, b = self.find(a), self.find(b)
        if a == b: return False
        if self.size[a] < self.size[b]: a, b = b, a
        self.parent[b] = a
        self.size[a] += self.size[b]
        return True

dsu = DSU(n)
cost = used = 0
for weight, u, v in sorted(edges):
    if dsu.union(u, v):
        cost += weight
        used += 1
if used != n - 1:
    raise ValueError("graph is disconnected")`
  },
  {
    title: "Dinic maximum flow", category: "Advanced Graphs", level: "Advanced", complexity: "O(V²E) general",
    when: "Maximum capacity, bipartite matching, disjoint paths, or min-cut modeling.",
    invariant: "Each phase sends a blocking flow through the current level graph.",
    cpp: `struct Dinic {
    struct Edge { int to, rev; long long cap; };
    int n; vector<vector<Edge>> g; vector<int> level, it;
    Dinic(int n): n(n), g(n), level(n), it(n) {}
    void addEdge(int u, int v, long long cap) {
        Edge a{v, (int)g[v].size(), cap};
        Edge b{u, (int)g[u].size(), 0};
        g[u].push_back(a); g[v].push_back(b);
    }
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q; q.push(s); level[s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : g[u]) if (e.cap && level[e.to] < 0)
                level[e.to] = level[u] + 1, q.push(e.to);
        }
        return level[t] >= 0;
    }
    long long dfs(int u, int t, long long pushed) {
        if (u == t || !pushed) return pushed;
        for (int &i = it[u]; i < (int)g[u].size(); ++i) {
            Edge &e = g[u][i];
            if (e.cap && level[e.to] == level[u] + 1) {
                long long flow = dfs(e.to, t, min(pushed, e.cap));
                if (flow) { e.cap -= flow; g[e.to][e.rev].cap += flow; return flow; }
            }
        }
        return 0;
    }
    long long maxFlow(int s, int t) {
        long long flow = 0, pushed, INF = (1LL << 60);
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while ((pushed = dfs(s, t, INF))) flow += pushed;
        }
        return flow;
    }
};`,
    python: `from collections import deque

class Dinic:
    def __init__(self, n):
        self.g = [[] for _ in range(n)]
    def add_edge(self, u, v, cap):
        self.g[u].append([v, cap, len(self.g[v])])
        self.g[v].append([u, 0, len(self.g[u]) - 1])
    def bfs(self, s, t):
        self.level = [-1] * len(self.g)
        self.level[s] = 0
        q = deque([s])
        while q:
            u = q.popleft()
            for v, cap, _ in self.g[u]:
                if cap and self.level[v] < 0:
                    self.level[v] = self.level[u] + 1
                    q.append(v)
        return self.level[t] >= 0
    def dfs(self, u, t, pushed):
        if u == t or not pushed: return pushed
        while self.it[u] < len(self.g[u]):
            i = self.it[u]
            v, cap, rev = self.g[u][i]
            if cap and self.level[v] == self.level[u] + 1:
                flow = self.dfs(v, t, min(pushed, cap))
                if flow:
                    self.g[u][i][1] -= flow
                    self.g[v][rev][1] += flow
                    return flow
            self.it[u] += 1
        return 0
    def max_flow(self, s, t):
        flow = 0
        while self.bfs(s, t):
            self.it = [0] * len(self.g)
            while pushed := self.dfs(s, t, 10**30):
                flow += pushed
        return flow`
  },
  {
    title: "Bellman–Ford shortest paths", category: "Advanced Graphs", level: "Advanced", complexity: "O(VE) time",
    when: "Shortest paths with negative edges or explicit negative-cycle detection.",
    invariant: "After pass i, every shortest path using at most i edges is correct.",
    cpp: `const long long INF = (1LL << 60);
vector<long long> dist(n, INF);
dist[source] = 0;
for (int pass = 0; pass < n - 1; ++pass) {
    bool changed = false;
    for (auto [u, v, weight] : edges) {
        if (dist[u] != INF && dist[u] + weight < dist[v]) {
            dist[v] = dist[u] + weight;
            changed = true;
        }
    }
    if (!changed) break;
}
bool negativeCycle = false;
for (auto [u, v, weight] : edges)
    if (dist[u] != INF && dist[u] + weight < dist[v])
        negativeCycle = true;`,
    python: `INF = float("inf")
dist = [INF] * n
dist[source] = 0
for _ in range(n - 1):
    changed = False
    for u, v, weight in edges:
        if dist[u] != INF and dist[u] + weight < dist[v]:
            dist[v] = dist[u] + weight
            changed = True
    if not changed:
        break
negative_cycle = any(
    dist[u] != INF and dist[u] + w < dist[v]
    for u, v, w in edges
)`
  },
  {
    title: "Floyd–Warshall all-pairs paths", category: "Advanced Graphs", level: "Advanced", complexity: "O(V³) time · O(V²) space",
    when: "All-pairs shortest paths on a small graph, including negative edges but no negative cycle.",
    invariant: "After k, dist[i][j] may use only intermediate vertices 0 through k.",
    cpp: `const long long INF = (1LL << 60);
vector<vector<long long>> dist(n, vector<long long>(n, INF));
for (int i = 0; i < n; ++i) dist[i][i] = 0;
for (auto [u, v, w] : edges) dist[u][v] = min(dist[u][v], w);
for (int k = 0; k < n; ++k)
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (dist[i][k] != INF && dist[k][j] != INF)
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);`,
    python: `INF = float("inf")
dist = [[INF] * n for _ in range(n)]
for i in range(n): dist[i][i] = 0
for u, v, weight in edges:
    dist[u][v] = min(dist[u][v], weight)
for k in range(n):
    for i in range(n):
        for j in range(n):
            if dist[i][k] != INF and dist[k][j] != INF:
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])`
  },
  {
    title: "Kosaraju strongly connected components", category: "Advanced Graphs", level: "Advanced", complexity: "O(V + E) time",
    when: "Condensing a directed graph, grouping mutual reachability, or solving 2-SAT-style structure.",
    invariant: "Reverse finishing order exposes one source component at a time in the transposed graph.",
    cpp: `vector<char> seen(n, false);
vector<int> order, component(n, -1);
function<void(int)> dfs1 = [&](int u) {
    seen[u] = true;
    for (int v : graph[u]) if (!seen[v]) dfs1(v);
    order.push_back(u);
};
for (int u = 0; u < n; ++u) if (!seen[u]) dfs1(u);

function<void(int,int)> dfs2 = [&](int u, int id) {
    component[u] = id;
    for (int v : reverseGraph[u]) if (component[v] < 0) dfs2(v, id);
};
reverse(order.begin(), order.end());
int count = 0;
for (int u : order) if (component[u] < 0) dfs2(u, count++);`,
    python: `seen, order = set(), []
def dfs1(u):
    seen.add(u)
    for v in graph[u]:
        if v not in seen: dfs1(v)
    order.append(u)
for u in range(n):
    if u not in seen: dfs1(u)

component = [-1] * n
def dfs2(u, group):
    component[u] = group
    for v in reverse_graph[u]:
        if component[v] < 0: dfs2(v, group)

count = 0
for u in reversed(order):
    if component[u] < 0:
        dfs2(u, count)
        count += 1`
  },
  {
    title: "Bridges in an undirected graph", category: "Advanced Graphs", level: "Advanced", complexity: "O(V + E) time",
    when: "Finding critical links whose removal disconnects an undirected graph.",
    invariant: "low[u] is the earliest discovery time reachable from u's DFS subtree without its parent edge.",
    cpp: `vector<int> tin(n, -1), low(n);
vector<pair<int,int>> bridges;
int timer = 0;
function<void(int,int)> dfs = [&](int u, int parentEdge) {
    tin[u] = low[u] = timer++;
    for (auto [v, edgeId] : graph[u]) {
        if (edgeId == parentEdge) continue;
        if (tin[v] >= 0) low[u] = min(low[u], tin[v]);
        else {
            dfs(v, edgeId);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) bridges.push_back({u, v});
        }
    }
};
for (int u = 0; u < n; ++u) if (tin[u] < 0) dfs(u, -1);`,
    python: `tin, low = [-1] * n, [0] * n
bridges = []
timer = 0
def dfs(u, parent_edge=-1):
    global timer
    tin[u] = low[u] = timer
    timer += 1
    for v, edge_id in graph[u]:
        if edge_id == parent_edge: continue
        if tin[v] >= 0:
            low[u] = min(low[u], tin[v])
        else:
            dfs(v, edge_id)
            low[u] = min(low[u], low[v])
            if low[v] > tin[u]:
                bridges.append((u, v))
for u in range(n):
    if tin[u] < 0: dfs(u)`
  },
  {
    title: "A* heuristic search", category: "Advanced Graphs", level: "Advanced", complexity: "Problem-dependent",
    when: "A shortest path to one target with an admissible heuristic, especially on geometric graphs or grids.",
    invariant: "The heap orders frontier states by known cost plus a lower bound on remaining cost.",
    cpp: `using State = tuple<long long,long long,int>; // f, g, node
priority_queue<State, vector<State>, greater<State>> open;
vector<long long> gScore(n, (1LL << 60));
gScore[start] = 0;
open.push({heuristic(start, goal), 0, start});
while (!open.empty()) {
    auto [f, g, u] = open.top(); open.pop();
    if (g != gScore[u]) continue;
    if (u == goal) break;
    for (auto [v, weight] : graph[u]) {
        long long next = g + weight;
        if (next < gScore[v]) {
            gScore[v] = next;
            open.push({next + heuristic(v, goal), next, v});
        }
    }
}`,
    python: `from heapq import heappush, heappop

g_score = [float("inf")] * n
g_score[start] = 0
open_set = [(heuristic(start, goal), 0, start)]
while open_set:
    _, cost, u = heappop(open_set)
    if cost != g_score[u]: continue
    if u == goal: break
    for v, weight in graph[u]:
        next_cost = cost + weight
        if next_cost < g_score[v]:
            g_score[v] = next_cost
            heappush(open_set, (
                next_cost + heuristic(v, goal), next_cost, v
            ))`
  },
  {
    title: "Lowest common ancestor: binary lifting", category: "Advanced Trees", level: "Advanced", complexity: "O(n log n) build · O(log n) query",
    when: "Many ancestor, distance, or path queries on a static rooted tree.",
    invariant: "up[k][v] is exactly the 2^k-th ancestor of v.",
    cpp: `int LOG = 1;
while ((1 << LOG) <= n) ++LOG;
vector<vector<int>> up(LOG, vector<int>(n));
vector<int> depth(n);
function<void(int,int)> dfs = [&](int u, int parent) {
    up[0][u] = parent;
    for (int k = 1; k < LOG; ++k) up[k][u] = up[k-1][up[k-1][u]];
    for (int v : tree[u]) if (v != parent) {
        depth[v] = depth[u] + 1;
        dfs(v, u);
    }
};
dfs(root, root);
auto lca = [&](int a, int b) {
    if (depth[a] < depth[b]) swap(a, b);
    for (int k = LOG - 1; k >= 0; --k)
        if (depth[a] - (1 << k) >= depth[b]) a = up[k][a];
    if (a == b) return a;
    for (int k = LOG - 1; k >= 0; --k)
        if (up[k][a] != up[k][b]) a = up[k][a], b = up[k][b];
    return up[0][a];
};`,
    python: `LOG = max(1, n.bit_length())
up = [[0] * n for _ in range(LOG)]
depth = [0] * n
def dfs(u, parent):
    up[0][u] = parent
    for k in range(1, LOG):
        up[k][u] = up[k - 1][up[k - 1][u]]
    for v in tree[u]:
        if v != parent:
            depth[v] = depth[u] + 1
            dfs(v, u)
dfs(root, root)

def lca(a, b):
    if depth[a] < depth[b]: a, b = b, a
    for k in range(LOG - 1, -1, -1):
        if depth[a] - (1 << k) >= depth[b]: a = up[k][a]
    if a == b: return a
    for k in range(LOG - 1, -1, -1):
        if up[k][a] != up[k][b]: a, b = up[k][a], up[k][b]
    return up[0][a]`
  },
  {
    title: "KD-tree nearest neighbor (2D)", category: "Spatial & Ordered", level: "Advanced", complexity: "O(n log n) build · O(log n) average query",
    when: "Nearest-neighbor queries over mostly static points in low-dimensional space.",
    invariant: "The far branch is pruned only when its splitting plane cannot beat the current best distance.",
    cpp: `struct Point { long long x, y; };
struct KDNode { Point p; KDNode *left=nullptr, *right=nullptr; };
KDNode* build(vector<Point>& points, int l, int r, int depth=0) {
    if (l >= r) return nullptr;
    int axis = depth & 1, mid = (l + r) / 2;
    nth_element(points.begin()+l, points.begin()+mid, points.begin()+r,
        [axis](Point a, Point b){ return axis ? a.y < b.y : a.x < b.x; });
    auto* node = new KDNode{points[mid]};
    node->left = build(points, l, mid, depth + 1);
    node->right = build(points, mid + 1, r, depth + 1);
    return node;
}
long long dist2(Point a, Point b) {
    long long dx=a.x-b.x, dy=a.y-b.y; return dx*dx + dy*dy;
}
void nearest(KDNode* node, Point q, int depth, Point& best) {
    if (!node) return;
    if (dist2(node->p, q) < dist2(best, q)) best = node->p;
    int axis = depth & 1;
    long long delta = axis ? q.y-node->p.y : q.x-node->p.x;
    KDNode *near = delta < 0 ? node->left : node->right;
    KDNode *far = delta < 0 ? node->right : node->left;
    nearest(near, q, depth + 1, best);
    if (delta * delta < dist2(best, q)) nearest(far, q, depth + 1, best);
}`,
    python: `class KDNode:
    def __init__(self, point, left=None, right=None):
        self.point, self.left, self.right = point, left, right

def build(points, depth=0):
    if not points: return None
    axis = depth % 2
    points.sort(key=lambda p: p[axis])
    mid = len(points) // 2
    return KDNode(points[mid],
        build(points[:mid], depth + 1),
        build(points[mid + 1:], depth + 1))

def dist2(a, b):
    return (a[0]-b[0])**2 + (a[1]-b[1])**2

def nearest(node, query, best=None, depth=0):
    if not node: return best
    if best is None or dist2(node.point, query) < dist2(best, query):
        best = node.point
    axis = depth % 2
    delta = query[axis] - node.point[axis]
    near, far = (node.left, node.right) if delta < 0 else (node.right, node.left)
    best = nearest(near, query, best, depth + 1)
    if delta * delta < dist2(best, query):
        best = nearest(far, query, best, depth + 1)
    return best`
  },
  {
    title: "Red-black tree insertion", category: "Spatial & Ordered", level: "Advanced", complexity: "O(log n) insert/search",
    when: "A dynamic ordered set needs guaranteed logarithmic insertion, lookup, predecessor, or successor.",
    invariant: "The root is black, red nodes have black children, and every root-to-leaf path has equal black height.",
    cpp: `struct RBTree {
    enum Color { RED, BLACK };
    struct Node {
        int key; Color color=RED;
        Node *left=nullptr, *right=nullptr, *parent=nullptr;
        Node(int key): key(key) {}
    };
    Node* root = nullptr;
    Color color(Node* node) { return node ? node->color : BLACK; }
    void rotateLeft(Node* x) {
        Node* y=x->right; x->right=y->left;
        if(y->left) y->left->parent=x;
        y->parent=x->parent;
        if(!x->parent) root=y;
        else if(x==x->parent->left) x->parent->left=y;
        else x->parent->right=y;
        y->left=x; x->parent=y;
    }
    void rotateRight(Node* x) {
        Node* y=x->left; x->left=y->right;
        if(y->right) y->right->parent=x;
        y->parent=x->parent;
        if(!x->parent) root=y;
        else if(x==x->parent->right) x->parent->right=y;
        else x->parent->left=y;
        y->right=x; x->parent=y;
    }
    void fix(Node* z) {
        while(z!=root && color(z->parent)==RED) {
            Node* p=z->parent; Node* g=p->parent;
            if(p==g->left) {
                Node* u=g->right;
                if(color(u)==RED) { p->color=u->color=BLACK; g->color=RED; z=g; }
                else { if(z==p->right){z=p;rotateLeft(z);p=z->parent;g=p->parent;} p->color=BLACK;g->color=RED;rotateRight(g); }
            } else {
                Node* u=g->left;
                if(color(u)==RED) { p->color=u->color=BLACK; g->color=RED; z=g; }
                else { if(z==p->left){z=p;rotateRight(z);p=z->parent;g=p->parent;} p->color=BLACK;g->color=RED;rotateLeft(g); }
            }
        }
        root->color=BLACK;
    }
    void insert(int key) {
        Node *parent=nullptr, *cur=root;
        while(cur){ parent=cur; if(key==cur->key)return; cur=key<cur->key?cur->left:cur->right; }
        Node* z=new Node(key); z->parent=parent;
        if(!parent) root=z; else if(key<parent->key) parent->left=z; else parent->right=z;
        fix(z);
    }
};`,
    python: `RED, BLACK = True, False
class Node:
    def __init__(self, key):
        self.key, self.red = key, RED
        self.left = self.right = self.parent = None

class RBTree:
    def __init__(self): self.root = None
    def color(self, node): return node.red if node else BLACK
    def rotate_left(self, x):
        y = x.right; x.right = y.left
        if y.left: y.left.parent = x
        y.parent = x.parent
        if not x.parent: self.root = y
        elif x is x.parent.left: x.parent.left = y
        else: x.parent.right = y
        y.left = x; x.parent = y
    def rotate_right(self, x):
        y = x.left; x.left = y.right
        if y.right: y.right.parent = x
        y.parent = x.parent
        if not x.parent: self.root = y
        elif x is x.parent.right: x.parent.right = y
        else: x.parent.left = y
        y.right = x; x.parent = y
    def insert(self, key):
        parent, cur = None, self.root
        while cur:
            parent = cur
            if key == cur.key: return
            cur = cur.left if key < cur.key else cur.right
        z = Node(key); z.parent = parent
        if not parent: self.root = z
        elif key < parent.key: parent.left = z
        else: parent.right = z
        self.fix(z)
    def fix(self, z):
        while z is not self.root and self.color(z.parent) == RED:
            p, g = z.parent, z.parent.parent
            if p is g.left:
                u = g.right
                if self.color(u) == RED:
                    p.red = u.red = BLACK; g.red = RED; z = g
                else:
                    if z is p.right: z = p; self.rotate_left(z); p, g = z.parent, z.parent.parent
                    p.red = BLACK; g.red = RED; self.rotate_right(g)
            else:
                u = g.left
                if self.color(u) == RED:
                    p.red = u.red = BLACK; g.red = RED; z = g
                else:
                    if z is p.left: z = p; self.rotate_right(z); p, g = z.parent, z.parent.parent
                    p.red = BLACK; g.red = RED; self.rotate_left(g)
        self.root.red = BLACK`
  }
];

const codeMaps = {
  "Frequency map / complement lookup": [
    "Create a map from a value already seen to its index.",
    "For each number, compute the complement that would complete the target.",
    "Return immediately when the complement exists; otherwise remember the current number."
  ],
  "Prefix sum / subarray count": [
    "Seed prefix sum 0 once so subarrays starting at index 0 are counted.",
    "Extend the running prefix sum with the current value.",
    "Count earlier prefixes equal to current sum − k, then record the current prefix."
  ],
  "Two pointers": [
    "Place one pointer at each end of the sorted search range.",
    "Compare the pair with the target on every iteration.",
    "Move only the side that can make the sum approach the target."
  ],
  "Sliding window": [
    "Expand the right edge and add the new element to the window state.",
    "Shrink from the left until the validity condition is restored.",
    "Measure the valid window after shrinking, when its invariant is true."
  ],
  "Monotonic stack": [
    "Store indices whose answer has not been found yet.",
    "Pop every smaller value solved by the current value.",
    "Push the current index so a future value can solve it."
  ],
  "Monotonic deque": [
    "Remove indices that have left the current window.",
    "Remove weaker values from the back because they can never become the maximum.",
    "The front now identifies the maximum for every complete window."
  ],
  "Linked-list reversal": [
    "Keep pointers to the reversed prefix and untouched suffix.",
    "Save the next node before changing the current link.",
    "Reverse one edge, advance both pointers, and return the new head."
  ],
  "Heap / top-k": [
    "Push each candidate into a min-heap.",
    "Remove the smallest item whenever more than k candidates are stored.",
    "The heap root is the kth largest after every item is processed."
  ],
  "Merge intervals": [
    "Sort intervals so possible overlaps become adjacent.",
    "Start a new output interval when there is a gap.",
    "Otherwise extend the last output interval to cover the overlap."
  ],
  "Binary search: first true": [
    "Maintain a half-open range that still contains the first valid answer.",
    "Test the midpoint without discarding a possible first true position.",
    "Stop when both bounds meet at the first true index."
  ],
  "Backtracking": [
    "Record the solution represented by the current choice path.",
    "Choose one candidate and recurse into the smaller decision space.",
    "Undo the choice so the next branch starts from the correct state."
  ],
  "Binary tree DFS": [
    "Return the identity value at an empty subtree.",
    "Recursively solve the complete left and right subtrees.",
    "Combine both results into the answer for the current subtree."
  ],
  "Graph DFS": [
    "Mark a node before exploring neighbors so cycles cannot revisit it.",
    "Traverse every unvisited outgoing neighbor.",
    "Start another traversal for each still-unvisited component when needed."
  ],
  "BFS shortest path": [
    "Initialize the queue with the source at distance zero.",
    "Pop nodes in increasing number of edges from the source.",
    "Assign each unseen neighbor once, then enqueue it for the next layer."
  ],
  "Topological sort": [
    "Count how many prerequisites enter each vertex.",
    "Queue every vertex whose prerequisites are already satisfied.",
    "Remove processed edges; a full output exists only when the graph is acyclic."
  ],
  "Dijkstra": [
    "Store the best known source distance and prioritize the smallest one.",
    "Skip heap entries made stale by a later, shorter route.",
    "Relax each outgoing edge and enqueue only genuine improvements."
  ],
  "Union-find (DSU)": [
    "Represent each connected component by a root parent.",
    "Compress paths during find so future root queries become almost constant time.",
    "Attach the smaller component below the larger root during union."
  ],
  "Top-down DP": [
    "Define a recursive state that contains all information needed by future choices.",
    "Return immediately when that state was solved before.",
    "Try valid transitions, save the best result, and reuse it later."
  ],
  "0/1 knapsack": [
    "Let dp[c] store the best value attainable with capacity c.",
    "Process capacities backward so one item cannot be selected twice.",
    "Compare skipping the item with taking it from the previous capacity state."
  ],
  "Bitmask subsets": [
    "Use one integer bitmask to represent a complete subset.",
    "Check each bit to decide whether its corresponding item is included.",
    "Process the constructed subset, then advance to the next mask."
  ],
  "Kruskal minimum spanning tree": [
    "Sort every edge from cheapest to most expensive.",
    "Use DSU to reject edges whose endpoints are already connected.",
    "Add accepted edge costs until the spanning tree has V−1 edges."
  ],
  "Dinic maximum flow": [
    "Store forward and reverse residual edges for every capacity.",
    "BFS builds levels containing only routes that can still reach the sink efficiently.",
    "DFS sends blocking flow, updates both residual directions, and repeats by level graph."
  ],
  "Bellman–Ford shortest paths": [
    "Initialize only the source distance; all other vertices begin unreachable.",
    "Relax every edge repeatedly so paths gain at most one edge per pass.",
    "A relaxation after V−1 passes proves a reachable negative cycle."
  ],
  "Floyd–Warshall all-pairs paths": [
    "Initialize the matrix with direct edges, zero diagonals, and infinity elsewhere.",
    "Introduce each vertex k as a newly allowed intermediate point.",
    "Improve i→j whenever traveling through k is shorter."
  ],
  "Kosaraju strongly connected components": [
    "First DFS records vertices after all descendants finish.",
    "Reverse every graph edge.",
    "Traverse in reverse finish order; each new DFS marks exactly one SCC."
  ],
  "Bridges in an undirected graph": [
    "Assign discovery and low-link times when DFS first visits a vertex.",
    "Propagate the earliest reachable discovery time back from each child.",
    "Mark parent–child edge as a bridge when the child cannot reach the parent or earlier."
  ],
  "A* heuristic search": [
    "Prioritize states by known route cost plus estimated remaining cost.",
    "Discard a queued state when a cheaper route to it was already found.",
    "Relax neighbors like Dijkstra while the heuristic guides expansion toward the goal."
  ],
  "Lowest common ancestor: binary lifting": [
    "DFS records depth and every power-of-two ancestor for each vertex.",
    "Lift the deeper query vertex until both depths match.",
    "Lift both vertices together from large jumps to small until their parents match."
  ],
  "KD-tree nearest neighbor (2D)": [
    "Build a balanced tree by alternating the coordinate used to split points.",
    "Search the side of the split containing the query before the other side.",
    "Visit the far side only when its splitting plane can beat the current best distance."
  ],
  "Red-black tree insertion": [
    "Insert the key as a red node using ordinary binary-search-tree ordering.",
    "While a red parent violates the rules, inspect the uncle to choose recoloring or rotation.",
    "Rotate and recolor locally, then force the root black to restore every invariant."
  ]
};

templates.forEach(item => { item.codeMap = codeMaps[item.title]; });

const catalogMode = document.body.dataset.catalog || "all";
const catalogTemplates = templates.filter(item => {
  if (catalogMode === "basic") return item.level !== "Advanced";
  if (catalogMode === "advanced") return item.level === "Advanced";
  return true;
});
const categories = ["All", ...new Set(catalogTemplates.map(item => item.category))];
const grid = document.querySelector("#template-grid");
const search = document.querySelector("#search");
const filters = document.querySelector("#filters");
const resultCount = document.querySelector("#result-count");
const emptyState = document.querySelector("#empty-state");
let activeCategory = "All";

const decisionPaths = {
  sequence: {
    question: "Is the answer contiguous, sorted, or a repeated range aggregate?",
    algorithms: ["Sliding window", "Two pointers", "Prefix sum", "Binary search: first true", "Monotonic stack"]
  },
  graph: {
    question: "Is the goal traversal, minimum steps, a nonnegative shortest path, dependency order, or connectivity?",
    algorithms: ["Graph DFS", "BFS shortest path", "Dijkstra", "Topological sort", "Union-find"]
  },
  tree: {
    question: "Do you need subtree information, a root-to-leaf property, or ordered lookup?",
    algorithms: ["Binary tree DFS", "Graph DFS", "Binary search: first true"]
  },
  state: {
    question: "Do choices repeat subproblems, require enumeration, or admit a provable local choice?",
    algorithms: ["Top-down DP", "0/1 knapsack", "Backtracking", "Bitmask subsets"]
  },
  interval: {
    question: "Are ranges overlapping, events ordered, or repeated endpoints controlling the answer?",
    algorithms: ["Merge intervals", "Heap / top-k", "Binary search: first true", "Prefix sum / subarray count"]
  }
};

function escapeHtml(value) {
  return value.replace(/[&<>'"]/g, character => ({
    "&": "&amp;", "<": "&lt;", ">": "&gt;", "'": "&#39;", '"': "&quot;"
  })[character]);
}

document.querySelector("#template-count").textContent = catalogTemplates.length;

function makeFilter(category) {
  const button = document.createElement("button");
  button.type = "button";
  button.className = `filter-button${category === activeCategory ? " active" : ""}`;
  button.textContent = category;
  button.setAttribute("aria-pressed", String(category === activeCategory));
  button.addEventListener("click", () => { setCategory(category); render(); });
  return button;
}

categories.forEach(category => filters.append(makeFilter(category)));

function setCategory(category) {
  activeCategory = category;
  [...filters.children].forEach(item => {
    const active = item.textContent === category;
    item.classList.toggle("active", active);
    item.setAttribute("aria-pressed", String(active));
  });
}

function cardFor(item) {
  const article = document.createElement("article");
  article.className = "template-card";
  article.classList.toggle("is-advanced", item.level === "Advanced");
  const safe = Object.fromEntries(
    ["title", "category", "complexity", "when", "invariant"].map(key => [key, escapeHtml(item[key])])
  );
  const levelBadge = item.level === "Advanced" ? '<span class="advanced-badge">Advanced</span>' : "";
  const codeMap = item.codeMap.map((step, index) => `<li><span>${String(index + 1).padStart(2, "0")}</span>${escapeHtml(step)}</li>`).join("");
  article.innerHTML = `
    <div class="card-head">
      <div class="card-meta"><span class="card-topic"><span>${safe.category}</span>${levelBadge}</span><span class="complex">${safe.complexity}</span></div>
      <h3>${safe.title}</h3>
      <p>${safe.when}</p>
    </div>
    <div class="code-map">
      <strong>Code map</strong>
      <ol>${codeMap}</ol>
    </div>
    <p class="invariant"><b>Invariant:</b> ${safe.invariant}</p>
    <div class="code-shell">
      <div class="code-tabs" role="group" aria-label="Choose code language">
        <button class="code-tab active" type="button" aria-pressed="true">C++17</button>
        <button class="code-tab" type="button" aria-pressed="false">Python 3</button>
        <button class="copy-button" type="button" aria-label="Copy ${safe.title} C++17 template">Copy</button>
      </div>
      <pre tabindex="0"><code></code></pre>
      <span class="copy-status sr-only" role="status" aria-live="polite"></span>
    </div>`;
  const code = article.querySelector("code");
  const tabs = article.querySelectorAll(".code-tab");
  const copy = article.querySelector(".copy-button");
  const copyStatus = article.querySelector(".copy-status");
  let language = "cpp";

  const showCode = () => {
    code.textContent = item[language];
    const languageName = language === "cpp" ? "C++17" : "Python 3";
    copy.setAttribute("aria-label", `Copy ${item.title} ${languageName} template`);
  };
  tabs.forEach((tab, index) => tab.addEventListener("click", () => {
    language = index === 0 ? "cpp" : "python";
    tabs.forEach((other, otherIndex) => {
      const active = otherIndex === index;
      other.classList.toggle("active", active);
      other.setAttribute("aria-pressed", String(active));
    });
    showCode();
  }));
  copy.addEventListener("click", async () => {
    const languageName = language === "cpp" ? "C++17" : "Python 3";
    try {
      await navigator.clipboard.writeText(item[language]);
      copy.textContent = "Copied";
      copyStatus.textContent = `${item.title} ${languageName} template copied.`;
    } catch {
      const range = document.createRange();
      range.selectNodeContents(code);
      const selection = window.getSelection();
      selection.removeAllRanges();
      selection.addRange(range);
      copy.textContent = "Selected";
      copyStatus.textContent = `${item.title} ${languageName} template selected for manual copying.`;
    }
    window.setTimeout(() => { copy.textContent = "Copy"; }, 1400);
  });
  showCode();
  return article;
}

function render() {
  const query = search.value.trim().toLowerCase();
  const visible = catalogTemplates.filter(item => {
    const categoryMatch = activeCategory === "All" || item.category === activeCategory;
    const haystack = `${item.title} ${item.category} ${item.when} ${item.invariant} ${item.complexity} ${item.cpp} ${item.python}`.toLowerCase();
    return categoryMatch && haystack.includes(query);
  });
  grid.replaceChildren(...visible.map(cardFor));
  resultCount.textContent = `${visible.length} of ${catalogTemplates.length} templates`;
  emptyState.hidden = visible.length !== 0;
}

search.addEventListener("input", render);

const decisionQuestion = document.querySelector("#decision-question");
const decisionAlgorithms = document.querySelector("#decision-algorithms");
const decisionNodes = document.querySelectorAll(".decision-node");

function renderDecision(key) {
  const path = decisionPaths[key];
  decisionQuestion.textContent = path.question;
  decisionAlgorithms.replaceChildren(...path.algorithms.map(name => {
    const button = document.createElement("button");
    button.type = "button";
    button.className = "algorithm-chip";
    button.textContent = name;
    button.addEventListener("click", () => {
      search.value = name;
      setCategory("All");
      render();
      document.querySelector("#templates").scrollIntoView({ behavior: "smooth" });
    });
    return button;
  }));
}

if (decisionNodes.length) {
  decisionNodes.forEach(node => node.addEventListener("click", () => {
    decisionNodes.forEach(other => {
      const active = other === node;
      other.classList.toggle("active", active);
      other.setAttribute("aria-pressed", String(active));
    });
    renderDecision(node.dataset.decision);
  }));
  renderDecision("sequence");
}

const root = document.documentElement;
const themeButton = document.querySelector("#theme-toggle");
let savedTheme = null;
try {
  savedTheme = localStorage.getItem("dsa-theme");
} catch {
  // Storage can be unavailable in privacy-restricted browser contexts.
}
const preferredTheme = savedTheme || (window.matchMedia("(prefers-color-scheme: light)").matches ? "light" : "dark");
root.dataset.theme = preferredTheme;

function updateThemeControl() {
  const nextTheme = root.dataset.theme === "light" ? "dark" : "light";
  themeButton.setAttribute("aria-label", `Switch to ${nextTheme} theme`);
  themeButton.setAttribute("title", `Switch to ${nextTheme} theme`);
  document.querySelector('meta[name="theme-color"]')
    .setAttribute("content", root.dataset.theme === "light" ? "#f5f8fc" : "#07111f");
}

updateThemeControl();
themeButton.addEventListener("click", () => {
  const next = root.dataset.theme === "light" ? "dark" : "light";
  root.dataset.theme = next;
  try {
    localStorage.setItem("dsa-theme", next);
  } catch {
    // Theme switching still works for this page view without persistence.
  }
  updateThemeControl();
});

render();
