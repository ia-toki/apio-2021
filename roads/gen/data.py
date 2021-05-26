import math

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


smallW = 10
smallN = 200
mediumN = 2000
maxN = 100000
maxW = 1000000000

@indexify_testcases
def gen_random(minN, maxN, minW, maxW, testcase_index):
  print(f"gen random {minN} {maxN} {minW} {maxW} {testcase_index}")

@indexify_testcases
def gen_halving(minN, maxN, minW, maxW, testcase_index):
  print(f"gen halving {minN} {maxN} {minW} {maxW} {testcase_index}")

@indexify_testcases
def gen_distinct(minN, maxN, minW, maxW, testcase_index):
  print(f"gen distinct {minN} {maxN} {minW} {maxW} {testcase_index}")

@indexify_testcases
def gen_star(minN, maxN, minW, maxW, testcase_index):
  print(f"gen star {minN} {maxN} {minW} {maxW} {testcase_index}")

@indexify_testcases
def gen_shuffle_star(minN, maxN, minW, maxW, testcase_index):
  print(f"gen starshuffle {minN} {maxN} {minW} {maxW} {testcase_index}")

@indexify_testcases
def gen_line(minN, maxN, minW, maxW, testcase_index):
  print(f"gen line {minN} {maxN} {minW} {maxW} {testcase_index}")

@indexify_testcases
def gen_silkworm(minN, maxN, minW, maxW, testcase_index):
  print(f"gen silkworm {minN} {maxN} {minW} {maxW} {testcase_index}")

@indexify_testcases
def gen_degree(minN, maxN, minW, maxW, K, testcase_index):
  print(f"gen degree {minN} {maxN} {minW} {maxW} {K} {testcase_index}")

@indexify_testcases
def gen_mindegree(minN, maxN, minW, maxW, K, testcase_index):
  print(f"gen mindegree {minN} {maxN} {minW} {maxW} {K} {testcase_index}")

@indexify_testcases
def gen_knary(minN, maxN, minW, maxW, K, testcase_index):
  print(f"gen knary {minN} {maxN} {minW} {maxW} {K} {testcase_index}")

@indexify_testcases
def gen_flower(minN, maxN, minW, maxW, K, testcase_index):
  print(f"gen flower {minN} {maxN} {minW} {maxW} {K} {testcase_index}")

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

# -- Testsets Starts Here -- #
# sample_0, star_1, line_2, cubic_3, quadratic_4, constant-W_5, small-W_6, full_7

## Constraints

# * $2 \le N \le 100\,000$.
# * $0 \le U[i], V[i] \lt N$ (for all $0 \le i \le N - 2$).
# * It is possible to travel between any pair of junctions through the roads.
# * $1 \le W[i] \le 10^9$ (for all $0 \le i \le N - 2$).
# 
# ## Subtasks
# 
# 1. (? points) $U[i] = 0$ (for all $0 \le i \le N - 2$).
# 1. (? points) $U[i] = i$, $V[i] = i + 1$ (for all $0 \le i \le N - 2$).
# 1. (? points) $N \le 200$.
# 1. (? points) $N \le 2000$.
# 1. (? points) $W[i] = 1$ (for all $0 \le i \le N - 2$).
# 1. (? points) $W[i] \le 10$ (for all $0 \le i \le N - 2$).
# 1. (? points) No additional constraints.

class TestsetSamples(Testset):
  def __init__(self):
    super().__init__()
    manual("sample-1")
    manual("sample-2")


class TestsetUnweightedCorners(Testset):
  def __init__(self):
    super().__init__()
    gen_line(2, 2, 1, 1)

class TestsetCorners(Testset):
  def __init__(self):
    super().__init__()
    gen_line(2, 2, 2, smallW)

