#include "hexagon.h"

#include <cassert>
#include <cstdio>

#include <vector>
// BEGIN SECRET
#include <string>
// END SECRET

int main() {
  // BEGIN SECRET
  {
    std::string in_secret = "4279751dd9ece9095f11fb277ecbef2e68be1cdf";
    std::string out_secret = "0a7385ba87a80c36d1403904d6d65e155ddd6e9c";
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
  int N, A, B;
  assert(3 == scanf("%d %d %d", &N, &A, &B));
  std::vector<int> D(N), L(N);
  for (int i = 0; i < N; ++i) {
    assert(2 == scanf("%d %d", &D[i], &L[i]));
  }

  int result = draw_territory(N, A, B, D, L);
  // BEGIN SECRET
  {
    std::string out_secret = "0a7385ba87a80c36d1403904d6d65e155ddd6e9c";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }
  // END SECRET
  printf("%d\n", result);
  return 0;
}
