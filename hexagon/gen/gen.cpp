#include "testlib.h"
#include <bits/stdc++.h>

using namespace std;

struct Point {
  int x, y;
  Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
  Point operator+(Point p) const { return Point(x+p.x, y+p.y); }
  Point operator-(Point p) const { return Point(x-p.x, y-p.y); }
  Point operator*(int s) const { return Point(x * s, y * s); }
  bool operator==(Point p) const { return x == p.x && y == p.y; }
  bool operator!=(Point p) const { return !(*this == p); }
  bool operator<(Point p) const { return x == p.x ? y < p.y : x < p.x; }
};

ostream& operator<<(ostream& os, Point p) {
  return os << "(" << p.x << ", " << p.y << ")";
}


constexpr int mod = 1e9 + 7;
constexpr int kDirs = 6;
const vector<Point> pdirs = { Point(0, 1), Point(1, 1), Point(1, 0),
                              Point(0, -1), Point(-1, -1), Point(-1, 0) };

struct Test {
  int A, B;
  vector<int> D, L;

  Test(int _A = 0, int _B = 0, vector<Point> points = {}) : A(_A), B(_B) {
    from_points(points);
  }

  void print() {
    int N = D.size();
    printf("%d %d %d\n", N, A, B);
    for (int i = 0; i < N; ++i) {
      printf("%d %d\n", D[i] + 1, L[i]);
    }
  }

  void add_move(int dir, int len) {
    D.push_back(dir);
    L.push_back(len);
  }

  long long sumL() {
    return accumulate(L.begin(), L.end(), 0LL);
  }

  Test& from_points(vector<Point>& points) {
    int N = points.size();
    D.resize(N);
    L.resize(N);
    for (int i = 0; i < N; ++i) {
      Point nxt = i + 1 < N ? points[i + 1] : points[0];
      Point diff = nxt - points[i];
      L[i] = max(abs(diff.x), abs(diff.y));
      D[i] = -1;
      for (int j = 0; j < kDirs; ++j) {
        if ((points[i] + pdirs[j] * L[i]) == nxt) {
          assert(D[i] == -1);
          D[i] = j;
        }
      }
      assert(D[i] != -1);
    }
    return *this;
  }

  Test& rotate(int x) {
    x %= kDirs;
    for (int & d : D) {
      d = (d + x) % kDirs;
      if (d < 0)
        d += kDirs;
    }
    return *this;
  }

  Test& scale(int x) {
    for (int & l : L) {
      l *= x;
    }
    return *this;
  }

  Test& shift(int x) {
    auto shift_vector = [](vector<int>& v, int s) {
      vector<int> tmp(v.size());
      int size = v.size();
      for (int i = 0, j = s; i < size; ++i, ++j) {
        if (j >= size)
          j -= size;
        tmp[j] = v[i];
      }
      v.swap(tmp);
    };
    int N = D.size();
    x %= N;
    if (x < 0) x += N;
    shift_vector(L, x);
    shift_vector(D, x);
    return *this;
  }

  Test& reverse() {
    std::reverse(L.begin(), L.end());
    std::reverse(D.begin(), D.end());
    return rotate(3);
  }

  Test& random_shift() {
    return shift(rnd.next(D.size()));
  }

  Test& random_reverse() {
    if (rnd.next(2)) {
      return reverse();
    } else {
      return *this;
    }
  }

  Test& compress() {
    int last = 0;
    int N = D.size();
    for (int i = 1; i < N; ++i) {
      if (D[i] == D[last]) {
        L[last] += L[i];
      } else {
        ++last;
        D[last] = D[i];
        L[last] = L[i];
      }
    }
    D.resize(last+1);
    L.resize(last+1);
    return *this;
  }

  Test& make_uniform() {
    compress();
    int g = accumulate(L.begin(), L.end(), 0, [](int a, int b) {
      while (a) {
        b %= a;
        swap(a, b);
      }
      return b;
    });
    vector<int> tmpD;
    int N = L.size();
    for (int i = 0; i < N; ++i) {
      int cnt = L[i] / g;
      for (int j = 0; j < cnt; ++j) {
        tmpD.push_back(D[i]);
      }
    }
    D.swap(tmpD);
    L.assign(D.size(), g);
    return *this;
  }
};

