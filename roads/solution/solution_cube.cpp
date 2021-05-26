#include "roads.h"

// O(N^3) solution
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define all(c) ((c).begin()), ((c).end())
#define sz(x) ((int)(x).size())

const ll INF = 1LL << 50;

vector<ll> minimum_closure_costs(int n, vector<int> a, vector<int> b, vector<int> w) {
  ll W = 0;
  vector<vector<int>> adj(n);
  for (int i = 0; i + 1 < n; i++) {
    adj[a[i]].push_back(i);
    adj[b[i]].push_back(i);
    W += w[i];
  }
  vector<vector<ll>> dp(n, vector<ll>(2));
  function<void(int, int, int)> dfs = [&](int s, int p, int k) {
    vector<ll> V(1, 0);
    for (int ind : adj[s]) {
      int v = a[ind] ^ b[ind] ^ s;
      if (v == p)
        continue;
      dfs(v, s, k);
      vector<ll> old = V;
      V.push_back(0LL);
      fill(all(V), INF);
      for (int i = 0; i < sz(old); i++) {
        ll cost = old[i];
        V[i] = min(V[i], cost + w[ind] + dp[v][0]);
        V[i + 1] = min(V[i + 1], cost + dp[v][1]);
      }
    }
    dp[s][0] = dp[s][1] = INF;
    for (int i = 0; i < min(sz(V), k + 1); i++) {
      dp[s][0] = min(dp[s][0], V[i]);
      if (i < k)
        dp[s][1] = min(dp[s][1], V[i]);
    }
  };
  vector<ll> answers(n, 0);
  answers[0] = W;
  for (int k = 1; k < n; k++) {
    dfs(0, 0, k);
    answers[k] += dp[0][0];
  }
  return answers;
}
