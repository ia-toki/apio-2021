#include "roads.h"

#include <bits/stdc++.h>

const long long INF = 1e18;

struct Edge {
  int v, w;
  Edge(int _v, int _w): v(_v), w(_w) {}
};

class Fenwick {
  int size, n, logn;
  std::vector<int> counts;
  std::vector<long long> sums;
  std::vector<int> weights;

 public:
  void build(const std::vector<Edge> &edges) {
    size = 0;
    weights.resize(edges.size());
    for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
      weights[i] = edges[i].w;
    }
    std::sort(weights.begin(), weights.end());
    weights.erase(std::unique(weights.begin(), weights.end()), weights.end());

    n = weights.size();
    sums.assign(n, 0LL);
    counts.assign(n, 0);
    for (logn = 0; 1 << logn <= n; ++logn) {}
  }

  void add(int w) {
    int idx = lower_bound(weights.begin(), weights.end(), w) - weights.begin();
    assert(idx < static_cast<int>(weights.size()) && weights[idx] == w);
    ++size;
    ++counts[idx]; sums[idx] += w;
  }

  long long sumKMin(int k) {
    if (size < k) return INF;
    int cnt = 0;
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
      if (cnt + counts[i] > k) {
        return sum + 1LL * std::max(0, k - cnt) * weights[i];
      }
      cnt += counts[i];
      sum += sums[i];
    }
    return sum;
  }
};

std::vector<long long> minimum_closure_costs(int N,
    std::vector<int> U, std::vector<int> V, std::vector<int> W) {
  std::vector<std::vector<Edge>> adj(N);

  for (int i = 0; i < N - 1; ++i) {
    adj[U[i]].emplace_back(V[i], W[i]);
    adj[V[i]].emplace_back(U[i], W[i]);
  }

  std::vector<int> degrees(N);
  for (int i = 0; i < N; ++i) degrees[i] = adj[i].size();

  std::vector<int> nodes(N);
  std::vector<Fenwick> fenwicks(N);
  for (int u = 0; u < N; ++u) {
    nodes[u] = u;
    std::sort(adj[u].begin(), adj[u].end(), [&](Edge e, Edge f) {
      return degrees[e.v] > degrees[f.v];
    });
    fenwicks[u].build(adj[u]);
  }
  std::sort(nodes.begin(), nodes.end(), [&](int u, int v) {
    return degrees[u] > degrees[v];
  });

  std::vector<long long> ans(N);
  std::vector<std::vector<long long>> dp(N, std::vector<long long>(2));
  ans[0] = std::accumulate(W.begin(), W.end(), 0LL);
  for (int deg = 1; deg < N; ++deg) {
    while (!nodes.empty() && degrees[nodes.back()] <= deg) {
      for (auto [v, w] : adj[nodes.back()]) {
        fenwicks[v].add(w);
        while (!adj[v].empty() && degrees[adj[v].back().v] <= deg) {
          adj[v].pop_back();
        }
      }
      nodes.pop_back();
    }

    for (int u : nodes) {
      dp[u][0] = dp[u][1] = -1;
    }

    std::function<long long(int, int, int)> dfs = [&](int u, int p, int st) {
      if (~dp[u][st]) return dp[u][st];
      std::vector<int> deltas;
      long long total = 0;
      for (auto [v, w] : adj[u]) {
        if (v == p) continue;

        total += dfs(v, u, 1);
        deltas.push_back(dfs(v, u, 0) - dfs(v, u, 1) + w);
      }

      int remove = degrees[u] - deg - (u != p && st == 0);
      long long ret = total + fenwicks[u].sumKMin(remove);
      std::sort(deltas.begin(), deltas.end());
      for (int i = 0; i < static_cast<int>(deltas.size()); ++i) {
        total += deltas[i];
        ret = std::min(ret, total + fenwicks[u].sumKMin(remove - i - 1));
      }

      return dp[u][st] = ret;
    };

    for (int u : nodes) {
      if (dp[u][0] == -1) ans[deg] += dfs(u, u, 0);
    }
  }

  return ans;
}
