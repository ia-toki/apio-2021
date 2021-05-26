#include <cassert>
#include <csignal>
#include <cstdio>

#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  assert(argc >= 3);

  {
    // Keep alive on broken pipes
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, NULL);
  }

  // Must be in this order
  FILE *fout = fopen(argv[2], "a");
  FILE *fin = fopen(argv[1], "r");

  {
    std::string in_secret = "209636825ecd2c38299498cd26890ef71dd128c2";
    std::string out_secret = "fc5005cd102fffc504bc1615010ebf88823d65f3";
    char secret[1000];
    assert(1 == scanf("%s", secret));
    if (std::string(secret) != in_secret) {
      printf("%s\n", out_secret.c_str());
      printf("SV\n");
      fclose(stdout);
      return 0;
    }
  }

  int N, Q;
  assert(2 == scanf("%d %d", &N, &Q));

  std::vector<int> H(N);
  for (int i = 0; i < N; ++i) {
    assert(1 == scanf("%d", &H[i]));
  }

  fprintf(fout, "%d\n", N);
  for (int i = 0; i < N; ++i) {
    if (i > 0) {
      fprintf(fout, " ");
    }
    fprintf(fout, "%d", H[i]);
  }
  fprintf(fout, "\n");
  fflush(fout);

  std::vector<int> jumps(Q);
  for (int i = 0; i < Q; ++i) {
    int A, B, C, D;
    assert(4 == scanf("%d %d %d %d", &A, &B, &C, &D));
    fprintf(fout, "%d %d %d %d\n", A, B, C, D);
    fflush(fout);

    {
      std::string in_secret = "P";
      std::string out_secret = "fc5005cd102fffc504bc1615010ebf88823d65f3";
      char secret[100];
      if (fscanf(fin, "%5s", secret) != 1 || std::string(secret) != in_secret) {
        printf("%s\n", out_secret.c_str());
        printf("SV\n");
        fclose(stdout);
        return 0;
      }
    }

    if (fscanf(fin, "%d", &jumps[i]) != 1) {
      std::string out_secret = "fc5005cd102fffc504bc1615010ebf88823d65f3";
      printf("%s\n", out_secret.c_str());
      printf("SV\n");
      fclose(stdout);
      return 0;
    }
  }
  fprintf(fout, "%d %d %d %d\n", -1, -1, -1, -1);
  fflush(fout);

  fclose(fin);
  fclose(fout);

  {
    std::string out_secret = "fc5005cd102fffc504bc1615010ebf88823d65f3";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }
  for (int i = 0; i < Q; ++i) {
    printf("%d\n", jumps[i]);
  }
  return 0;
}
