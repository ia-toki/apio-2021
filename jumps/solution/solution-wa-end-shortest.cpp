#include "jumps.h"

#include <bits/stdc++.h>
using namespace std;

constexpr int kMaxLogN = 20;

int N;
vector<int> H;
vector<vector<int>> L, R;
vector<vector<int>> higher;

void init(int _N, std::vector<int> _H) {
  N = _N;
  H = _H;

  H.insert(H.begin(), INT_MAX);
  H.insert(H.end(), INT_MAX);
  N += 2;
  L = R = higher = vector<vector<int>>(kMaxLogN, vector<int>(N));

  stack<int> decreasing_height;
  for (int i = 0; i < N; ++i) {
    while (!decreasing_height.empty() && H[decreasing_height.top()] <= H[i]) {
      decreasing_height.pop();
    }
    L[0][i] = decreasing_height.empty() ? i : decreasing_height.top();
    decreasing_height.push(i);
  }

  while (!decreasing_height.empty()) {
    decreasing_height.pop();
  }
  for (int i = N - 1; i >= 0; --i) {
    while (!decreasing_height.empty() && H[decreasing_height.top()] <= H[i]) {
      decreasing_height.pop();
    }
    R[0][i] = decreasing_height.empty() ? i : decreasing_height.top();
    decreasing_height.push(i);
  }

  for (int i = 0; i < N; ++i) {
    higher[0][i] = H[L[0][i]] > H[R[0][i]] ? L[0][i] : R[0][i];
  }

  for (int j = 1; j < kMaxLogN; ++j) {
    for (int i = 0; i < N; ++i) {
      L[j][i] = L[j - 1][L[j - 1][i]];
      R[j][i] = R[j - 1][R[j - 1][i]];
      higher[j][i] = higher[j - 1][higher[j - 1][i]];
    }
  }
}

int maximum_height_index(int l, int r) {
  for (int j = kMaxLogN - 1; j >= 0; --j) {
    if (R[j][l] <= r) {
      l = R[j][l];
    }
  }
  return l;
}

int minimum_jumps(int s, int e) {
  int jumps = 0;
  for (int j = kMaxLogN - 1; j >= 0; --j) {
    if (H[higher[j][s]] <= H[e]) {
      s = higher[j][s];
      jumps |= (1 << j);
    }
  }
  if (H[L[0][s]] > H[e]) {
    for (int j = kMaxLogN - 1; j >= 0; --j) {
      if (H[R[j][s]] <= H[e]) {
        s = R[j][s];
        jumps += (1 << j);
      }
    }
  } else {
    for (int j = kMaxLogN - 1; j >= 0; --j) {
      if (H[L[j][s]] <= H[e]) {
        s = L[j][s];
        jumps += (1 << j);
      }
    }
  }
  return s == e ? jumps : -1;
}

int minimum_jumps(int A, int B, int C, int D) {
  ++A; ++B; ++C; ++D;

  int s = B;
  int e = C;
  if (B + 1 < C) {
    int middle_tallest_index = maximum_height_index(B + 1, C - 1);
    if (H[e] < H[middle_tallest_index]) {
      for (int j = kMaxLogN - 1; j >= 0; --j) {
        if (H[R[j][e]] < H[middle_tallest_index]) {
          e = R[j][e];
        }
      }
      e = R[0][e];
    }
  }
  if (e > D) {
    return -1;
  }
  int jumps = INT_MAX;
  while (s >= A) {
    if (H[e] > H[s]) {
      jumps = min(jumps, minimum_jumps(s, e));
    } else {
      for (int j = kMaxLogN - 1; j >= 0; --j) {
        if (H[R[j][e]] < H[s]) {
          e = R[j][e];
        }
      }
      e = R[0][e];

      if (e > D) {
        break;
      }
      jumps = min(jumps, minimum_jumps(s, e));
    }
    s = L[0][s];
  }
  return jumps == INT_MAX ? -1 : jumps;
}