vector<Point> find_boundary_from_points(vector<Point> nodes) {
  sort(nodes.begin(), nodes.end());
  nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
  vector<vector<int>> graph(nodes.size(), vector<int>(6, -1));
  auto find_edges = [&](int dir, vector<int> ids,
                        function<bool(Point, Point)> cmp) {
    sort(ids.begin(), ids.end(), [&](int le, int ri) {
      return cmp(nodes[le], nodes[ri]);
    });
    for (size_t i = 0; i + 1 < ids.size(); ++i) {
      int u = ids[i], v = ids[i+1];
      Point pu = nodes[u], pv = nodes[v];
      if ((pu + pdirs[dir]) == pv) {
        graph[u][dir] = v;
        graph[v][dir+3] = u;
      }
    }
  };
  auto cmp_vertical = [](Point a, Point b) {
    return a.x == b.x ? a.y < b.y : a.x < b.x;
  };
  auto cmp_diagonal = [](Point a, Point b) {
    int va = a.x - a.y, vb = b.x - b.y;
    if (va != vb)
      return va < vb;
    return a.x + a.y < b.x + b.y;
  };
  auto cmp_horizontal = [](Point a, Point b) {
    return a.y == b.y ? a.x < b.x : a.y < b.y;
  };
  vector<int> ids(nodes.size());
  iota(ids.begin(), ids.end(), 0);
  find_edges(0, ids, cmp_vertical);
  find_edges(1, ids, cmp_diagonal);
  find_edges(2, ids, cmp_horizontal);
  int v = min_element(nodes.begin(), nodes.end()) - nodes.begin();
  int dir = 3;
  vector<Point> result;
  int start_v = v, start_dir = dir;
  do {
    if (graph[v][dir] == -1) {
      --dir;
      if (dir < 0) dir += kDirs;
      continue;
    }
    v = graph[v][dir];
    result.push_back(nodes[v]);
    dir += 2;
    if (dir >= kDirs)
      dir -= kDirs;
  } while (v != start_v || dir != start_dir);
  return result;
}

Test triangle(int A, int B, int dir1, int dir2, int dir3, int len) {
  Test t(A, B);
  t.D = {dir1, dir2, dir3};
  t.L.assign(3, len);
  return t;
}

Test random_linear_traversal(int A, int B, int maxN, int maxSumL, int constantL,
                     int maxCnt, int maxLen) {
  if (maxN == 3)
    maxCnt = 3;
  set<Point> points;
  priority_queue<pair<int, Point>> pq;
  auto add_point = [&](Point p) {
    if (points.count(p)) return;
    int priority = rnd.next(INT_MAX);
    points.insert(p);
    pq.emplace(priority, p);
  };
  add_point(Point(0, 0));
  add_point(Point(1, 0));
  add_point(Point(1, 1));
  while (!pq.empty() && (int)points.size() < maxCnt) {
    Point p = pq.top().second;
    pq.pop();
    int dir = rnd.next(kDirs);
    Point q;
    bool found = false;
    for (int i = 0; i < kDirs; ++i) {
      if (((i - dir) % 3) == 0) continue;
      q = p + pdirs[i];
      if (points.count(q)) {
        found = true;
        break;
      }
    }
    assert(found);
    int len = rnd.next(maxLen+1);
    Point pmove = pdirs[dir];
    for (int i = 0; i < len; ++i) {
      p = p + pmove;
      q = q + pmove;
      add_point(p);
      add_point(q);
    }
  }
  vector<Point> boundary = find_boundary_from_points(
      vector<Point>(points.begin(), points.end()));
  Test T(A, B, boundary);
  int sumL = T.sumL();
  int maxScale = max(maxSumL / sumL, 1);
  int scale = rnd.next((maxScale + 1) / 2, maxScale);
  T.random_shift().compress().scale(scale);
  if (constantL) {
    T.make_uniform().random_shift();
  }
  return T.random_reverse();
}


