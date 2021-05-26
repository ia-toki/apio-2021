#include "testlib.h"

#include <assert.h>
#include <string.h>

#include <algorithm>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

const int kMaxN = 200000;
const int kMaxA = 1000000000;
const int kMaxB = 1000000000;
const int kMaxSumL = 1000000000;

const int dirx[] = {0, 0, 1, 1, 0, -1, -1};
const int diry[] = {0, 1, 0, -1, -1, 0, 1};

std::pair<int, int> move(std::pair<int, int> pos, int D, int L) {
  pos.first += dirx[D] * L;
  pos.second += diry[D] * L;
  return pos;
}

namespace is_closed {

bool is_closed(int N, const std::vector<int>& D, const std::vector<int>& L) {
  std::pair<int, int> current_pos = std::make_pair(0, 0);

  for (int i = 0; i < N; ++i) {
    current_pos = move(current_pos, D[i], L[i]);
  }

  return current_pos == std::make_pair(0, 0);
}

}  // namespace is_closed

namespace is_simple {

int current_x = 0;

struct Line {
  std::pair<int, int> start, end;
  int index;

  Line(std::pair<int, int> start,
       std::pair<int, int> end,
       int index): start(start), end(end), index(index) {
    assert(start.first < end.first);
  }

  int get_y(int x) const {
    if (end.second < start.second) {
      return start.second - (x - start.first);
    }
    if (end.second == start.second) {
      return start.second;
    }
    assert(false);
  }

  int get_y() const {
    return get_y(current_x);
  }

  bool operator<(const Line& other) const {
    return std::make_pair(get_y(), index)
         < std::make_pair(other.get_y(), other.index);
  }

  bool intersect(const Line& other) const {
    int end_x = std::min(end.first, other.end.first);
  
    if (get_y() < other.get_y()) {
      return get_y(end_x) >= other.get_y(end_x);
    } else if (get_y() > other.get_y()) {
      return get_y(end_x) <= other.get_y(end_x);
    } else {
      return true;
    }
  }
};

bool is_simple(int N, const std::vector<int>& D, const std::vector<int>& L) {
  std::map<int, std::vector<Line>> in_lines, out_lines;
  std::map<int, std::vector<std::pair<int, int>>> vertical_lines;
  std::vector<int> xs;
  std::set<Line> active_lines;

  std::pair<int, int> current_pos = std::make_pair(0, 0);
  for (int i = 0; i < N; ++i) {
    std::pair<int, int> first_endpoint = move(current_pos, D[i], 1);
    
    current_pos = move(current_pos, D[i], L[i]);
    std::pair<int, int> second_endpoint = current_pos;

    if (first_endpoint > second_endpoint) {
      swap(first_endpoint, second_endpoint);
    }

    if (first_endpoint.first == second_endpoint.first) {
      vertical_lines[first_endpoint.first].emplace_back(first_endpoint.second,
                                                        second_endpoint.second);
    } else {
      in_lines[first_endpoint.first].emplace_back(
          first_endpoint, second_endpoint, i);
      out_lines[second_endpoint.first].emplace_back(
          first_endpoint, second_endpoint, i);
    }
    xs.push_back(first_endpoint.first);
    xs.push_back(second_endpoint.first);
  }
  std::sort(xs.begin(), xs.end());
  xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
  for (int x : xs) {
    current_x = x;
    if (in_lines.count(x) > 0) {
      for (const Line& line : in_lines[x]) {
        std::set<Line>::iterator it = active_lines.insert(line).first;
        if (it != active_lines.begin()) {
          std::set<Line>::iterator prev = std::prev(it);
          if (line.intersect(*prev)) {
            return false;
          }
        }
        std::set<Line>::iterator next = std::next(it);
        if (next != active_lines.end() && line.intersect(*next)) {
          return false;
        }
      }
    }
    if (vertical_lines.count(x) > 0) {
      std::vector<std::pair<int, int>> lines = vertical_lines[x];
      std::sort(lines.begin(), lines.end());
      for (int i = 0; i < static_cast<int>(lines.size()) - 1; ++i) {
        if (lines[i + 1].first <= lines[i].second) {
          // Two vertical lines intersect.
          return false;
        }
      }
      for (const std::pair<int, int>& line : lines) {
        assert(line.first <= line.second);
        Line bottom_line(std::make_pair(INT_MIN, line.first),
                         std::make_pair(INT_MAX, line.first),
                         INT_MIN);
        Line top_line(std::make_pair(INT_MIN, line.second),
                      std::make_pair(INT_MAX, line.second),
                      INT_MAX);
        if (active_lines.lower_bound(bottom_line)
            != active_lines.lower_bound(top_line)) {
          return false;
        }
      }
    }
    if (out_lines.count(x) > 0) {
      for (const Line& line : out_lines[x]) {
        std::set<Line>::iterator it = active_lines.find(line);
        if (it != active_lines.begin() && std::next(it) != active_lines.end()) {
          if (std::prev(it)->intersect(*std::next(it))) {
            return false;
          }
        }
        active_lines.erase(it);
      }
    }
  }
  return true;
}

}  // namespace is_simple

