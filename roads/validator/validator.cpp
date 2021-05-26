#include "testlib.h"

#include <string.h>

#include <functional>
#include <numeric>
#include <string>
#include <vector>

const int kMaxN = 100'000;
const int kMaxW = 1'000'000'000;

int main(int, char *argv[]) {
  registerValidation();

  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"c10234cabdfd6eae47773fb6f163e0350fc797e1",
          "Secret not found!");

  const char* subtask_name = argv[1];

  int maxN = kMaxN;
  if (strcmp(subtask_name, "cubic") == 0) {
    maxN = 200;
  } else if (strcmp(subtask_name, "quadratic") == 0) {
    maxN = 2000;
  }

  int maxW = kMaxW;
  if (strcmp(subtask_name, "constant-W") == 0) {
    maxW = 1;
  } else if (strcmp(subtask_name, "small-W") == 0) {
    maxW = 10;
  }

  int N = inf.readInt(2, maxN, "N");
  inf.readEoln();

  std::vector<int> U(N - 1), V(N - 1);
  for (int i = 0; i < N - 1; ++i) {
    U[i] = inf.readInt(0, N - 1, "U");
    inf.readSpace();
    V[i] = inf.readInt(0, N - 1, "V");
    inf.readSpace();
    inf.readInt(1, maxW, "W");
    inf.readEoln();
  }
  inf.readEof();

  if (strcmp(subtask_name, "star") == 0) {
    ensuref(std::all_of(U.begin(), U.end(), [](int u) { return u == 0; }),
            "Found U[i] != 0");
  }

  if (strcmp(subtask_name, "line") == 0) {
    for (int i = 0; i < N - 1; ++i) {
      ensuref(U[i] == i, "Found U[i] != i");
      ensuref(V[i] == i + 1, "Found V[i] != i + 1");
    }
  }

  ensuref([](int N, const std::vector<int> &U, const std::vector<int>&V) {
    std::vector<int> root(N);
    std::iota(root.begin(), root.end(), 0);
    std::function<int(int)> find = [&find, &root](int u) {
      if (u == root[u]) return u;
      return root[u] = find(root[u]);
    };

    for (int i = 0; i < N - 1; ++i) {
      if (find(U[i]) == find(V[i])) return false;
      root[root[U[i]]] = root[V[i]];
    }

    return true;
  }(N, U, V), "Edges do not form a tree!");

  return 0;
}
