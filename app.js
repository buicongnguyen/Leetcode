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
  }
];

const categories = ["All", ...new Set(templates.map(item => item.category))];
const grid = document.querySelector("#template-grid");
const search = document.querySelector("#search");
const filters = document.querySelector("#filters");
const resultCount = document.querySelector("#result-count");
const emptyState = document.querySelector("#empty-state");
let activeCategory = "All";

document.querySelector("#template-count").textContent = templates.length;

function makeFilter(category) {
  const button = document.createElement("button");
  button.type = "button";
  button.className = `filter-button${category === activeCategory ? " active" : ""}`;
  button.textContent = category;
  button.setAttribute("aria-pressed", String(category === activeCategory));
  button.addEventListener("click", () => {
    activeCategory = category;
    [...filters.children].forEach(item => {
      const active = item.textContent === category;
      item.classList.toggle("active", active);
      item.setAttribute("aria-pressed", String(active));
    });
    render();
  });
  return button;
}

categories.forEach(category => filters.append(makeFilter(category)));

function cardFor(item) {
  const article = document.createElement("article");
  article.className = "template-card";
  article.innerHTML = `
    <div class="card-head">
      <div class="card-meta"><span>${item.category}</span><span class="complex">${item.complexity}</span></div>
      <h3>${item.title}</h3>
      <p>${item.when}</p>
    </div>
    <p class="invariant"><b>Invariant:</b> ${item.invariant}</p>
    <div class="code-shell">
      <div class="code-tabs" role="tablist" aria-label="Choose code language">
        <button class="code-tab active" type="button" role="tab" aria-selected="true">C++17</button>
        <button class="code-tab" type="button" role="tab" aria-selected="false">Python 3</button>
        <button class="copy-button" type="button">Copy</button>
      </div>
      <pre tabindex="0"><code></code></pre>
    </div>`;
  const code = article.querySelector("code");
  const tabs = article.querySelectorAll(".code-tab");
  const copy = article.querySelector(".copy-button");
  let language = "cpp";

  const showCode = () => { code.textContent = item[language]; };
  tabs.forEach((tab, index) => tab.addEventListener("click", () => {
    language = index === 0 ? "cpp" : "python";
    tabs.forEach((other, otherIndex) => {
      const active = otherIndex === index;
      other.classList.toggle("active", active);
      other.setAttribute("aria-selected", String(active));
    });
    showCode();
  }));
  copy.addEventListener("click", async () => {
    try {
      await navigator.clipboard.writeText(item[language]);
      copy.textContent = "Copied";
    } catch {
      const range = document.createRange();
      range.selectNodeContents(code);
      const selection = window.getSelection();
      selection.removeAllRanges();
      selection.addRange(range);
      copy.textContent = "Selected";
    }
    window.setTimeout(() => { copy.textContent = "Copy"; }, 1400);
  });
  showCode();
  return article;
}

function render() {
  const query = search.value.trim().toLowerCase();
  const visible = templates.filter(item => {
    const categoryMatch = activeCategory === "All" || item.category === activeCategory;
    const haystack = `${item.title} ${item.category} ${item.when} ${item.invariant} ${item.complexity}`.toLowerCase();
    return categoryMatch && haystack.includes(query);
  });
  grid.replaceChildren(...visible.map(cardFor));
  resultCount.textContent = `${visible.length} of ${templates.length} templates`;
  emptyState.hidden = visible.length !== 0;
}

search.addEventListener("input", render);

const root = document.documentElement;
const themeButton = document.querySelector("#theme-toggle");
const savedTheme = localStorage.getItem("dsa-theme");
if (savedTheme) root.dataset.theme = savedTheme;
themeButton.addEventListener("click", () => {
  const next = root.dataset.theme === "light" ? "dark" : "light";
  root.dataset.theme = next;
  localStorage.setItem("dsa-theme", next);
  themeButton.setAttribute("aria-label", `Switch to ${next === "dark" ? "light" : "dark"} theme`);
});

render();
