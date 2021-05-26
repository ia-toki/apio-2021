#include "jumps.h"

#include <bits/stdc++.h>

std::vector<std::vector<int>> lft, rgt, up;

void init(int N, std::vector<int> H) {
  std::vector<int> stack; lft.clear();
  lft.push_back(std::vector<int>(N, -1));
  for (int i = 0; i < N; ++i) {
    while (!stack.empty() && H[stack.back()] < H[i]) stack.pop_back();
    if (!stack.empty()) lft[0][i] = stack.back();
    stack.push_back(i);
  }

  stack.clear(); rgt.clear();
  rgt.push_back(std::vector<int>(N, -1));
  for (int i = N - 1; i >= 0; --i) {
    while (!stack.empty() && H[i] > H[stack.back()]) stack.pop_back();
    if (!stack.empty()) rgt[0][i] = stack.back();
    stack.push_back(i);
  }

  up.clear(); up.push_back(std::vector<int>(N, -1));
  for (int i = 0; i < N; ++i) {
    if (lft[0][i] == -1 && rgt[0][i] == -1) continue;
    if (lft[0][i] == -1) {
      up[0][i] = rgt[0][i];
    } else if (rgt[0][i] == -1) {
      up[0][i] = lft[0][i];
    } else {
      up[0][i] = H[lft[0][i]] < H[rgt[0][i]] ? rgt[0][i] : lft[0][i];
    }
  }

  auto buildTable = [](std::vector<std::vector<int>> &table) {
    for (int i = 1; 1 << i < static_cast<int>(table[0].size()); ++i) {
      table.push_back(std::vector<int>(table[0].size(), -1));
      for (int j = 0; j < static_cast<int>(table[0].size()); ++j) {
        table[i][j] = table[i - 1][j] != -1 ? table[i - 1][table[i - 1][j]] :
                                              -1;
      }
    }
  };

  buildTable(lft); buildTable(rgt); buildTable(up);
}

int minimum_jumps(int A, int B, int C, int D) {
  auto jumpWhile = [](int &x, std::vector<std::vector<int>> &table,
                      std::function<bool(int)> condition) {
    int jump = 0;
    for (int i = static_cast<int>(table.size()) - 1; i >= 0; --i) {
      if (table[i][x] != -1 && condition(table[i][x])) {
        jump += 1 << i;
        x = table[i][x];
      }
    }
    return jump;
  };

  jumpWhile(B, lft, [&](int x) {
    return x >= A && rgt[0][x] != -1 && rgt[0][x] <= D;
  });

  int ans = jumpWhile(B, up, [&](int x) {
    return rgt[0][x] != -1 && rgt[0][x] < C;
  });
  if (rgt[0][B] != -1 && rgt[0][B] < C &&
      rgt[0][up[0][B]] != -1 && rgt[0][up[0][B]] <= D) {
    ++ans;
    B = up[0][B];
  }

  ans += jumpWhile(B, rgt, [&](int x) {
    return x < C;
  });

  return rgt[0][B] == -1 || rgt[0][B] > D ? -1 : ans + 1;
}
