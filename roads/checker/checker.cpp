#include "testlib.h"

#include <string>

int main(int argc, char * argv[]) {
  registerChecker("roads", argc, argv);

  std::string output_secret = "064bde9ff69ddc34b3b45c2d26d58873d85290d3";

  readBothSecrets(output_secret);
  readBothGraderResults();

  compareRemainingLines();
}
