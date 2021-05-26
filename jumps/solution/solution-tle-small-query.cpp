#include "jumps.h"

#include <bits/stdc++.h>
using namespace std;

int N;
vector<vector<int>> adj;

void init(int _N, std::vector<int> H) {
  N = _N;
  adj = vector<vector<int>>(N, vector<int>());

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
}

int minimum_jumps(int A, int B, int C, int D) {
  vector<int> dist(N, INT_MAX);
  queue<int> q;
  for (int i = A; i <= B; ++i) {
    dist[i] = 0;
    q.push(i);
  }
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    if (C <= u && u <= D) {
      return dist[u];
    }
    for (int v : adj[u]) {
      if (dist[v] > dist[u] + 1) {
        dist[v] = dist[u] + 1;
        q.push(v);
      }
    }
  }
  return -1;
}
