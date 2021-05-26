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


constexpr long long inv2 = (mod + 1LL) / 2;
// calc: sum(a + (i-1) * b)) for i = 1..n
long long calc_1(long long a, long long b, long long n) {
  long long ret = (2LL * a + (n-1LL) * b) % mod;
  ret = ret * n % mod;
  ret = ret * inv2 % mod;
  return ret;
}

struct Column {
  int x, bot, top;
  int minval, mbot, mtop;

  Column(int _x, int _bot, int _top,
         int _minval = -1, int _mbot = -1, int _mtop = -1)
    : x(_x), bot(_bot), top(_top), minval(_minval), mbot(_mbot), mtop(_mtop) {}

  void update(int _minval, int _mbot, int _mtop) {
    minval = _minval;
    mbot = _mbot;
    mtop = _mtop;
    if (mbot > top) {
      minval += mbot - top;
      minval %= mod;
      mbot = mtop = top;
    }
    if (mtop < bot) {
      minval += bot - mtop;
      minval %= mod;
      mbot = mtop = bot;
    }
    mtop = min(mtop, top);
    mbot = max(mbot, bot);
  }

  long long calc(long long a, long long b) {
    long long ret = (top - bot + 1LL) * a % mod;

    long long cost = minval * (mtop - mbot + 1LL) % mod;
    if (mtop < top)
      cost += calc_1(minval+1, 1, top - mtop);
    if (mbot > bot)
      cost += calc_1(minval+1, 1, mbot - bot);
    ret = (ret + cost * b) % mod;
    return ret;
  }
};

vector<Column> find_columns(vector<Point> & vp) {
  auto top_bottom = find_top_bottom(vp);
  vector<pair<Point, int>> events;
  for (auto & line : top_bottom) {
    Point a, b;
    int sign;
    tie(a, b, sign) = line;
    Point pdir = b.y > a.y ? pdirs[1] : pdirs[2];
    for (int i = 0; i <= b.x - a.x; ++i) {
      events.emplace_back(a + pdir * i, sign);
    }
  }
  sort(events.begin(), events.end());
  vector<Column> result;
  for (int i = 0; i + 1 < (int)events.size(); ++i) {
    if (i & 1) {
      assert(events[i].first < events[i+1].first);
    } else {
      assert(events[i].second < 0 && events[i+1].second > 0);
      assert(events[i].first.x == events[i+1].first.x);
      result.emplace_back(events[i].first.x, events[i].first.y+1,
                          events[i+1].first.y);
    }
  }
  return result;
}

vector<Column> columns;
vector<vector<pair<int, int>>> tree;

void construct_tree() {
  tree.resize(columns.size());
  vector<tuple<int, int, int>> events;
  for (int i = 0; i < (int)columns.size(); ++i) {
    events.emplace_back(columns[i].x, columns[i].bot, i);
    events.emplace_back(columns[i].x+1, columns[i].bot, ~i);
  }
  int idl = -1, idr = -1;
  sort(events.begin(), events.end());
  int cnt = 0;
  for (auto & e : events) {
    int id = get<2>(e);
    if (id < 0)
      idl = ~id;
    else
      idr = id;
    if (idl >= 0 && idr >= 0 && columns[idl].x+1 == columns[idr].x) {
      if (columns[idl].bot <= columns[idr].top &&
          columns[idr].bot <= columns[idl].top) {
        tree[idl].emplace_back(idr, 1);
        tree[idr].emplace_back(idl, -1);
        ++cnt;
      }
    }
  }
  assert(cnt + 1 == (int)columns.size());
}

int dfs(int v, int par, long long a, long long b) {
  int ret = columns[v].calc(a, b);
  for (auto & e : tree[v]) {
    int u = e.first;
    if (u == par) continue;
    if (e.second > 0) {
      columns[u].update(columns[v].minval+1, columns[v].mbot,
                        columns[v].mtop+1);
    } else {
      columns[u].update(columns[v].minval+1, columns[v].mbot-1,
                        columns[v].mtop);
    }
    ret = (ret + dfs(u, v, a, b)) % mod;
  }
  return ret;
}

int draw_territory(int N, int A, int B, vector<int> D, vector<int> L) {
  vector<Point> vp(N+1);
  for (int i = 0; i < N; ++i) {
    vp[i+1] = vp[i] + pdirs[D[i]-1] * L[i];
  }
  if (signed_twice_area(vp) < 0) {
    reverse(vp.begin(), vp.end());
  }
  columns = find_columns(vp);
  construct_tree();
  int root = -1;
  for (int i = 0; i < (int)columns.size(); ++i) {
    if (columns[i].x > 0) continue;
    if (columns[i].x < 0) continue;
    int top = columns[i].top, bot = columns[i].bot;
    if (bot <= 0 && 0 <= top) {
      assert(root == -1);
      root = i;
      columns[root].update(0, 0, 0);
    }
  }
  assert(root != -1);
  long long ans = dfs(root, -1, A, B) % mod;
  if (ans < 0)
    ans += mod;
  return ans;
}
