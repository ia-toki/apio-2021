import math
import random

TESTSET_PREFIX = 'Testset'


def manual(testcase):
  print(f"manual {testcase}.in")

def include(include):
  print(f"@include {include.name or include.__name__[len(TESTSET_PREFIX):]}")

def indexify_testcases(gen_func):
  testcase_index = 0
  def _gen_func(*args):
    nonlocal testcase_index
    gen_func(*args, testcase_index=testcase_index)
    testcase_index += 1
  return _gen_func


globalMinN = 3
globalMaxN = 200_000

globalMaxSumL = 1_000_000_000
linearMaxSumL = 200_000
quadraticMaxSumL = 2_000

globalMinA = 0
globalMaxA = 1_000_000_000

globalMinB = 0
globalMaxB = 1_000_000_000


@indexify_testcases
def gen_triangle(maxA, maxB, dir1, dir2, dir3, length, testcase_index):
  print(f"gen triangle {maxA} {maxB} {dir1} {dir2} {dir3} {length} "
        f"{testcase_index}")


@indexify_testcases
def gen_random_linear_traversal(maxA, maxB, maxN, maxSumL, constantL, maxCnt,
                                maxLen, testcase_index):
  print(f"gen random_linear_traversal {maxA} {maxB} {maxN} {maxSumL} "
        f"{constantL} {maxCnt} {maxLen} {testcase_index}")


@indexify_testcases
def gen_dfs_linear_traversal(maxA, maxB, maxN, maxSumL, constantL, maxCnt,
                             maxLen, rotate, testcase_index):
  print(f"gen dfs_linear_traversal {maxA} {maxB} {maxN} {maxSumL} {constantL} "
        f"{maxCnt} {maxLen} {rotate} {testcase_index}")


@indexify_testcases
def gen_spiral(maxA, maxB, maxN, maxSumL, constantL, moreSpace, rotate,
               testcase_index):
  print(f"gen spiral {maxA} {maxB} {maxN} {maxSumL} {constantL} {moreSpace} "
        f"{rotate} {testcase_index}")


@indexify_testcases
def gen_zig_zag(maxA, maxB, maxN, maxSumL, constantL, rotate, testcase_index):
  print(f"gen zig_zag {maxA} {maxB} {maxN} {maxSumL} {constantL} {rotate} "
        f"{testcase_index}")


@indexify_testcases
def gen_snake(maxA, maxB, maxN, maxSumL, constantL, length, rotate,
              testcase_index):
  print(f"gen snake {maxA} {maxB} {maxN} {maxSumL} {constantL} {length} "
        f"{rotate} {testcase_index}")