Test dfs_linear_traversal(int A, int B, int maxN, int maxSumL, int constantL,
                     int maxCnt, int maxLen, int rotate) {
  if (maxN == 3)
    maxCnt = 3;
  set<Point> points;
  priority_queue<pair<int, Point>> pq;
  auto add_point = [&](Point p) {
    if (points.count(p)) return;
    int priority = points.size();
    points.insert(p);
    pq.emplace(priority, p);
  };
  add_point(Point(0, 0));
  add_point(Point(1, 0));
  add_point(Point(1, 1));
  while (!pq.empty() && (int)points.size() < maxCnt) {
    Point p = pq.top().second;
    pq.pop();
    int dir = rnd.next(kDirs);
    Point q;
    bool found = false;
    for (int i = 0; i < kDirs; ++i) {
      if (((i - dir) % 3) == 0) continue;
      q = p + pdirs[i];
      if (points.count(q)) {
        found = true;
        break;
      }
    }
    assert(found);
    int len = rnd.next(maxLen+1);
    Point pmove = pdirs[dir];
    for (int i = 0; i < len; ++i) {
      p = p + pmove;
      q = q + pmove;
      add_point(p);
      add_point(q);
    }
  }
  vector<Point> boundary = find_boundary_from_points(
      vector<Point>(points.begin(), points.end()));
  Test T(A, B, boundary);
  int sumL = T.sumL();
  int maxScale = max(maxSumL / sumL, 1);
  int scale = rnd.next((maxScale + 1) / 2, maxScale);
  T.random_shift().compress().scale(scale);
  if (constantL) {
    T.make_uniform().random_shift();
  }
  return T.random_reverse().rotate(rotate);
}

Point intersect_ver_diag(int x, int x_minus_y) {
  return Point(x, x - x_minus_y);
}

Point intersect_ver_hor(int x, int y) {
  return Point(x, y);
}

Point intersect_hor_diag(int y, int x_minus_y) {
  return Point(x_minus_y + y, y);
}

Point move_point_to_hor(Point p, int dir, int hor_y) {
  if (dir == 0 || dir == 3)
    return intersect_ver_hor(p.x, hor_y);
  else if (dir == 1 || dir == 4)
    return intersect_hor_diag(hor_y, p.x - p.y);
  else
    assert(false);
}

Point move_point_to_ver(Point p, int dir, int ver_x) {
  if (dir == 2 || dir == 5)
    return intersect_ver_hor(ver_x, p.y);
  else if (dir == 1 || dir == 4)
    return intersect_ver_diag(ver_x, p.x - p.y);
  else
    assert(false);
}

Point move_point_to_diag(Point p, int dir, int diag_x_minus_y) {
  if (dir == 0 || dir == 3)
    return intersect_ver_diag(p.x, diag_x_minus_y);
  else if (dir == 2 || dir == 5)
    return intersect_hor_diag(p.y, diag_x_minus_y);
  else
    assert(false);
}

int distance(Point p, Point q) {
  if (p.x == q.x)
    return abs(p.y - q.y);
  else
    return abs(p.x - q.x);
}

