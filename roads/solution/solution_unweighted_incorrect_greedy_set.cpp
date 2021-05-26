#include "roads.h"

#include <vector>
#include <iostream>
#include <set>
using namespace std;

typedef pair<int, int> PII;
#define fi first
#define se second
typedef long long LL;

int degCount[100005];
set <PII> edge[100005];

std::vector<long long> minimum_closure_costs(int N, std::vector<int> U,
    std::vector<int> V,
    std::vector<int> W) {

  for (int i = 0; i < N - 1; i++) {
    degCount[U[i]]++;
    degCount[V[i]]++;
  }

  for (int i = 0; i < N - 1; i++) {
    edge[U[i]].insert({-degCount[V[i]], V[i]});
    edge[V[i]].insert({-degCount[U[i]], U[i]});
  }

  set <PII> perm;

  for (int i = 0; i < N; i++) {
    perm.insert({ -degCount[i], i});
  }

  LL cost = 0;
  vector <long long> ans(N, 0);
  set <PII> removedEdge;
  for (int k = N - 1; k >= 0; k--) {
    while (!perm.empty() && -(*perm.begin()).fi > k) {
      PII fr = *perm.begin();
      if(!edge[fr.se].size()) break;
      if (-fr.fi != degCount[fr.se]) {
        perm.erase(perm.begin());
        if (degCount[fr.se]) perm.insert({ -degCount[fr.se], fr.se});
        continue;
      }

      while (!edge[fr.se].empty()) {

        PII nx = *edge[fr.se].begin();

        
        if (-nx.fi != degCount[nx.se]) {
          edge[fr.se].erase(edge[fr.se].begin());
          if (degCount[nx.se]) edge[fr.se].insert({ -degCount[nx.se], nx.se});
          continue;
        }

        PII curEdge = {nx.se, fr.se};
        if(curEdge.fi > curEdge.se) swap(curEdge.fi, curEdge.se);
        if(!removedEdge.count(curEdge)){
          removedEdge.insert(curEdge);

          degCount[nx.se]--;
          degCount[fr.se]--;
          
          cost++;
          perm.erase(perm.begin());
          if (degCount[fr.se]) perm.insert({ -degCount[fr.se], fr.se});
        }
        
        edge[fr.se].erase(edge[fr.se].begin());

        break;

      }
    }
    ans[k] = cost;
  }

  return ans;
}
