#include "jumps.h"

#include <bits/stdc++.h>
using namespace std;

class MinSegmentTree {
 private:
  int N;
  vector<int> tree;

  void build(const vector<int> &val, int ix, int L, int R) {
    if (L == R) {
      tree[ix] = val[L];
      return;
    }
    int M = (L + R) >> 1;
    build(val, ix * 2 + 1, L, M);
    build(val, ix * 2 + 2, M + 1, R);
    tree[ix] = min(tree[ix * 2 + 1], tree[ix * 2 + 2]);
  }

  int query(int ix, int L, int R, int x, int y) {
    if (x <= L && R <= y) {
      return tree[ix];
    }
    if (R < x || y < L) {
      return INT_MAX;
    }
    int M = (L + R) >> 1;
    return min(query(ix * 2 + 1, L, M, x, y),
               query(ix * 2 + 2, M + 1, R, x, y));
  }

 public:
  void build(const vector<int> &val) {
    N = val.size();
    tree.resize(4 * N);
    build(val, 0, 0, N - 1);
  }

  int query(int x, int y) {
    return query(0, 0, N - 1, x, y);
  }
};

int N;
vector<MinSegmentTree> seg_trees;

void init(int _N, std::vector<int> H) {
  N = _N;
  seg_trees = vector<MinSegmentTree>(N);

  vector<vector<int>> adj = vector<vector<int>>(N, vector<int>());
  for (int i = 0; i < N; ++i) {
    for (int j = i - 1; j >= 0; --j) {
      if (H[j] > H[i]) {
        adj[i].push_back(j);
        break;
      }
    }
    for (int j = i + 1; j < N; ++j) {
      if (H[j] > H[i]) {
        adj[i].push_back(j);
        break;
      }
    }
  }
  for (int i = 0; i < N; ++i) {
    vector<int> dist(N, INT_MAX);
    queue<int> q;

    dist[i] = 0;
    q.push(i);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int v : adj[u]) {
        if (dist[v] > dist[u] + 1) {
          dist[v] = dist[u] + 1;
          q.push(v);
        }
      }
    }

    seg_trees[i].build(dist);
  }
}

int minimum_jumps(int A, int B, int C, int D) {
  int jumps = INT_MAX;
  for (int s = A; s <= B; ++s) {
    jumps = min(jumps, seg_trees[s].query(C, D));
  }
  return jumps == INT_MAX ? -1 : jumps;
}
