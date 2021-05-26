#include "hexagon.h"
#include <bits/stdc++.h>

using namespace std;

constexpr int mod = 1e9 + 7;

int draw_territory(int /*N*/, int A, int B, vector<int> /*D*/, vector<int> L) {
  int len = L[0] + 1;
  long long arith = 1LL * len * (len + 1) / 2;
  long long ans = (arith % mod) * (A - B) % mod;
  long long temp = 2LL * len + 1;
  if (arith % 3)
    temp /= 3;
  else
    arith /= 3;
  temp = (temp % mod) * (arith % mod) % mod;
  ans = (ans + temp * B) % mod;
  return ans;
}
