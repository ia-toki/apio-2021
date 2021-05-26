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

long long signed_twice_area(vector<Point> & vp) {
  long long ret = 0;
  int n = (int)vp.size() - 1;
  for (int i = 0; i < n; ++i) {
    ret += 1LL * vp[i].x * vp[i+1].y - 1LL * vp[i+1].x * vp[i].y;
  }
  return ret;
}

int draw_territory(int N, int A, int /*B*/, vector<int> D, vector<int> L) {
  vector<Point> vp(N+1);
  for (int i = 0; i < N; ++i) {
    vp[i+1] = vp[i] + pdirs[D[i]-1] * L[i];
  }
  long long boundary = accumulate(L.begin(), L.end(), 0LL);
  long long inside = (abs(signed_twice_area(vp)) - boundary) / 2 + 1;
  long long ans = boundary + inside;
  ans = (ans % mod) * A % mod;
  if (ans < 0)
    ans += mod;
  return ans;
}