Test spiral(int A, int B, int maxN, int maxSumL, int constantL, int more_space,
            int rotate) {
  Point pin(1, 0), pout(0, 0);
  vector<Point> vpin, vpout;
  int ymin = 0, ymax = 0;
  int xmin = 0, xmax = 0;
  int x_y_min = 0, x_y_max = 0;
  auto add = [&](Point p) {
    ymin = min(ymin, p.y);
    ymax = max(ymax, p.y);
    xmin = min(xmin, p.x);
    xmax = max(xmax, p.x);
    x_y_min = min(x_y_min, p.x - p.y);
    x_y_max = max(x_y_max, p.x - p.y);
  };
  int cnt = (maxN - 2) / 2;
  int limitSumL = maxSumL;
  if (constantL) {
    limitSumL = maxN;
  }
  add(pin);
  add(pout);
  vpin.push_back(pin);
  vpout.push_back(pout);
  int totalL = distance(pin, pout);
  for (int i = 0; i < cnt; ++i) {
    int dir = i % kDirs;
    int free_space = 2 + rnd.next(0, more_space);
    int filled_space = 1 + rnd.next(0, more_space);
    if (i + 1 == cnt) {
      filled_space = 0;
    }
    Point prev_pin = pin, prev_pout = pout;
    if (dir == 0) {
      pin = move_point_to_diag(pin, dir, x_y_min - free_space);
      pout = move_point_to_diag(pout, dir, x_y_min - free_space - filled_space);
    } else if (dir == 1) {
      pin = move_point_to_hor(pin, dir, ymax + free_space);
      pout = move_point_to_hor(pout, dir, ymax + free_space + filled_space);
    } else if (dir == 2) {
      pin = move_point_to_ver(pin, dir, xmax + free_space);
      pout = move_point_to_ver(pout, dir, xmax + free_space + filled_space);
    } else if (dir == 3) {
      pin = move_point_to_diag(pin, dir, x_y_max + free_space);
      pout = move_point_to_diag(pout, dir, x_y_max + free_space + filled_space);
    } else if (dir == 4) {
      pin = move_point_to_hor(pin, dir, ymin - free_space);
      pout = move_point_to_hor(pout, dir, ymin - free_space - filled_space);
    } else if (dir == 5) {
      pin = move_point_to_ver(pin, dir, xmin - free_space);
      pout = move_point_to_ver(pout, dir, xmin - free_space - filled_space);
    }
    totalL += distance(prev_pin, pin) + distance(prev_pout, pout);
    if (totalL + more_space + 1 > limitSumL) {
      pin = prev_pin;
      pout = prev_pout;
      if (dir == 0) {
        int x_y_meet = min(pin.x - pin.y, pout.x - pout.y);
        pin = move_point_to_diag(pin, dir, x_y_meet);
        pout = move_point_to_diag(pout, dir, x_y_meet);
      } else if (dir == 1) {
        int y_meet = max(pin.y, pout.y);
        pin = move_point_to_hor(pin, dir, y_meet);
        pout = move_point_to_hor(pout, dir, y_meet);
      } else if (dir == 2) {
        int x_meet = max(pin.x, pout.x);
        pin = move_point_to_ver(pin, dir, x_meet);
        pout = move_point_to_ver(pout, dir, x_meet);
      } else if (dir == 3) {
        int x_y_meet = max(pin.x - pin.y, pout.x - pout.y);
        pin = move_point_to_diag(pin, dir, x_y_meet);
        pout = move_point_to_diag(pout, dir, x_y_meet);
      } else if (dir == 4) {
        int y_meet = min(pin.y, pout.y);
        pin = move_point_to_hor(pin, dir, y_meet);
        pout = move_point_to_hor(pout, dir, y_meet);
      } else if (dir == 5) {
        int x_meet = min(pin.x, pout.x);
        pin = move_point_to_ver(pin, dir, x_meet);
        pout = move_point_to_ver(pout, dir, x_meet);
      }
      if (vpin.back() != pin)
        vpin.push_back(pin);
      if (vpout.back() != pout)
        vpout.push_back(pout);
      break;
    }
    add(pin);
    add(pout);
    vpin.push_back(pin);
    vpout.push_back(pout);
  }
  vpin.insert(vpin.end(), vpout.rbegin(), vpout.rend());

  Test T(A, B, vpin);
  int sumL = T.sumL();
  int maxScale = max(maxSumL / sumL, 1);
  int scale = rnd.next((maxScale + 1) / 2, maxScale);
  T.random_shift().compress().scale(scale);
  if (constantL) {
    T.make_uniform().random_shift();
  }
  return T.random_reverse().rotate(rotate);
}

Test zig_zag(int A, int B, int maxN, int maxSumL, int constantL, int rotate) {
  int height = rnd.next(1, int(maxSumL/5));
  int width = (maxSumL - 2 * height) / 3;
  if (constantL) {
    height = rnd.next(1, int(min(maxSumL, maxN)/5));
    width = (min(maxSumL, maxN) - 2 * height) / 3;
  }
  vector<int> turning_x;
  int same_count = 3;
  int needed = maxN-4 - same_count;
  if (width < 2 * needed) {
    turning_x.resize(width-1);
    iota(turning_x.begin(), turning_x.end(), 1);
    shuffle(turning_x.begin(), turning_x.end());
    turning_x.resize(min(width-1, needed) - rnd.next(10));
  } else {
    set<int> distincts;
    while ((int)distincts.size() < needed) {
      distincts.insert(rnd.next(1, width-1));
    }
    turning_x.insert(turning_x.end(), distincts.begin(), distincts.end());
  }
  sort(turning_x.begin(), turning_x.end());
  int same_start = rnd.any(turning_x);
  Point down(0, 0), up(0, height);
  vector<Point> vup = {up}, vdown = {down};
  bool is_up_hor = rnd.next(2), is_down_hor = rnd.next(2);
  for (int x : turning_x) {
    if (is_up_hor && !is_down_hor) {
      int limit = up.x + abs(up.y - down.y) - 1;
      x = min(x, limit);
    }
    int turning_mask = 0;
    if (x >= same_start && same_count > 0) {
      --same_count;
      turning_mask = 3;
    } else {
      turning_mask = rnd.next(1, 2);
    }
    up = move_point_to_ver(up, 1 + is_up_hor, x);
    down = move_point_to_ver(down, 1 + is_down_hor, x);
    if (turning_mask & 1) {
      is_up_hor = !is_up_hor;
      if (vup.back() != up)
        vup.push_back(up);
    }
    if (turning_mask & 2) {
      is_down_hor = !is_down_hor;
      if (vdown.back() != down)
        vdown.push_back(down);
    }
  }
  if (is_up_hor && !is_down_hor) {
    int limit = up.x + abs(up.y - down.y) - 1;
    width = min(width, limit);
  }
  up = move_point_to_ver(up, 1 + is_up_hor, width);
  down = move_point_to_ver(down, 1 + is_down_hor, width);
  if (up != vup.back())
    vup.push_back(up);
  if (down != vdown.back())
    vdown.push_back(down);
  vup.insert(vup.end(), vdown.rbegin(), vdown.rend());

  Test T(A, B, vup);
  int sumL = T.sumL();
  int maxScale = max(maxSumL / sumL, 1);
  int scale = rnd.next((maxScale + 1) / 2, maxScale);
  T.random_shift().compress().scale(scale);
  if (constantL) {
    T.make_uniform().random_shift();
  }
  return T.random_reverse().rotate(rotate);
}