def common_testcases(maxA, maxB, maxN, maxSumL, constantL):
  gen_random_linear_traversal(maxA, maxB, maxN, maxSumL, constantL,
                              min(maxN * 15, maxSumL), 2)
  gen_random_linear_traversal(maxA, maxB, maxN, maxSumL, constantL,
                              min(maxN * 15, maxSumL), 10)
  gen_random_linear_traversal(maxA, maxB, maxN, maxSumL, constantL,
                              min(maxN * 15, maxSumL), 100)
  gen_random_linear_traversal(maxA, maxB, maxN, maxSumL, constantL,
                              min(maxN * 15, maxSumL), 300)

  if constantL:
    gen_dfs_linear_traversal(maxA, maxB, maxN, maxSumL, constantL,
                             min(maxN, maxSumL)//17, 17, 0)
    gen_dfs_linear_traversal(maxA, maxB, maxN, maxSumL, constantL,
                             min(maxN, maxSumL)//6, 6, 1)
    gen_dfs_linear_traversal(maxA, maxB, maxN, maxSumL, constantL,
                             min(maxN, maxSumL)//5, 5, 2)
  else:
    gen_dfs_linear_traversal(maxA, maxB, maxN, maxSumL, constantL,
                             min(maxN * 15, maxSumL), 17, 0)
    gen_dfs_linear_traversal(maxA, maxB, maxN, maxSumL, constantL,
                             min(maxN * 15, maxSumL), 12, 1)
    gen_dfs_linear_traversal(maxA, maxB, maxN, maxSumL, constantL,
                             min(maxN * 6, maxSumL), 5, 2)

  gen_spiral(maxA, maxB, maxN, maxSumL, constantL, 0, 0)
  gen_spiral(maxA, maxB, maxN, maxSumL, constantL, 3, 2)
  gen_spiral(maxA, maxB, maxN, maxSumL, constantL, 10, 4)

  gen_zig_zag(maxA, maxB, maxN, maxSumL, constantL, 3)
  gen_zig_zag(maxA, maxB, maxN, maxSumL, constantL, 5)
  gen_zig_zag(maxA, maxB, maxN, maxSumL, constantL, 4)

  rotates = [0, 1, 2]
  random.shuffle(rotates)
  gen_snake(maxA, maxB, maxN, maxSumL, constantL, max(maxSumL // maxN, 10),
            rotates[0])
  gen_snake(maxA, maxB, maxN, maxSumL, constantL,
            min(maxN // 4, int(math.sqrt(maxSumL))), rotates[1])
  gen_snake(maxA, maxB, maxN, maxSumL, constantL,
            2 * min(maxN // 4, int(math.sqrt(maxSumL))), rotates[2])

class Testset():
  name = None

  def __init__(self):
    assert self.__class__.__name__.startswith(TESTSET_PREFIX)
    print("")
    print(f"@testset {self.__class__.__name__[len(TESTSET_PREFIX):]}")


class TestsetQuadraticSumLTriangle(Testset):
  def __init__(self):
    super().__init__()
    gen_triangle(globalMaxA, globalMaxB, 0, 2, 4, quadraticMaxSumL//3)
    gen_triangle(globalMaxA, globalMaxB, 3, 1, 5, quadraticMaxSumL//3 - 13)
    gen_triangle(globalMaxA, globalMaxB, 2, 0, 4, 2)


class TestsetBEqualsZeroLinearSumLTriangle(Testset):
  def __init__(self):
    super().__init__()
    gen_triangle(globalMaxA, 0, 3, 5, 1, linearMaxSumL//3)
    gen_triangle(globalMaxA, 0, 0, 4, 2, linearMaxSumL//3 - 231)
    gen_triangle(globalMaxA, 0, 1, 5, 3, 1)


class TestsetLinearSumLTriangle(Testset):
  def __init__(self):
    super().__init__()
    gen_triangle(globalMaxA, globalMaxB, 2, 4, 0, linearMaxSumL//3)
    gen_triangle(globalMaxA, globalMaxB, 1, 3, 5, linearMaxSumL//3 - 271)


class Subtask():
  name = None

  def __init__(self):
    print("")
    print(f"@subtask {self.name}")


class SubtaskSamples(Subtask):
  name = "samples"

  def __init__(self):
    super().__init__()
    manual("sample-1")


class SubtaskBEqualsZeroTriangle(Subtask):
  name = "B-equals-zero-triangle"

  def __init__(self):
    super().__init__()
    include(TestsetBEqualsZeroLinearSumLTriangle)
    gen_triangle(globalMaxA, 0, 4, 2, 0, globalMaxSumL//3)
    gen_triangle(globalMaxA, 0, 4, 0, 2, globalMaxSumL//3 - 314)

class SubtaskTriangle(Subtask):
  name = "triangle"

  def __init__(self):
    super().__init__()
    include(SubtaskBEqualsZeroTriangle)
    include(TestsetQuadraticSumLTriangle)
    include(TestsetLinearSumLTriangle)
    gen_triangle(globalMaxA, globalMaxB, 5, 3, 1, globalMaxSumL//3)
    gen_triangle(globalMaxA, globalMaxB, 5, 1, 3, globalMaxSumL//3 - 97)


class SubtaskQuadraticSumL(Subtask):
  name = "quadratic-sum-L"

  def __init__(self):
    super().__init__()
    include(SubtaskSamples)
    include(TestsetQuadraticSumLTriangle)

    common_testcases(globalMaxA, globalMaxB, globalMaxN, quadraticMaxSumL, 0)


class SubtaskBEqualsZeroSmall(Subtask):
  name = "B-equals-zero-small"

  def __init__(self):
    super().__init__()
    include(TestsetBEqualsZeroLinearSumLTriangle)

    common_testcases(globalMaxA, 0, globalMaxN, linearMaxSumL, 0)


class SubtaskBEqualsZero(Subtask):
  name = "B-equals-zero"

  def __init__(self):
    super().__init__()
    include(SubtaskBEqualsZeroSmall)
    include(SubtaskBEqualsZeroTriangle)

    common_testcases(globalMaxA, 0, globalMaxN, globalMaxSumL, 0)


class SubtaskLinearSumL(Subtask):
  name = "linear-sum-L"

  def __init__(self):
    super().__init__()
    include(SubtaskSamples)
    include(TestsetLinearSumLTriangle)
    include(SubtaskQuadraticSumL)
    include(SubtaskBEqualsZeroSmall)

    common_testcases(globalMaxA, globalMaxB, globalMaxN, linearMaxSumL, 0)


class SubtaskConstantL(Subtask):
  name = "constant-L"

  def __init__(self):
    super().__init__()
    include(SubtaskTriangle)

    common_testcases(globalMaxA, globalMaxB, globalMaxN, globalMaxSumL, 1)


class SubtaskFull(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()
    include(SubtaskSamples)
    include(SubtaskBEqualsZeroTriangle)
    include(SubtaskTriangle)
    include(SubtaskBEqualsZeroSmall)
    include(SubtaskBEqualsZero)
    include(SubtaskLinearSumL)
    include(SubtaskConstantL)

    common_testcases(globalMaxA, globalMaxB, globalMaxN, globalMaxSumL, 0)


def main():
  random.seed(314159)
  for testset in Testset.__subclasses__():
    testset()
  for subtask in Subtask.__subclasses__():
    subtask()

main()