namespace is_exposed {

namespace path {

std::map<int, std::vector<int>> same_x, same_y, same_x_plus_y;

void preprocess() {
  for (auto & [k, v] : same_x) {
    sort(v.begin(), v.end());
  }
  for (auto & [k, v] : same_y) {
    sort(v.begin(), v.end());
  }
  for (auto & [k, v] : same_x_plus_y) {
    sort(v.begin(), v.end());
  }
}

bool is_inside(const std::vector<int>& v, int x) {
  return (std::upper_bound(v.begin(), v.end(), x) - v.begin()) & 1;
}

bool in_path(int x, int y) {
  if (same_x.count(x) > 0) {
    if (is_inside(same_x[x], y)) {
      return true;
    }
  }
  if (same_y.count(y) > 0) {
    if (is_inside(same_y[y], x)) {
      return true;
    }
  }
  if (same_x_plus_y.count(x + y) > 0) {
    if (is_inside(same_x_plus_y[x + y], x - y)) {
      return true;
    }
  }
  return false;
}

bool in_path(std::pair<int, int> position) {
  return in_path(position.first, position.second);
}

}  // namespace path

namespace important_point {

std::map<int, std::vector<int>> same_x, same_y, same_x_plus_y;

void preprocess() {
  for (auto & [k, v] : same_x) {
    sort(v.begin(), v.end());
  }
  for (auto & [k, v] : same_y) {
    sort(v.begin(), v.end());
  }
  for (auto & [k, v] : same_x_plus_y) {
    sort(v.begin(), v.end());
  }
}

int higher(const std::vector<int> &v, int x) {
  return *(std::upper_bound(v.begin(), v.end(), x));
}

int lower(const std::vector<int> &v, int x) {
  return *(std::prev(std::lower_bound(v.begin(), v.end(), x)));
}

std::pair<int, int> neighbour(std::pair<int, int> pos, int d) {
  if (d == 1) {
    return std::make_pair(pos.first, higher(same_x[pos.first], pos.second));
  }
  if (d == 2) {
    return std::make_pair(higher(same_y[pos.second], pos.first), pos.second);
  }
  if (d == 3) {
    int x_plus_y = pos.first + pos.second;
    int x_minus_y = higher(same_x_plus_y[x_plus_y], pos.first - pos.second);
    return std::make_pair((x_plus_y + x_minus_y) / 2,
                          (x_plus_y - x_minus_y) / 2);
  }
  if (d == 4) {
    return std::make_pair(pos.first, lower(same_x[pos.first], pos.second));
  }
  if (d == 5) {
    return std::make_pair(lower(same_y[pos.second], pos.first), pos.second); 
  }
  if (d == 6) {
    int x_plus_y = pos.first + pos.second;
    int x_minus_y = lower(same_x_plus_y[x_plus_y], pos.first - pos.second);
    return std::make_pair((x_plus_y + x_minus_y) / 2,
                          (x_plus_y - x_minus_y) / 2); 
  }
  assert(false);
}

int distance(std::pair<int, int> pos, std::pair<int, int> nxt) {
  if (pos.first == nxt.first || pos.second == nxt.second) {
    return std::abs(pos.first - nxt.first) + std::abs(pos.second - nxt.second);
  }
  return (
      std::abs(pos.first - nxt.first) + std::abs(pos.second - nxt.second)) / 2;
}

}  // namespace important_point

std::set<std::pair<int, int>> important_points;

bool is_exposed(int N, const std::vector<int>& D, const std::vector<int>& L) {
  std::pair<int, int> current_pos = std::make_pair(0, 0);
  for (int i = 0; i < N; ++i) {
    std::pair<int, int> first_endpoint = current_pos;
    
    current_pos = move(current_pos, D[i], L[i]);
    std::pair<int, int> second_endpoint = current_pos;

    int add = 0;
    if (first_endpoint > second_endpoint) {
      std::swap(first_endpoint, second_endpoint);
      add = 1;
    }
    if (first_endpoint.first == second_endpoint.first) {
      path::same_x[first_endpoint.first].push_back(
          first_endpoint.second + add);
      path::same_x[second_endpoint.first].push_back(
          second_endpoint.second + add);
    } else if (first_endpoint.second == second_endpoint.second) {
      path::same_y[first_endpoint.second].push_back(
          first_endpoint.first + add);
      path::same_y[second_endpoint.second].push_back(
          second_endpoint.first + add);
    } else {
      path::same_x_plus_y[
          first_endpoint.first + first_endpoint.second].push_back(
              first_endpoint.first - first_endpoint.second + add);
      path::same_x_plus_y[
          second_endpoint.first + second_endpoint.second].push_back(
              second_endpoint.first - second_endpoint.second + add);
    }
    important_points.insert(current_pos);
  }

  path::preprocess();

  std::vector<std::pair<int, int>> initial_important_points(
      important_points.begin(), important_points.end());
  for (const std::pair<int, int>& point : initial_important_points) {
    for (int d = 1; d <= 6; ++d) {
      std::pair<int, int> neighbour = move(point, d, 1);
      if (path::in_path(neighbour)) {
        important_points.insert(neighbour);
      }
    }
  }

  for (const std::pair<int, int>& point : important_points) {
    important_point::same_x[point.first].push_back(point.second);
    important_point::same_y[point.second].push_back(point.first);
    important_point::same_x_plus_y[point.first + point.second].push_back(
        point.first - point.second);
  }
  important_point::preprocess();

  int sum_path = 0;
  int current_dir = 1;
  std::pair<int, int> begin_pos = *(important_points.begin());
  current_pos = begin_pos;
  do {
    std::pair<int, int> next_pos = current_pos;
    for (int i = 0; i < 5; ++i) {
      if (path::in_path(move(current_pos, current_dir, 1))) {
        next_pos = important_point::neighbour(current_pos, current_dir);
        break;
      }
      current_dir = (current_dir == 6 ? 1 : current_dir + 1);
    }
    assert(next_pos != current_pos);

    sum_path += important_point::distance(current_pos, next_pos);
    current_pos = next_pos;
    current_dir += 4;
    if (current_dir > 6) {
      current_dir -= 6;
    }
  } while (current_pos != begin_pos);
  return sum_path == std::accumulate(L.begin(), L.end(), 0);
}

}  // namespace is_exposed

