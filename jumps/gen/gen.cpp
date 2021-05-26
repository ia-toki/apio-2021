#include "testlib.h"

#include <bits/stdc++.h>
using namespace std;

struct Test {
  vector<int> H;
  vector<tuple<int, int, int, int>> queries;

  void print() {
    int N = H.size();
    int Q = queries.size();
    printf("%d %d\n", N, Q);
    for (int i = 0; i < N; ++i) {
      printf("%d%c", H[i] + 1, " \n"[i == N - 1]);
    }
    for (int i = 0; i < Q; ++i) {
      printf("%d %d %d %d\n",
             get<0>(queries[i]), get<1>(queries[i]),
             get<2>(queries[i]), get<3>(queries[i]));
    }
  }
};

void addRandomQueries(Test &T, int N, int Q, bool fixed_start, bool fixed_end) {
  while (static_cast<int>(T.queries.size()) < Q) {
    int B = rnd.next(0, N - 2);
    int A = fixed_start ? B : rnd.next(0, B);

    int C = rnd.next(B + 1, N - 1);
    int D = fixed_end ? C : rnd.next(C, N - 1);
    T.queries.emplace_back(A, B, C, D);
  }
}

Test randomCase(int N, int Q, bool increasing_H,
                bool fixed_start, bool fixed_end)
{
  Test T;

  T.H = vector<int>(N);
  iota(T.H.begin(), T.H.end(), 0);
  if (!increasing_H) {
    shuffle(T.H.begin(), T.H.end());
  }

  T.queries.emplace_back(0, 0, N - 1, N - 1);
  addRandomQueries(T, N, Q, fixed_start, fixed_end);

  return T;
}

Test almostIncreasingH(int N, int Q, int swaps,
                       bool fixed_start, bool fixed_end) {
  Test T;

  T.H = vector<int>(N);
  iota(T.H.begin(), T.H.end(), 0);
  while (swaps-- > 0) {
    int x = rnd.next(0, N - 2);
    int y = rnd.next(x + 1, N - 1);
    swap(T.H[x], T.H[y]);
  }

  T.queries.emplace_back(0, 0, N - 1, N - 1);
  addRandomQueries(T, N, Q, fixed_start, fixed_end);

  return T;
}

Test decreasingIncreasingH(int N, int Q, bool fixed_start, bool fixed_end) {
  Test T;

  T.H = vector<int>(N);
  iota(T.H.begin(), T.H.end(), 0);
  shuffle(T.H.begin(), T.H.end());

  int M = fixed_end ? N - 1 : rnd.next(1, N - 1);
  sort(T.H.begin(), T.H.begin() + M, std::greater<>());
  sort(T.H.begin() + M, T.H.end());

  if (fixed_start && fixed_end) {
    T.queries.emplace_back(0, 0, N - 1, N - 1);
  } else if (fixed_start) {
    T.queries.emplace_back(0, 0, M, N - 1);
  } else if (fixed_end) {
    T.queries.emplace_back(0, M - 1, N - 1, N - 1);
  } else {
    T.queries.emplace_back(0, M - 1, M, N - 1);
  }

  while (static_cast<int>(T.queries.size()) < Q) {
    int B = rnd.next(0, M - 1);
    int A = fixed_start ? B : rnd.next(0, B);

    int C = rnd.next(M, N - 1);
    int D = fixed_end ? C : rnd.next(C, N - 1);
    T.queries.emplace_back(A, B, C, D);
  }

  return T;
}

Test zigzag(int N, int Q, bool fixed_start, bool fixed_end) {
  Test T;

  deque<int> dq;
  for (int i = 0; i < N; ++i) {
    if (i / 3 % 2 == 0) {
      dq.push_back(i);
    } else {
      dq.push_front(i);
    }
  }
  T.H = vector<int>(dq.begin(), dq.end());

  int shortest_height_index = 0;
  int tallest_height_index = 0;
  for (int i = 0; i < N; ++i) {
    if (T.H[i] == 0) {
      shortest_height_index = i;
    }
    if (T.H[i] == N - 1) {
      tallest_height_index = i;
    }
  }
  if (shortest_height_index > tallest_height_index) {
    reverse(T.H.begin(), T.H.end());
    shortest_height_index = N - shortest_height_index;
    tallest_height_index = N - tallest_height_index;
  }

  T.queries.emplace_back(shortest_height_index, shortest_height_index,
                         tallest_height_index, tallest_height_index);
  addRandomQueries(T, N, Q, fixed_start, fixed_end);

  return T;
}

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);

  if (strcmp(argv[1], "random") == 0) {
    int N = rnd.next(atoi(argv[2]), atoi(argv[3]));
    int Q = rnd.next(atoi(argv[4]), atoi(argv[5]));
    bool increasing_H = strcasecmp("true", argv[6]) == 0;
    bool fixed_start = strcasecmp("true", argv[7]) == 0;
    bool fixed_end = strcasecmp("true", argv[8]) == 0;
    randomCase(N, Q, increasing_H, fixed_start, fixed_end).print();
  } else if (strcmp(argv[1], "almost-increasing-H") == 0) {
    int N = rnd.next(atoi(argv[2]), atoi(argv[3]));
    int Q = rnd.next(atoi(argv[4]), atoi(argv[5]));
    int swaps = atoi(argv[6]);
    bool fixed_start = strcasecmp("true", argv[7]) == 0;
    bool fixed_end = strcasecmp("true", argv[8]) == 0;
    almostIncreasingH(N, Q, swaps, fixed_start, fixed_end).print();
  } else if (strcmp(argv[1], "decreasing-increasing-H") == 0) {
    int N = rnd.next(atoi(argv[2]), atoi(argv[3]));
    int Q = rnd.next(atoi(argv[4]), atoi(argv[5]));
    bool fixed_start = strcasecmp("true", argv[6]) == 0;
    bool fixed_end = strcasecmp("true", argv[7]) == 0;
    decreasingIncreasingH(N, Q, fixed_start, fixed_end).print();
  } else if (strcmp(argv[1], "zigzag") == 0) {
    int N = rnd.next(atoi(argv[2]), atoi(argv[3]));
    int Q = rnd.next(atoi(argv[4]), atoi(argv[5]));
    bool fixed_start = strcasecmp("true", argv[6]) == 0;
    bool fixed_end = strcasecmp("true", argv[7]) == 0;
    zigzag(N, Q, fixed_start, fixed_end).print();
  }
}
