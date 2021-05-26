#include "hexagon.h"
#include <bits/stdc++.h>

using namespace std;

constexpr int mod = 1e9 + 7;

// sum of i for i=1..n
long long calc(int n) {
  long long ret = 1LL * n * (n + 1);
  ret /= 2;
  ret %= mod;
  return ret;
}

// sum of i^2 for i=1..n
long long calc2(int n) {
  long long ret = 1LL * n * (n + 1) % mod;
  ret = 1LL * ret * (2 * n + 1) % mod;
  ret /= 6; // <-- this should be done before modulo
  return ret;
}

int draw_territory(int /*N*/, int A, int B, vector<int> /*D*/, vector<int> L) {
  int len = L[0] + 1;
  long long ans = ((calc(len) * (A - B) % mod) + (calc2(len) * B % mod)) % mod;
  if (ans < 0)
    ans += mod;
  return ans;
}