int main(int, char *argv[]) {
  registerValidation();
  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"4279751dd9ece9095f11fb277ecbef2e68be1cdf",
          "Secret not found!");

  const char* subtask_name = argv[1];

  int maxN = kMaxN;
  int maxB = kMaxB;
  int maxSumL = kMaxSumL;
  if (strcmp(subtask_name, "B-equals-zero-triangle") == 0) {
    maxN = 3;
    maxB = 0;
  } else if (strcmp(subtask_name, "triangle") == 0) {
    maxN = 3;
  } else if (strcmp(subtask_name, "quadratic-sum-L") == 0) {
    maxSumL = 2000;
  } else if (strcmp(subtask_name, "B-equals-zero-small") == 0) {
    maxB = 0;
    maxSumL = 200000;
  } else if (strcmp(subtask_name, "B-equals-zero") == 0) {
    maxB = 0;
  } else if (strcmp(subtask_name, "linear-sum-L") == 0) {
    maxSumL = 200000;
  }
  
  int N = inf.readInt(3, maxN, "N");
  inf.readSpace();
  inf.readInt(0, kMaxA, "A");
  inf.readSpace();
  inf.readInt(0, maxB, "B");
  inf.readEoln();

  std::vector<int> D(N), L(N);
  for (int i = 0; i < N; ++i) {
    D[i] = inf.readInt(1, 6, "D_i");
    inf.readSpace();
    L[i] = inf.readInt(1, maxSumL, "L_i");
    inf.readEoln();
  }

  ensuref(std::accumulate(L.begin(), L.end(), 0LL) <= maxSumL,
          "Sum of L exceeds %d", maxSumL);

  if (strcmp(subtask_name, "constant-L") == 0) {
    ensuref(std::all_of(L.begin(), L.end(), [&](int l) { return l == L[0]; }),
            "The values L is not constant");
  }

  ensuref(is_closed::is_closed(N, D, L), "The path is not closed");
  ensuref(is_simple::is_simple(N, D, L), "The path is not simple");
  ensuref(is_exposed::is_exposed(N, D, L), "The path is not exposed");

  inf.readEof();
  return 0;
}
