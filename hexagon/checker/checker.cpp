#include "testlib.h"

#include <string>

int main(int argc, char * argv[]) {
  registerChecker("hexagon", argc, argv);

  std::string output_secret = "0a7385ba87a80c36d1403904d6d65e155ddd6e9c";

  readBothSecrets(output_secret);
  readBothGraderResults();

  compareRemainingLines();
}
