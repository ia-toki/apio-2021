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


globalMinN = 2
globalMaxN = 200000
smallQueryMaxQ = 5
globalMaxQ = 100000

cubicMaxN = 200
cubicMaxQ = 200
quadraticMaxN = 2000
quadraticMaxQ = 2000


@indexify_testcases
def gen_random(minN, maxN, minQ, maxQ, increasing_H, fixed_start, fixed_end,
               testcase_index):
  print(f"gen random {minN} {maxN} {minQ} {maxQ} {increasing_H} {fixed_start} "
        f"{fixed_end} {testcase_index}")


@indexify_testcases
def gen_almost_increasing_H(minN, maxN, minQ, maxQ, swaps,
                            fixed_start, fixed_end, testcase_index):
  print(f"gen almost-increasing-H {minN} {maxN} {minQ} {maxQ} {swaps} "
        f"{fixed_start} {fixed_end} {testcase_index}")


@indexify_testcases
def gen_decreasing_increasing_H(minN, maxN, minQ, maxQ, fixed_start, fixed_end,
                               testcase_index):
  print(f"gen decreasing-increasing-H {minN} {maxN} {minQ} {maxQ} {fixed_start} "
        f"{fixed_end} {testcase_index}")


@indexify_testcases
def gen_zigzag(minN, maxN, minQ, maxQ, fixed_start, fixed_end, testcase_index):
  print(f"gen zigzag {minN} {maxN} {minQ} {maxQ} {fixed_start} {fixed_end} "
        f"{testcase_index}")


