#include "testlib.h"

#include <string.h>

#include <string>
#include <vector>

const int kMaxN = 200'000;
const int kMaxQ = 100'000;

int main(int, char *argv[]) {
  registerValidation();
  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"209636825ecd2c38299498cd26890ef71dd128c2",
          "Secret not found!");

  const char* subtask_name = argv[1];

  int maxN = kMaxN, maxQ = kMaxQ;
  if (strcmp(subtask_name, "cubic") == 0) {
    maxN = 200;
    maxQ = 200;
  } else if (strcmp(subtask_name, "quadratic") == 0) {
    maxN = 2000;
    maxQ = 2000;
  } else if (strcmp(subtask_name, "small-query") == 0) {
    maxQ = 5;
  }

  int N = inf.readInt(2, maxN, "N");
  inf.readSpace();
  int Q = inf.readInt(1, maxQ, "Q");
  inf.readEoln();

  std::vector<int> H = inf.readInts(N, 1, N, "H");
  inf.readEoln();

  if (strcmp(subtask_name, "increasing-H") == 0) {
    for (int i = 0; i < N; ++i) {
      ensuref(H[i] == i + 1, "H[i] != i + 1");
    }
  } else {
    std::sort(H.begin(), H.end());
    ensuref(std::unique(H.begin(), H.end()) == H.end(), "H is not unique!");
  }

  for (int i = 0; i < Q; ++i) {
    int A = inf.readInt(0, N - 1, "A");
    inf.readSpace();
    int B = inf.readInt(A, N - 1, "B");
    inf.readSpace();
    int C = inf.readInt(B + 1, N - 1, "C");
    inf.readSpace();
    int D = inf.readInt(C, N - 1, "D");
    inf.readEoln();

    if (strcmp(subtask_name, "fixed-start-end") == 0) {
      ensuref(A == B, "Start is not fixed!");
      ensuref(C == D, "End is not fixed!");
    }
    if (strcmp(subtask_name, "fixed-end") == 0) {
      ensuref(C == D, "End is not fixed!");
    }
  }
  inf.readEof();

  return 0;
}
