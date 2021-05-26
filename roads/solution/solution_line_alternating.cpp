#include "roads.h"

#include <vector>
#include <numeric>
#include <cstring>
using namespace std;

std::vector<long long> minimum_closure_costs(int N, std::vector<int> U,
    std::vector<int> V,
    std::vector<int> W) {
  long long total = 0;
  long long odd = 0;
  for(int i = 0;i < N - 1; ++i) total += W[i];
  for(int i = 0;i < N - 1; i += 2) odd += W[i];
  vector <long long> ans(N, 0);
  ans[0] = total; ans[1] = min(odd, total - odd);
  return ans;
}
