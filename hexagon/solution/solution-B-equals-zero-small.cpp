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

int draw_territory(int N, int A, int /*B*/, vector<int> D, vector<int> L) {
  Point now;
  int min_x = 0, max_x = 0;
  int min_y = 0, max_y = 0;
  set<Point> path;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < L[i]; ++j) {
      now = now + pdirs[D[i]-1];
      min_x = min(min_x, now.x);
      max_x = max(max_x, now.x);
      min_y = min(min_y, now.y);
      max_y = max(max_y, now.y);
      path.insert(now);
    }
  }

  set<Point> surroundings;
  for (auto & p : path) {
    for (auto & pdir : pdirs) {
      surroundings.insert(p + pdir);
    }
  }
  for (auto & p : path) {
    surroundings.erase(p);
  }

  // Flood-fill to mark the outside of the territory
  set<Point> outside;
  queue<Point> que;
  que.emplace(*surroundings.begin());
  outside.insert(que.front());
  while (!que.empty()) {
    Point p = que.front();
    que.pop();
    for (auto & pdir : pdirs) {
      Point nxt = p + pdir;
      if (!surroundings.count(nxt) || outside.count(nxt))
        continue;
      outside.insert(nxt);
      que.push(nxt);
    }
  }

  // Calculate total score
  long long ans = 0;
  for (auto & p : path) {
    Point up = p + pdirs[0], down = p + pdirs[3];
    if (outside.count(up))
      ans += p.y;
    if (outside.count(down))
      ans -= down.y;
  }
  ans = (ans % mod) * A % mod;
  if (ans < 0)
    ans += mod;
  return ans;
}