Test snake(int A, int B, int maxN, int maxSumL, int constantL, int length,
           int rotate) {
  Test T(A, B);

  int limitSumL = maxSumL;
  if (constantL)
    limitSumL = min(maxN, maxSumL);

  int repeat = (limitSumL - 2 * length - 4) / (4  * length + 6);
  repeat = min(repeat, (maxN - 4) / 12);

  T.add_move(0, length+1);
  for (int i = 0; i < repeat; ++i) {
    T.add_move(1, 2);
    T.add_move(2, 2);
    T.add_move(3, length-3);
    T.add_move(2, 1);
    T.add_move(1, 1);
    T.add_move(0, length);
  }
  T.add_move(1, 1);
  T.add_move(3, length+1);
  for (int i = 0; i < repeat; ++i) {
    T.add_move(4, 2);
    T.add_move(5, 2);
    T.add_move(0, length-3);
    T.add_move(5, 1);
    T.add_move(4, 1);
    T.add_move(3, length);
  }
  T.add_move(4, 1);

  int sumL = T.sumL();
  int maxScale = max(maxSumL / sumL, 1);
  int scale = rnd.next(1, maxScale);
  T.random_shift().compress().scale(scale);
  if (constantL) {
    T.make_uniform().random_shift();
  }
  return T.random_reverse().rotate(rotate);
}

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);

  if (strcmp(argv[1], "triangle") == 0) {
    int A = rnd.next(0, atoi(argv[2]));
    int B = rnd.next(0, atoi(argv[3]));
    int dir1 = atoi(argv[4]);
    int dir2 = atoi(argv[5]);
    int dir3 = atoi(argv[6]);
    int len = atoi(argv[7]);
    triangle(A, B, dir1, dir2, dir3, len).print();
    return 0;
  }

  assert(argc > 6);

  int A = rnd.next(0, atoi(argv[2]));
  int B = rnd.next(0, atoi(argv[3]));
  int maxN = atoi(argv[4]);
  int maxSumL = atoi(argv[5]);
  int constantL = atoi(argv[6]);

  if (strcmp(argv[1], "random_linear_traversal") == 0) {
    int maxCnt = atoi(argv[7]);
    int maxLen = atoi(argv[8]);
    random_linear_traversal(A, B, maxN, maxSumL, constantL, maxCnt, maxLen)
      .print();
  } else if (strcmp(argv[1], "dfs_linear_traversal") == 0) {
    int maxCnt = atoi(argv[7]);
    int maxLen = atoi(argv[8]);
    int rotate = atoi(argv[9]);
    dfs_linear_traversal(A, B, maxN, maxSumL, constantL, maxCnt, maxLen,
      rotate).print();
  } else if (strcmp(argv[1], "spiral") == 0) {
    int more_space = atoi(argv[7]);
    int rotate = atoi(argv[8]);
    spiral(A, B, maxN, maxSumL, constantL, more_space, rotate).print();
  } else if (strcmp(argv[1], "zig_zag") == 0) {
    int rotate = atoi(argv[7]);
    zig_zag(A, B, maxN, maxSumL, constantL, rotate).print();
  } else if (strcmp(argv[1], "snake") == 0) {
    int length = atoi(argv[7]);
    int rotate = atoi(argv[8]);
    snake(A, B, maxN, maxSumL, constantL, length, rotate).print();
  } else {
    assert(false);
  }
}
