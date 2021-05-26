#include "roads.h"

#include <vector>
#include <algorithm>
using namespace std;

std::vector<long long> minimum_closure_costs(int N, std::vector<int> U,
                                             std::vector<int> V,
                                             std::vector<int> W) {
  vector <long long> ans(W.begin(), W.end());
  sort(ans.begin(), ans.end());
  for(int i = 1;i < (int) ans.size();i++) ans[i] += ans[i - 1];
  reverse(ans.begin(), ans.end());
  ans.push_back(0);
  return ans;
}
