#include "testlib.h"

#include <string>

int main(int argc, char * argv[]) {
  registerChecker("jumps", argc, argv);

  std::string output_secret = "fc5005cd102fffc504bc1615010ebf88823d65f3";

  readBothSecrets(output_secret);
  readBothGraderResults();

  compareRemainingLines();
}
