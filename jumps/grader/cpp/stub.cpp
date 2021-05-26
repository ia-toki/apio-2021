#include "jumps.h"

#include <cassert>
#include <cstdio>

#include <string>
#include <vector>

int main() {
  int N;
  assert(1 == scanf("%d", &N));

  std::vector<int> H(N);
  for (int i = 0; i < N; ++i) {
    assert(1 == scanf("%d", &H[i]));
  }

  init(N, H);

  while (true) {
    int A, B, C, D;
    if (scanf("%d %d %d %d", &A, &B, &C, &D) != 4) {
      return 0;
    }
    if (A < 0 || B < 0 || C < 0 || D < 0) {
      break;
    }

    int result = minimum_jumps(A, B, C, D);
    {
      std::string out_secret = "P";
      printf("%s\n", out_secret.c_str());
    }
    printf("%d\n", result);
    fflush(stdout);
  }
  return 0;
}
