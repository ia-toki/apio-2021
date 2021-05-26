#include "roads.h"

// O(N log N) solution implemented with Treap
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define all(c) ((c).begin()), ((c).end())
#define sz(x) ((int)(x).size())

const ll INF = 1LL << 50;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
inline int getRand(int x, int y) {
  return uniform_int_distribution<int>(x, y)(rng);
}

struct Treap {
  struct node {
    node *ch[2];
    ll sum;
    int count, key, value;
    node() {ch[0] = ch[1] = NULL; sum = count = 0;}
    node(int k, int v) {
      ch[0] = ch[1] = NULL;
      count = 1;
      key = k;
      sum = value = v;
    }
    void set(node * lft, node * rgt) {
      ch[0] = lft; ch[1] = rgt;
      sum = (lft ? lft->sum : 0) + (rgt ? rgt->sum : 0) + value;
      count = (lft ? lft->count : 0) + (rgt ? rgt->count : 0) + 1;
    }
  };
  node * root;
  Treap() {root = NULL;}
  node* merge(node* A, node* B) {
    if (!A) return B;
    if (!B) return A;
    if (A->key > B->key) {
      A->set(A->ch[0], merge(A->ch[1], B));
      return A;
    } else {
      B->set(merge(A, B->ch[0]), B->ch[1]);
      return B;
    }
  }
  pair<node*, node*> split(node * A, int v) {
    if (!A) return {NULL, NULL};
    node * lft = A->ch[0];
    node * rgt = A->ch[1];
    int Av = A->value;
    if (Av <= v) {
      pair<node*, node*> P = split(rgt, v);
      A->set(lft, P.first);
      return {A, P.second};
    } else {
      pair<node*, node*> P = split(lft, v);
      A->set(P.second, rgt);
      return {P.first, A};
    }
  }
  void add(int x) {
    node * nd = new node(getRand(0, 1 << 30), x);
    if (!root) {
      root = nd;
      return;
    }
    pair<node*, node*> P = split(root, x);
    root = merge(P.first, merge(nd, P.second));
  }
  ll getSumOfMinK(int k) {
    if (k == 0) return k;
    node * curr = root;
    ll ret = 0;
    while (curr) {
      node* lft = curr->ch[0];
      ll sum = lft ? lft->sum : 0;
      int count = lft ? lft->count : 0;
      if (count >= k) {
        curr = lft;
        continue;
      }
      ret += sum + curr->value;
      k -= count + 1;
      if (k == 0) return ret;
      curr = curr->ch[1];
    }
    return ret;
  }
  ll get(int k) {
    int size = root ? root->count : 0;
    return getSumOfMinK(size - min(k, size));
  }
};

std::vector<long long> minimum_closure_costs(int n, vector<int> a, vector<int> b, vector<int> w) {
  ll W = 0;
  vector<vector<int>> adj(n);
  for (int i = 0; i + 1 < n; i++) {
    adj[a[i]].push_back(i);
    adj[b[i]].push_back(i);
    W += w[i];
  }
  vector<int> perm(n);
  iota(all(perm), 0);
  sort(all(perm), [&](int i, int j) {return sz(adj[i]) > sz(adj[j]);});
  vector<vector<int>> nodes(n);
  vector<Treap> D(n);
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
        D[v].add(w[ind]);
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
        dp[s][0] = min(dp[s][0], sum + D[s].get(k - i));
        if (i < k) {
          dp[s][1] = min(dp[s][1], sum + D[s].get(k - 1 - i));
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
