#include "testlib.h"
#include "treegenerator.h"

#include <bits/stdc++.h>
using namespace std;
using namespace tree_generator_by_ouuan;

typedef pair<int, int> pii;

struct Test {
  int N;
  vector<int> U, V, W;

  Test(int _N): N(_N) {};
  Test() {};

  void print() {
    printf("%d\n", N);
    for (int i = 0; i < N - 1; ++i) {
      printf("%d %d %d\n", U[i], V[i], W[i]);
    }
  }
};


// This will assign each edges with weight between the pair W
void assignWeightRandomRange(Test &T, pii W) {
  (T.W).resize(T.N - 1);
  for (int &w : T.W) {
    w = rnd.next(W.first, W.second);
  }
}

void assignWeightToDegreeSum(Test &T, int div) {
  (T.W).resize(T.N - 1);
  vector <int> degree(T.N);
  for (int i = 0; i < T.N - 1; i++) {
    degree[T.U[i]]++;
    degree[T.V[i]]++;
  }
  for (int i = 0; i < T.N - 1; i++) {
    T.W[i] = degree[T.U[i]] + degree[T.V[i]];
    T.W[i] /= div; if(T.W[i] == 0) T.W[i]++;
  }
}

void assignWeightToDegreeMin(Test &T, int offset) {
  (T.W).resize(T.N - 1);
  vector <int> degree(T.N);
  for (int i = 0; i < T.N - 1; i++) {
    degree[T.U[i]]++;
    degree[T.V[i]]++;
  }
  for (int i = 0; i < T.N - 1; i++) {
    T.W[i] = min(degree[T.U[i]], degree[T.V[i]]) + rnd.next(-offset, offset);
    if(T.W[i] <= 0) T.W[i] = 1;
  }
}

// Start of Trees Generator --------------

// void get(vector <int> &U, vector <int> &V,
//          bool ShuffleV = 1, bool ShuffleE = 1,
//          bool swapV = 0);
// This will assign edges to U and V consecutively
// and shuffle accordingly. By default auto shuffle both, and dont swap edges. The generator bit weird for star...
// Random tree with size N
Test randomCase(int N) {

  Test T(N);

  Tree tree(N);
  tree.get(T.U, T.V);

  return T;
}

Test starCase(int N) {
  Test T(N);

  Tree tree;
  tree.star(N, 0);
  tree.get(T.U, T.V, 0, 1, 1);

  return T;
}

// Generate line case
Test lineCase(int N) {

  Test T(N);

  Tree tree;
  tree.chain(N, 0);
  tree.get(T.U, T.V, 0, 0, 1);

  return T;
}

// All nodes degree doesn't exceed K
Test maxDegreeCase(int N, int K) {
  Test T(N);

  Tree tree;
  tree.maxDegree(N, K, 0);
  tree.get(T.U, T.V);

  return T;
}

// All non-leaf nodes degree in range [K, K + 5]
Test minDegreeCase(int N, int K) {
  Test T(N);

  Tree tree;
  tree.minDegree(N, K, 5, 0);
  tree.get(T.U, T.V);

  return T;
}

// All nodes will try to make K-nary, will add accordingly if can't
Test knaryCase(int N, int K) {
  Test T(N);

  Tree tree;
  tree.complete(N, K, 0);
  tree.get(T.U, T.V);

  return T;
}

// All nodes will try to make K-nary, with N/2 nodes and some offsets for the rest
Test flowerKnaryCase(int N, int K) {
  Test T(N);

  Tree tree;
  tree.flowerKnary(N, N/2, K, 0);
  tree.get(T.U, T.V);

  return T;
}

Test silkwormCase(int N) {
  Test T(N);

  Tree tree;
  tree.silkworm(N, 0);
  tree.get(T.U, T.V);

  return T;
}

Test starShuffleCase(int N) {
  Test T(N);

  Tree tree;
  tree.star(N, 0);
  tree.get(T.U, T.V);

  return T;
}

Test distinctCase(int N) {
  Test T(N);

  Tree tree;
  tree.distinctDegree(N, 0);
  tree.get(T.U, T.V);

  return T;
}


Test halvingCase(int N) {
  Test T(N);

  Tree tree;
  tree.halvingDegree(N, 0);
  tree.get(T.U, T.V);

  return T;
}

// End of Trees Generator --------------

// Seed the default tree generator with first rnd.next of testlib.h
inline void setTreeGeneratorSeed(int seed) {
  defaultRNG = mt19937(seed);
}

int main(int argc, char *argv[]) {
  registerGen(argc, argv, 0);
  setTreeGeneratorSeed(rnd.next(INT_MAX));

  Test T;
  int N = rnd.next(atoi(argv[2]), atoi(argv[3]));

  if (strcmp(argv[1], "random") == 0)
    T = randomCase(N);
  if (strcmp(argv[1], "distinct") == 0)
    T = distinctCase(N);
  if (strcmp(argv[1], "halving") == 0)
    T = halvingCase(N);
  if (strcmp(argv[1], "star") == 0)
    T = starCase(N);
  if (strcmp(argv[1], "starshuffle") == 0)
    T = starShuffleCase(N);
  if (strcmp(argv[1], "line") == 0)
    T = lineCase(N);
  if (strcmp(argv[1], "silkworm") == 0)
    T = silkwormCase(N);
  if (strcmp(argv[1], "degree") == 0)
    T = maxDegreeCase(N, atoi(argv[6]));
  if (strcmp(argv[1], "mindegree") == 0)
    T = minDegreeCase(N, atoi(argv[6]));
  if (strcmp(argv[1], "knary") == 0)
    T = knaryCase(N, atoi(argv[6]));
  if (strcmp(argv[1], "flower") == 0)
    T = flowerKnaryCase(N, atoi(argv[6]));

  if(strcmp(argv[4], "sum") == 0){
    assignWeightToDegreeSum(T, atoi(argv[5]));
  }else if(strcmp(argv[4], "min") == 0){
    assignWeightToDegreeMin(T, atoi(argv[5]));
  }else{
    pii W = {atoi(argv[4]), atoi(argv[5])};
    assignWeightRandomRange(T, W);
  }
  
  T.print();
}
