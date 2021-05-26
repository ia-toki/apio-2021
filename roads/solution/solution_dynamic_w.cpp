#include "roads.h"

#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define all(c) ((c).begin()), ((c).end())
#define sz(x) ((int)(x).size())

const ll INF = 1LL << 50;

inline void insert(const vector <long long> &V, vector <int> &D, int k){
  int pos = lower_bound(all(V), k) - begin(V);
  D[pos]++;
}

inline long long get(const vector <long long> &V, const vector <int> &D, int k){
  long long ans = 0;
  long long total = accumulate(all(D), 0);
  k = total - k;
  for(int i = 0;i < sz(D);++i){
    if(k <= 0) break;
    int need = min(k, D[i]);
    k -= need;
    ans += need * V[i];
  }
  return ans;
}

std::vector<long long> minimum_closure_costs(int n, vector<int> a, vector<int> b, vector<int> w) {
  ll W = 0;
  vector<vector<long long>> w_bucket(n);
  vector<vector<int>> adj(n), D(n);
  for (int i = 0; i + 1 < n; i++) {
    adj[a[i]].push_back(i);
    adj[b[i]].push_back(i);
    W += w[i];
    w_bucket[a[i]].push_back(w[i]);
    w_bucket[b[i]].push_back(w[i]);
  }
  vector<int> perm(n);
  iota(all(perm), 0);
  sort(all(perm), [&](int i, int j) {return sz(adj[i]) > sz(adj[j]);});
  vector<vector<int>> nodes(n);
  for (int i = 0; i < n; i++) {
    
    sort(all(w_bucket[i]));
    w_bucket[i].erase(unique(all(w_bucket[i])),w_bucket[i].end());

    D[i].resize(sz(w_bucket[i]));

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
        insert(w_bucket[v], D[v], w[ind]);
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
        dp[s][0] = min(dp[s][0], sum + get(w_bucket[s], D[s], k - i));
        if (i < k) {
          dp[s][1] = min(dp[s][1], sum + get(w_bucket[s], D[s], k - 1 - i));
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