# 13467
class TestsetSmallWeightSmallStar(Testset):
  def __init__(self):
    super().__init__()
    gen_star(smallN//2, smallN, smallW//2, smallW)
    gen_star(smallN, smallN, 1, smallW)

# 1347
class TestsetSmallStar(Testset):
  def __init__(self):
    super().__init__()
    gen_star(smallN//2, smallN, maxW//2, maxW)
    gen_star(smallN, smallN, 1, maxW)
    gen_star(smallN, smallN, maxW, maxW)

# 1467
class TestsetSmallWeightMediumStar(Testset):
  def __init__(self):
    super().__init__()
    gen_star(mediumN//2, mediumN, smallW//2, smallW)
    gen_star(mediumN, mediumN, 1, smallW)

# 147
class TestsetMediumStar(Testset):
  def __init__(self):
    super().__init__()
    gen_star(3 * mediumN//4, mediumN, maxW//2, maxW)
    gen_star(mediumN, mediumN, maxW//4, maxW)
    gen_star(mediumN, mediumN, maxW, maxW)

# 167
class TestsetSmallWeightStar(Testset):
  def __init__(self):
    super().__init__()
    gen_star(maxN//2, maxN, smallW//2, smallW)
    gen_star(maxN, maxN, 1, smallW)

# 1567
class TestsetUnweightedStar(Testset):
  def __init__(self):
    super().__init__()
    gen_star(9 * maxN//10, maxN, 1, 1)

# 17
class TestsetStar(Testset):
  def __init__(self):
    super().__init__()
    gen_star(3 * maxN//4, maxN, maxW//2, maxW)
    gen_star(3 * maxN//4, maxN, 1, maxW)
    gen_star(maxN, maxN, maxW//3, maxW)
    gen_star(maxN, maxN, maxW, maxW)

# 2347
class TestsetSmallLine(Testset):
  def __init__(self):
    super().__init__()
    gen_line(smallN//2, smallN, maxW//2, maxW)
    gen_line(smallN * 2//3, smallN, 1, maxW)
    gen_line(smallN, smallN, maxW, maxW)

# 247
class TestsetMediumLine(Testset):
  def __init__(self):
    super().__init__()
    gen_line(3 * mediumN//4, mediumN, maxW//2, maxW)
    gen_line(mediumN, mediumN, maxW//4, maxW)
    gen_line(mediumN * 2 // 3, mediumN, maxW, maxW)

# 257
class TestsetUnweightedLine(Testset):
  def __init__(self):
    super().__init__()
    gen_line(maxN//2, maxN - 1, 1, 1)
    gen_line(maxN, maxN, 1, 1)

# 267
class TestsetSmallWeightLine(Testset):
  def __init__(self):
    super().__init__()
    gen_line(maxN//2, maxN, smallW//2, smallW)
    gen_line(maxN, maxN, 1, smallW)

# 27
class TestsetLine(Testset):
  def __init__(self):
    super().__init__()
    gen_line(3 * maxN//4, maxN, maxW//2, maxW)
    gen_line(3 * maxN//4, maxN, 1, maxW)
    gen_line(maxN, maxN, maxW//3, maxW)
    gen_line(maxN, maxN, maxW, maxW)

# 3467
class TestsetSmallWeightSmall(Testset):
  def __init__(self):
    super().__init__()
    gen_random(smallN//2, smallN, smallW//2, smallW)
    gen_random(smallN, smallN, 1, smallW)
    gen_flower(smallN, smallN, 7, smallW, int(math.sqrt(smallN)))

# 347
class TestsetSmall(Testset):
  def __init__(self):
    super().__init__()
    gen_random(smallN//2, smallN, maxW//2, maxW)
    gen_random(smallN, smallN, 1, maxW)
    gen_knary(9 * smallN // 10, smallN, "min", 0, 20)
    gen_degree(9 * smallN // 10, smallN, 1, maxW, 40)
    gen_degree(smallN, smallN, maxW, maxW, 40)
    gen_flower(smallN, smallN, 9 * maxW//10, maxW, int(math.sqrt(smallN)))

# 4567
class TestsetUnweightedMedium(Testset):
  def __init__(self):
    super().__init__()
    gen_random(3 * mediumN//4, mediumN, 1, 1)
    gen_random(mediumN, mediumN, 1, 1)
    gen_distinct(mediumN, mediumN, 1, 1)
    gen_halving(mediumN, mediumN, 1, 1)
    gen_mindegree(mediumN, mediumN, 1, 1, int(math.sqrt(mediumN)))

# 467
class TestsetSmallWeightMedium(Testset):
  def __init__(self):
    super().__init__()
    gen_random(mediumN//2, mediumN, smallW//2, smallW)
    gen_random(mediumN, mediumN, 1, smallW)
    gen_knary(mediumN, mediumN, 1, smallW, int(math.sqrt(maxN)))

# 47
class TestsetMedium(Testset):
  def __init__(self):
    super().__init__()
    gen_random(mediumN//2, mediumN, maxW//2, maxW)
    gen_random(mediumN, mediumN, 1, maxW)
    gen_random(3 * mediumN // 4, mediumN, "sum", 1)
    gen_flower(mediumN, mediumN, "sum", 2, int(math.sqrt(mediumN)))
    gen_knary(mediumN, mediumN, maxW//2, maxW, int(math.sqrt(maxN)))
    gen_knary(9 * mediumN // 10, mediumN, "min", 0, int(math.sqrt(maxN)) + 15)
    gen_degree(9 * mediumN // 10, mediumN, 1, maxW, 300)
    gen_mindegree(mediumN, mediumN, maxW * 9 // 10, maxW, int(math.sqrt(mediumN)))
    gen_distinct(mediumN, mediumN, maxW - 10, maxW)
    gen_halving(mediumN, mediumN, maxW, maxW)

# 567
class TestsetUnweighted(Testset):
  def __init__(self):
    super().__init__()
    gen_random(3 * maxN//4, maxN, 1, 1)
    gen_random(3 * maxN//4, maxN, 1, 1)
    gen_mindegree(maxN, maxN, 1, 1, int(math.sqrt(maxN)))
    gen_random(maxN, maxN, 1, 1)
    gen_knary(maxN, maxN, 1, 1, int(math.sqrt(maxN)))
    gen_knary(maxN, maxN, 1, 1, 2000)
    gen_flower(maxN, maxN, 1, 1, int(math.sqrt(maxN)))
    gen_shuffle_star(9 * maxN//10, maxN, 1, 1)
    gen_silkworm(9 * maxN//10, maxN, 1, 1)
    gen_degree(9 * maxN//10, maxN, 1, 1, 300)
    gen_distinct(maxN, maxN, 1, 1)
    gen_halving(maxN, maxN, 1, 1)

# 67
class TestsetSmallWeight(Testset):
  def __init__(self):
    super().__init__()
    gen_random(3 * maxN//4, maxN, smallW//2, smallW)
    gen_random(3 * maxN//4, maxN, 1, smallW)
    gen_random(maxN, maxN, 1, smallW)
    gen_mindegree(maxN, maxN, 1, smallW, int(math.sqrt(maxN)))
    gen_knary(9 * maxN//10, maxN, smallW//2, smallW, int(math.sqrt(maxN)))
    gen_knary(9 * maxN//10, maxN, 1, smallW, 2000)
    gen_knary(9 * maxN//10, maxN, 1, smallW, 12000)
    gen_degree(9 * maxN//10, maxN, 1, 3, 10000)
    gen_shuffle_star(9 * maxN//10, maxN, 1, smallW)
    gen_flower(maxN, maxN, 1, smallW, int(math.sqrt(maxN)))
    gen_silkworm(3 * maxN//4, maxN, 7, smallW)
    gen_distinct(maxN - 10, maxN, 1, smallW)
    gen_halving(maxN - 10, maxN, 1, smallW)

# 7
class TestsetFull(Testset):
  def __init__(self):
    super().__init__()
    gen_random(3 * maxN//4, maxN, maxW//2, maxW)
    gen_random(3 * maxN//4, maxN, 1, maxW)
    gen_mindegree(maxN, maxN, "min", 5, int(math.sqrt(maxN)))
    gen_knary(9 * maxN//10, maxN, 1, maxW, int(math.sqrt(maxN)))
    gen_knary(maxN, maxN, maxW, maxW, int(math.sqrt(maxN)) - 3)
    gen_knary(maxN, maxN, "sum", 1, int(math.sqrt(maxN)) + 200)
    gen_flower(maxN, maxN, "sum", 2, int(math.sqrt(maxN)))
    gen_knary(9 * maxN//10, maxN, maxW // 2, maxW, 2200)
    gen_knary(9 * maxN//10, maxN, maxW // 2, maxW, 8000)
    gen_knary(9 * maxN//10, maxN, maxW // 2, maxW, 12000)
    gen_shuffle_star(9 * maxN//10, maxN, maxW // 2, maxW)
    gen_random(9 * maxN//10, maxN, "sum", 2)
    gen_degree(9 * maxN//10, maxN, maxW - 3, maxW, 400)
    gen_distinct(maxN - 10, maxN, 9*maxW//10, maxW)
    gen_distinct(maxN, maxN, "min", 0)
    gen_halving(maxN - 10, maxN, 9*maxW//10, maxW)
    gen_halving(maxN - 10, maxN, 1, maxW)
    gen_distinct(maxN, maxN, maxW, maxW)

# -- Testsets Ends Here -- #

class SubtaskSamples(Subtask):
  name = "samples"

  def __init__(self):
    super().__init__()
    include(TestsetSamples)

class SubtaskStar(Subtask):
  name = "star"

  def __init__(self):
    super().__init__()
    include(TestsetCorners)
    include(TestsetUnweightedCorners)
    include(TestsetSmallWeightSmallStar)
    include(TestsetSmallWeightMediumStar)
    include(TestsetSmallWeightStar)

    include(TestsetSmallStar)
    include(TestsetMediumStar)
    include(TestsetStar)

    include(TestsetUnweightedStar)

class SubtaskLine(Subtask):
  name = "line"

  def __init__(self):
    super().__init__()
    include(TestsetCorners)
    include(TestsetUnweightedCorners)
    include(TestsetSmallWeightLine)
    
    include(TestsetSmallLine)
    include(TestsetMediumLine)
    include(TestsetUnweightedLine)
    include(TestsetLine)

class SubtaskCubic(Subtask):
  name = "cubic"

  def __init__(self):
    super().__init__()
    include(TestsetCorners)
    include(TestsetUnweightedCorners)
    include(TestsetSamples)

    include(TestsetSmallWeightSmallStar)
    include(TestsetSmallStar)

    include(TestsetSmallLine)
    
    include(TestsetSmallWeightSmall)
    include(TestsetSmall)

class SubtaskQuadratic(Subtask):
  name = "quadratic"

  def __init__(self):
    super().__init__()
    include(SubtaskCubic)
    
    include(TestsetSmallWeightMediumStar)
    include(TestsetMediumStar)
    include(TestsetMediumLine)

    include(TestsetUnweightedMedium)
    include(TestsetSmallWeightMedium)
    include(TestsetMedium)

class SubtaskConstant_W(Subtask):
  name = "constant-W"

  def __init__(self):
    super().__init__()
    include(TestsetUnweightedCorners)
    include(TestsetUnweightedLine)
    include(TestsetUnweightedStar)
    include(TestsetUnweightedMedium)
    include(TestsetUnweighted)

class SubtaskSmall_W(Subtask):
  name = "small-W"

  def __init__(self):
    super().__init__()
    include(TestsetSamples)
    include(TestsetCorners)
    include(SubtaskConstant_W)


    include(TestsetSmallWeightSmallStar)
    include(TestsetSmallWeightMediumStar)

    include(TestsetSmallWeightStar)
    include(TestsetSmallWeightLine)

    include(TestsetSmallWeightSmall)

    include(TestsetSmallWeightMedium)
    include(TestsetSmallWeight)

class SubtaskFull(Subtask):
  name = "full"

  def __init__(self):
    super().__init__()
    include(SubtaskStar)
    include(SubtaskLine)
    include(SubtaskQuadratic)
    include(SubtaskSmall_W)
    include(TestsetFull)

    

def main():
  for testset in Testset.__subclasses__():
    testset()
  for subtask in Subtask.__subclasses__():
    subtask()

main()
