#include "roads.h"

#include <vector>
#include <numeric>
#include <cstring>
using namespace std;

const long long LINF = 4557430888798830399LL;
vector <long long> arr;
int n;
long long memo[100005][2];
long long dp(int pos, bool take) {
  if (pos == n - 1) return 0;
  long long &ret = memo[pos][take];
  if (~ret) return ret;
  ret = dp(pos + 1, 0) + (long long) arr[pos];
  if (!take) ret = min(ret, dp(pos + 1, 1));
  return ret;
}

std::vector<long long> minimum_closure_costs(int N, std::vector<int> U,
    std::vector<int> V,
    std::vector<int> W) {
  n = N;
  memset(memo, -1, sizeof(memo));
  vector <long long> ans(N, 0);
  for (auto i : W) ans[0] += i, arr.push_back(i);
  ans[1] = dp(0, 0);
  return ans;
}
