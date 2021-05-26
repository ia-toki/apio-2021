#include "roads.h"

#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define all(c) ((c).begin()), ((c).end())
#define sz(x) ((int)(x).size())

int MAXW;
const ll INF = 1LL << 50;

inline int get(const vector <int> &V, int k){
  int ans = 0;
  int total = accumulate(V.begin(), V.end(), 0);
  k = total - k;
  for(int i = 0;i < sz(V);++i){
    int need = min(k, V[i]);
    k -= need;
    ans += need * i;
    if(k <= 0) break;
  }
  return ans;
}

std::vector<long long> minimum_closure_costs(int n, vector<int> a, vector<int> b, vector<int> w) {
  ll W = 0;
  vector<vector<int>> adj(n);
  for (int i = 0; i + 1 < n; i++) {
    adj[a[i]].push_back(i);
    adj[b[i]].push_back(i);
    W += w[i];
    MAXW = max(MAXW, w[i]);
  }
  vector<int> perm(n);
  iota(all(perm), 0);
  sort(all(perm), [&](int i, int j) {return sz(adj[i]) > sz(adj[j]);});
  vector<vector<int>> nodes(n);
  vector<vector<int>> D(n, vector <int>(MAXW + 1, 0));
  for (int i = 0; i < n; i++) {
    sort(all(adj[i]), [&](int j, int k) {
      int u = a[j] ^ b[j] ^ i;
      int v = a[k] ^ b[k] ^ i;
      return sz(adj[u]) > sz(adj[v]);
    });
    nodes[sz(adj[i])].push_back(i);
  }
  vector<ll> answers(n); answers[0] = W;
  vector<bool> vis(n);
  vector<vector<ll>> dp(n, vector<ll>(2));
  for (int k = 1; k < n; k++) {
    for (int s : nodes[k]) {
      // activate
      for (int ind : adj[s]) {
        int v = a[ind] ^ b[ind] ^ s;
        D[v][w[ind]]++;
      }
    }
    for (int u : perm) {
      vis[u] = false;
      if (sz(adj[u]) <= k) break;
    }
    function<void(int, int)> dfs = [&](int s, int p) {
      vis[s] = true;
      vector<ll> vals;
      ll sum = 0;
      for (int ind : adj[s]) {
        int v = a[ind] ^ b[ind] ^ s;
        if (v == p) continue;
        if (sz(adj[v]) <= k) break;
        dfs(v, s);
        sum += dp[v][0] + w[ind];
        vals.push_back(dp[v][1] - w[ind] - dp[v][0]);
      }
      sort(all(vals));
      dp[s][0] = dp[s][1] = INF;
      for (int i = 0; i <= sz(vals) && i <= k; i++) {
        dp[s][0] = min(dp[s][0], sum + get(D[s], k - i));
        if (i < k) {
          dp[s][1] = min(dp[s][1], sum + get(D[s], k - 1 - i));
        }
        if (i < sz(vals)) sum += vals[i];
      }
    };
    answers[k] = 0;
    for (int u : perm) {
      if (sz(adj[u]) <= k) break;
      if (!vis[u]) {
        dfs(u, u);
        answers[k] += dp[u][0];
      }
    }
  }
  return answers;
}
