#include "hexagon.h"
#include <bits/stdc++.h>

using namespace std;

struct Point {
  int x, y;
  Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
  Point operator+(Point p) const { return Point(x+p.x, y+p.y); }
  Point operator-(Point p) const { return Point(x-p.x, y-p.y); }
  Point operator*(int s) const { return Point(x * s, y * s); }
  bool operator==(Point p) const { return x == p.x && y == p.y; }
  bool operator<(Point p) const { return x == p.x ? y < p.y : x < p.x; }
};

ostream& operator<<(ostream& os, Point p) {
  return os << "(" << p.x << ", " << p.y << ")";
}

constexpr int mod = 1e9 + 7;
const vector<Point> pdirs = { Point(0, 1), Point(1, 1), Point(1, 0),
                              Point(0, -1), Point(-1, -1), Point(-1, 0) };

// assumption: vp circulating the territory in counter-clockwise direction
vector<tuple<Point, Point, int>> find_top_bottom(vector<Point> & vp) {
  vector<tuple<Point, Point, int>> result;
  int n = (int)vp.size()-1;
  for (int i = 0; i < n; ++i) {
    Point a = vp[i], b = vp[(i+1) % n];
    Point bef = vp[(i+n-1) % n], aft = vp[(i+2) % n];
    if (a.x == b.x)
      continue;
    if (a.x < b.x) { // bottom
      int dir;
      if (a.y < b.y) {
        dir = 1;
      } else {
        assert(a.y == b.y);
        dir = 2;
      }
      if (bef.x < a.x || bef.y < a.y)
        a = a + pdirs[dir];
      if (aft.x <= b.x && aft.y < b.y)
        b = b - pdirs[dir];
      if (b < a)
        continue;
      --a.y;
      --b.y;
      result.emplace_back(a, b, -1);
    } else { // top
      int dir;
      if (a.y > b.y) {
        dir = 4;
      } else {
        assert(a.y == b.y);
        dir = 5;
      }
      if (bef.x > a.x || bef.y > a.y)
        a = a + pdirs[dir];
      if (aft.x >= b.x && aft.y > b.y)
        b = b - pdirs[dir];
      if (a < b)
        continue;
      result.emplace_back(b, a, +1);
    }
  }
  return result;
}

// calc: sum(a + (i-1) * b)) for i = 1..n
long long calc_1(long long a, long long b, long long n) {
  long long ret = (2LL * a + (n-1LL) * b);
  ret = ret * n / 2;
  return ret;
}

long long count_territory(vector<Point> & vp) {
  auto top_bottom = find_top_bottom(vp);
  long long ret = 0;
  for (auto & line : top_bottom) {
    Point a, b;
    int sign;
    tie(a, b, sign) = line;
    if (sign > 0)
      ret += calc_1(a.y, (b.y > a.y ? 1 : (b.y < a.y ? -1 : 0)), b.x - a.x + 1);
    else
      ret -= calc_1(a.y, (b.y > a.y ? 1 : (b.y < a.y ? -1 : 0)), b.x - a.x + 1);
  }
  return ret;
}

int draw_territory(int N, int A, int /*B*/, vector<int> D, vector<int> L) {
  vector<Point> vp(N+1);
  for (int i = 0; i < N; ++i) {
    vp[i+1] = vp[i] + pdirs[D[i]-1] * L[i];
  }
  long long ans = count_territory(vp);
  reverse(vp.begin(), vp.end());
  ans = max(ans, count_territory(vp));
  assert(ans > 0);
  ans = (ans % mod) * A % mod;
  if (ans < 0)
    ans += mod;
  return ans;
}
