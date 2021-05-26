#include "jumps.h"

#include <bits/stdc++.h>
using namespace std;

int N;
vector<vector<int>> dist;

void init(int _N, std::vector<int> H) {
  N = _N;
  dist = vector<vector<int>>(N, vector<int>(N, N));
  for (int i = 0; i < N; ++i) {
    for (int j = i - 1; j >= 0; --j) {
      if (H[j] > H[i]) {
        dist[i][j] = 1;
        break;
      }
    }
    for (int j = i + 1; j < N; ++j) {
      if (H[j] > H[i]) {
        dist[i][j] = 1;
        break;
      }
    }
  }
  for (int k = 0; k < N; ++k) {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }
  }
}

int minimum_jumps(int A, int B, int C, int D) {
  int jumps = N;
  for (int st = A; st <= B; ++st) {
    for (int nd = C; nd <= D; ++nd) {
      jumps = min(jumps, dist[st][nd]);
    }
  }
  return jumps == N ? -1 : jumps;
}
