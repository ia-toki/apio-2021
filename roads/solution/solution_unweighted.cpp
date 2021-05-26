#include "roads.h"

// Unweighted greedy solution
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define all(c) ((c).begin()), ((c).end())
#define sz(x) ((int)(x).size())

vector<ll> minimum_closure_costs(int n, vector<int> a, vector<int> b, vector<int> w) {
  vector<vector<int>> adj(n);
  for (int i = 0; i + 1 < n; i++) {
    adj[a[i]].push_back(b[i]);
    adj[b[i]].push_back(a[i]);
  }
  vector<int> perm(n);
  iota(all(perm), 0);
  sort(all(perm), [&](int i, int j) {return sz(adj[i]) > sz(adj[j]);});
  for (int i = 0; i < n; i++) sort(all(adj[i]), [&](int j, int k) {return sz(adj[j]) > sz(adj[k]);});
  vector<ll> answers(n); answers[0] = n - 1;
  vector<int> deg(n);
  vector<bool> vis(n);
  for (int k = 1; k < n; k++) {
    for (int u : perm) {
      deg[u] = sz(adj[u]);
      vis[u] = false;
      if (deg[u] <= k) break;
    }
    function<void(int, int)> dfs = [&](int s, int p) {
      for (int v : adj[s]) {
        if (sz(adj[v]) <= k) break;
        if (v == p) continue;
        dfs(v, s);
      }
      answers[k] += max(0, deg[s] - k);
      if (deg[s] > k) deg[p]--;
      vis[s] = true;
    };
    for (int u : perm) {
      if (sz(adj[u]) <= k) break;
      if (!vis[u]) dfs(u, u);
    }
  }
  return answers;
}
