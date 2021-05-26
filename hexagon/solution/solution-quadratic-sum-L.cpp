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

int draw_territory(int N, int A, int B, vector<int> D, vector<int> L) {
  Point now;
  int min_x = 0, max_x = 0;
  int min_y = 0, max_y = 0;
  vector<Point> path;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < L[i]; ++j) {
      now = now + pdirs[D[i]-1];
      min_x = min(min_x, now.x);
      max_x = max(max_x, now.x);
      min_y = min(min_y, now.y);
      max_y = max(max_y, now.y);
      path.push_back(now);
    }
  }

  // Flood-fill to mark the outside of the territory
  int width = max_x - min_x + 3, height = max_y - min_y + 3;
  vector<vector<bool>> on_path(width, vector<bool>(height, false));
  Point move(-min_x + 1, -min_y + 1);
  now = now + move;
  for (auto & p : path) {
    p = p + move;
    on_path[p.x][p.y] = true;
  }
  vector<vector<int>> distance(width, vector<int>(height, -2));
  queue<Point> que;
  distance[0][0] = -1;
  que.emplace(0, 0);
  while (!que.empty()) {
    Point p = que.front();
    que.pop();
    for (auto & pdir : pdirs) {
      Point nxt = p + pdir;
      if (nxt.x < 0 || nxt.x >= width || nxt.y < 0 || nxt.y >= height)
        continue;
      if (on_path[nxt.x][nxt.y] || distance[nxt.x][nxt.y] == -1)
        continue;
      distance[nxt.x][nxt.y] = -1;
      que.push(nxt);
    }
  }

  // BFS to calculate the distances and total score.
  distance[now.x][now.y] = 0;
  long long ans = A;
  que.push(now);
  while (!que.empty()) {
    Point p = que.front();
    que.pop();
    int d = distance[p.x][p.y] + 1;
    for (auto & pdir : pdirs) {
      Point nxt = p + pdir;
      if (nxt.x < 0 || nxt.x >= width || nxt.y < 0 || nxt.y >= height)
        continue;
      if (distance[nxt.x][nxt.y] != -2)
        continue;
      distance[nxt.x][nxt.y] = d;
      ans = (ans + A + 1LL * B * d) % mod;
      que.push(nxt);
    }
  }
  return ans;
}
