#include "jumps.h"

#include <bits/stdc++.h>
using namespace std;

int N;
vector<vector<int>> adj;

void init(int _N, std::vector<int> H) {
  N = _N;
  adj = vector<vector<int>>(N, vector<int>());

  stack<int> decreasing_height;
  for (int i = 0; i < N; ++i) {
    while (!decreasing_height.empty() && H[decreasing_height.top()] < H[i]) {
      decreasing_height.pop();
    }
    if (!decreasing_height.empty()) {
      adj[i].push_back(decreasing_height.top());
    }
    decreasing_height.push(i);
  }

  while (!decreasing_height.empty()) {
    decreasing_height.pop();
  }
  for (int i = N - 1; i >= 0; --i) {
    while (!decreasing_height.empty() && H[decreasing_height.top()] < H[i]) {
      decreasing_height.pop();
    }
    if (!decreasing_height.empty()) {
      adj[i].push_back(decreasing_height.top());
    }
    decreasing_height.push(i);
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
