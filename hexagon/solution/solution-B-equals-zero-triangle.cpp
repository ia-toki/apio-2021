#include "hexagon.h"
#include <bits/stdc++.h>

using namespace std;

constexpr int mod = 1e9 + 7;

int draw_territory(int /*N*/, int A, int /*B*/, vector<int> /*D*/,
                   vector<int> L) {
  int len = L[0] + 1;
  long long ans = 1LL * len * (len + 1) / 2;
  ans = (ans % mod) * A % mod;
  return ans;
}
