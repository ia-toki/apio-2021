#include "roads.h"

#include <cassert>
#include <cstdio>

// BEGIN SECRET
#include <string>
// END SECRET
#include <vector>

int main() {
  // BEGIN SECRET
  {
    std::string in_secret = "c10234cabdfd6eae47773fb6f163e0350fc797e1";
    std::string out_secret = "064bde9ff69ddc34b3b45c2d26d58873d85290d3";
    char secret[1000];
    assert(1 == scanf("%s", secret));
    if (std::string(secret) != in_secret) {
      printf("%s\n", out_secret.c_str());
      printf("SV\n");
      fclose(stdout);
      return 0;
    }
  }
  // END SECRET
  int N;
  assert(1 == scanf("%d", &N));

  std::vector<int> U(N - 1), V(N - 1), W(N - 1);
  for (int i = 0; i < N - 1; ++i) {
    assert(3 == scanf("%d %d %d", &U[i], &V[i], &W[i]));
  }

  std::vector<long long> closure_costs = minimum_closure_costs(N, U, V, W);
  // BEGIN SECRET
  {
    std::string out_secret = "064bde9ff69ddc34b3b45c2d26d58873d85290d3";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }
  // END SECRET
  for (int i = 0; i < static_cast<int>(closure_costs.size()); ++i) {
    if (i > 0) {
      printf(" ");
    }
    printf("%lld",closure_costs[i]);
  }
  printf("\n");
  return 0;
}
