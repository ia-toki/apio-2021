// WA solution: This solution may start at a point to the left of A.

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

int minimum_jumps(int A, int B, int C, int D) {
  ++A; ++B; ++C; ++D;

  if (B == C - 1) {
    return R[0][B] <= D ? 1 : -1;
  }
  int middle_tallest_index = maximum_height_index(B + 1, C - 1);
  if (H[B] > H[middle_tallest_index]) {
    return R[0][B] <= D ? 1 : -1;
  }
  int s = B;
  for (int j = kMaxLogN - 1; j >= 0; --j) {
    if (H[L[j][s]] < H[middle_tallest_index]) {
      s = L[j][s];
    }
  }
  int jumps = 0;
  if (A <= L[0][s]) {
    if (R[0][L[0][s]] <= D) {
      return 1;
    }
  } else {
    for (int j = kMaxLogN - 1; j >= 0; --j) {
      if (H[higher[j][s]] <= H[middle_tallest_index]) {
        jumps |= (1 << j);
        s = higher[j][s];
      }
    }
    if (s == middle_tallest_index) {
      return R[0][s] <= D ? jumps + 1 : -1;
    }
    if (0 < L[0][s] && R[0][L[0][s]] <= D) {
      return jumps + 2;
    }
  }
  for (int j = kMaxLogN - 1; j >= 0; --j) {
    if (R[j][s] < C) {
      jumps += (1 << j);
      s = R[j][s];
    }
  }
  return C <= R[0][s] && R[0][s] <= D ? jumps + 1 : -1;
}