def common_testcases(minN, maxN, minQ, maxQ, increasing_H,
                     fixed_start, fixed_end, own_subtask=True):
  gen_random(minN, minN, minQ, minQ, increasing_H, fixed_start, fixed_end)
  for i in range(3 if own_subtask else 1):
    gen_random(minN, maxN, minQ, maxQ, increasing_H, fixed_start, fixed_end)
    gen_random(maxN, maxN, maxQ, maxQ, increasing_H, fixed_start, fixed_end)
  if not increasing_H:
    swaps_list = [0, 1, 2, maxN // 2] if own_subtask else [1, maxN // 2]
    for swaps in swaps_list:
      gen_almost_increasing_H(maxN, maxN, maxQ, maxQ, swaps,
                              fixed_start, fixed_end)
    gen_zigzag(maxN, maxN, maxQ, maxQ, fixed_start, fixed_end)
    for i in range(2 if own_subtask else 1):
      gen_decreasing_increasing_H(maxN, maxN, maxQ, maxQ,
                                  fixed_start, fixed_end)


class Testset():
  name = None

  def __init__(self):
    assert self.__class__.__name__.startswith(TESTSET_PREFIX)
    print("")
    print(f"@testset {self.__class__.__name__[len(TESTSET_PREFIX):]}")


class Subtask():
  name = None

  def __init__(self):
    print("")
    print(f"@subtask {self.name}")


class TestsetCubicSmallQuery(Testset):
  def __init__(self):
    super().__init__()
    common_testcases(globalMinN, cubicMaxN, 1, smallQueryMaxQ,
                     False, False, False, False)


class TestsetQuadraticSmallQuery(Testset):
  def __init__(self):
    super().__init__()
    common_testcases(cubicMaxN + 1, quadraticMaxN, 1, smallQueryMaxQ,
                     False, False, False, False)


class TestsetCubicFixedStartEnd(Testset):
  def __init__(self):
    super().__init__()
    manual("kill-wa-start-outside")
    common_testcases(globalMinN, cubicMaxN, 1, cubicMaxQ,
                     False, True, True, False)


class TestsetCubicFixedEnd(Testset):
  def __init__(self):
    super().__init__()
    common_testcases(globalMinN, cubicMaxN, 1, cubicMaxQ,
                     False, False, True, False)


class TestsetQuadraticFixedStartEnd(Testset):
  def __init__(self):
    super().__init__()
    common_testcases(cubicMaxN + 1, quadraticMaxN, 1, quadraticMaxQ,
                     False, True, True, False)


class TestsetQuadraticFixedEnd(Testset):
  def __init__(self):
    super().__init__()
    common_testcases(cubicMaxN + 1, quadraticMaxN, 1, quadraticMaxQ,
                     False, False, True, False)


class TestsetSmallQueryFixedStartEnd(Testset):
  def __init__(self):
    super().__init__()
    common_testcases(globalMinN, globalMaxN, 1, smallQueryMaxQ,
                     False, True, True, False)


class TestsetSmallQueryFixedEnd(Testset):
  def __init__(self):
    super().__init__()
    common_testcases(globalMinN, globalMaxN, 1, smallQueryMaxQ,
                     False, False, True, False)


class SubtaskSamples(Subtask):
  name = "samples"

  def __init__(self):
    super().__init__()
    manual("sample-1")


class SubtaskIncreasingH(Subtask):
  name = "increasing-H"

  def __init__(self):
    super().__init__()
    manual("smallest-case-increasing")
    common_testcases(globalMinN, globalMaxN, 1, globalMaxQ, True, False, False)


class SubtaskCubic(Subtask):
  name = "cubic"

  def __init__(self):
    super().__init__()
    include(SubtaskSamples)
    include(TestsetCubicSmallQuery)
    include(TestsetCubicFixedStartEnd)
    include(TestsetCubicFixedEnd)

    manual("smallest-case-increasing")
    manual("smallest-case-decreasing")
    common_testcases(globalMinN, cubicMaxN, 1, cubicMaxQ, False, False, False)


class SubtaskQuadratic(Subtask):
  name = "quadratic"

  def __init__(self):
    super().__init__()
    include(SubtaskCubic)
    include(TestsetQuadraticSmallQuery)
    include(TestsetQuadraticFixedStartEnd)
    include(TestsetQuadraticFixedEnd)

    common_testcases(cubicMaxN + 1, quadraticMaxN, 1, quadraticMaxQ,
                     False, False, False)


class SubtaskSmallQuery(Subtask):
  name = "small-query"

  def __init__(self):
    super().__init__()
    include(SubtaskSamples)
    include(TestsetCubicSmallQuery)
    include(TestsetQuadraticSmallQuery)
    include(TestsetSmallQueryFixedEnd)
    include(TestsetSmallQueryFixedStartEnd)

    manual("smallest-case-increasing")
    manual("smallest-case-decreasing")
    common_testcases(globalMinN, globalMaxN, 1, smallQueryMaxQ,
                     False, False, False)


class SubtaskFixedStartEnd(Subtask):
  name = "fixed-start-end"

  def __init__(self):
    super().__init__()
    include(TestsetCubicFixedStartEnd)
    include(TestsetQuadraticFixedStartEnd)
    include(TestsetSmallQueryFixedStartEnd)

    manual("smallest-case-increasing")
    manual("smallest-case-decreasing")
    common_testcases(globalMinN, globalMaxN, 1, globalMaxQ, False, True, True)


class SubtaskFixedEnd(Subtask):
  name = "fixed-end"

  def __init__(self):
    super().__init__()
    include(SubtaskFixedStartEnd)
    include(TestsetCubicFixedEnd)
    include(TestsetQuadraticFixedEnd)
    include(TestsetSmallQueryFixedEnd)

    common_testcases(globalMinN, globalMaxN, 1, globalMaxQ, False, False, True)


class SubtaskFull(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()
    include(SubtaskIncreasingH)
    include(SubtaskQuadratic)
    include(SubtaskSmallQuery)
    include(SubtaskFixedEnd)

    common_testcases(globalMinN, globalMaxN, 1, globalMaxQ, False, False, False)


def main():
  for testset in Testset.__subclasses__():
    testset()
  for subtask in Subtask.__subclasses__():
    subtask()

main()
