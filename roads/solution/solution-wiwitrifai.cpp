#include "roads.h"
#include <bits/stdc++.h>

std::vector<std::priority_queue<long long>> used_edges, free_edges;
std::vector<std::vector<long long>> neighbouring_edges;
std::vector<int> last_limit, degree;
std::vector<long long> full_dp, partial_dp, used_sum;
std::vector<std::set<std::pair<int, int>>> tree;

long long get(int v, int limit) {
  if (limit <= 0)
    return 0;
  auto & used_pq = used_edges[v];
  auto & free_pq = free_edges[v];
  auto & sum = used_sum[v];
  while ((int)used_pq.size() > limit) {
    int w = -used_pq.top();
    used_pq.pop();
    sum -= w;
    free_pq.push(w);
  }
  while ((int)used_pq.size() < limit && free_pq.size() > 0) {
    int w = free_pq.top();
    free_pq.pop();
    sum += w;
    used_pq.push(-w);
  }
  while (used_pq.size() > 0 && free_pq.size() > 0 &&
      -used_pq.top() < free_pq.top()) {
    sum += free_pq.top() + used_pq.top();
    used_pq.push(-free_pq.top());
    free_pq.push(-used_pq.top());
    used_pq.pop();
    free_pq.pop();
  }
  int remaining_slots = limit - used_pq.size();
  long long ans = sum;
  for (long long x : neighbouring_edges[v]) {
    if (x < 0) break;
    if (remaining_slots > 0) {
      ans += x;
      --remaining_slots;
      continue;
    }
    if (used_pq.size() > 0 && -used_pq.top() < x) {
      ans += x + used_pq.top();
      sum += used_pq.top();
      free_pq.push(-used_pq.top());
      used_pq.pop();
    }
  }
  return ans;
}

void dfs(int v, int par, int limit) {
  assert(degree[v] > limit);
  last_limit[v] = limit;
  full_dp[v] = partial_dp[v] = 0;
  neighbouring_edges[v].clear();
  for (auto & e : tree[v]) {
    int u, w;
    std::tie(u, w) = e;
    if (u == par) continue;
    dfs(u, v, limit);
    partial_dp[v] += full_dp[u];
    neighbouring_edges[v].push_back(w + partial_dp[u] - full_dp[u]);
  }
  sort(neighbouring_edges[v].rbegin(), neighbouring_edges[v].rend());
  long long cursum = partial_dp[v];
  partial_dp[v] = cursum + get(v, limit-1);
  full_dp[v] = cursum + get(v, limit);
}

std::vector<long long> minimum_closure_costs(int N, std::vector<int> U,
                                             std::vector<int> V,
                                             std::vector<int> W) {
  free_edges.resize(N);
  used_edges.resize(N);
  neighbouring_edges.resize(N);
  last_limit.resize(N, 0);
  long long total = std::accumulate(W.begin(), W.end(), 0LL);
  full_dp.resize(N);
  partial_dp.resize(N);
  used_sum.resize(N, 0);
  tree.resize(N);
  for (int i = 0; i < N-1; ++i) {
    tree[U[i]].insert({V[i], W[i]});
    tree[V[i]].insert({U[i], W[i]});
  }
  degree.resize(N);
  for (int i = 0; i < N; ++i)
    degree[i] = tree[i].size();
  std::vector<int> nodes(N);
  std::iota(nodes.begin(), nodes.end(), 0);
  std::sort(nodes.begin(), nodes.end(), [&](int lef, int rig) {
    return degree[lef] > degree[rig];
  });
  int last = N-1;
  long long totally_free = 0;
  std::vector<long long> ans(N, total);
  for (int k = 1; k < N; ++k) {
    while (last >= 0 && degree[nodes[last]] <= k) {
      int v = nodes[last];
      for (auto & e : tree[v]) {
        int u, w;
        std::tie(u, w) = e;
        assert(tree[u].count({v, w}));
        tree[u].erase({v, w});
        free_edges[u].push(w);
      }
      while (!free_edges[v].empty()) {
        totally_free += free_edges[v].top();
        free_edges[v].pop();
      }
      totally_free += used_sum[v];
      tree[v].clear();
      --last;
    }
    ans[k] -= totally_free;
    for (int v : nodes) {
      if (degree[v] <= k) break;
      if (last_limit[v] >= k) continue;
      dfs(v, v, k);
      ans[k] -= full_dp[v];
    }
  }
  return ans;
}
