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
};

constexpr long long inv2 = (mod + 1LL) / 2;
// calc: sum(a + (i-1) * b)) for i = 1..n
long long calc_1(long long a, long long b, long long n) {
  long long ret = (2LL * a + (n-1LL) * b) % mod;
  ret = ret * n % mod;
  ret = ret * inv2 % mod;
  return ret;
}

constexpr long long inv3 = ((mod % 3) == 2 ? (mod + 1LL) : (2LL * mod + 1)) / 3;
// calc: sum(i * (a + (i-1) * b)) for i = 1..n
long long calc_2(long long a, long long b, long long n) {
  long long ret = n * (n + 1LL) % mod;
  ret = ret * inv2 % mod;
  long long mul = (2LL * n + 1LL) * inv3 % mod;
  mul = (a - b + mul * b) % mod;
  ret = ret * mul % mod;
  return ret;
}

long long calc_rectangle(int width, int minval, int same, int down, int up) {
  long long ret = 0;
  long long a = 1LL * minval * same %  mod;
  if (down) {
    a = (a + calc_1(minval+1, 1, down)) % mod;
  }
  ret += calc_1(a, same + down, width);
  if (up) {
    int len = min(up, width);
    ret += calc_2(minval+1, 1, len) + calc_2(minval+1, 1, len-1);
    if (len < width) {
      ret += calc_1(1LL * (minval + len) * up % mod, up, width - len);
    }
    if (len < up) {
      ret += calc_1(1LL * (minval + len + 1) * width % mod, width, up - len);
    }
  }
  return ret % mod;
}

long long calc(int width, int minval, int same, int down, int up,
               bool del_topleft, bool del_botright) {
  long long ret = calc_rectangle(width, minval, same, down, up);
  if (width > 1) {
    int d = width - 1;
    if (del_topleft) {
      assert(up >= d);
      ret -= calc_2(minval + (up - d + 1), 1, d);
    }
    if (del_botright) {
      int len = min(down, d);
      ret -= calc_2(minval + len + down, mod-1, len);
      if (len < d) {
        ret -= calc_2(minval + len+1, 1, d - len);
        long long a = calc_1(minval + len+2, 1, down);
        ret -= calc_1(a, down, d - len);
      }
    }
  }
  ret %= mod;
  if (ret < 0)
    ret += mod;
  return ret;
}

struct Block {
  Column left, right;
  Block(Column _left, Column _right) : left(_left), right(_right) {}

  long long move_right(long long a, long long b) {
    int dx = right.x - left.x;
    long long ret = (left.top - left.bot + right.top - right.bot + 2LL);
    ret = ((ret * (dx + 1) % mod) * inv2 % mod) * a % mod;

    right.update(left.minval + dx, left.mbot, left.mtop + dx);
    int width = right.x - left.x + 1;
    int same = left.mtop - left.mbot + 1;
    int up = right.top - left.mtop;
    int down = left.mbot - left.bot;
    bool del_toplef = left.top < right.top, del_botrig = left.bot < right.bot;
    ret += b * calc(width, left.minval, same, down, up, del_toplef, del_botrig);
    return ret % mod;
  }

  long long move_left(long long a, long long b) {
    int dx = right.x - left.x;
    long long ret = (left.top - left.bot + right.top - right.bot + 2LL);
    ret = ((ret * (dx + 1) % mod) * inv2 % mod) * a % mod;

    left.update(right.minval + dx, right.mbot - dx, right.mtop);
    int width = right.x - left.x + 1;
    int same = right.mtop - right.mbot + 1;
    int down = right.top - right.mtop;
    int up = right.mbot - left.bot;
    bool del_botrig = left.top < right.top, del_toplef = left.bot < right.bot;
    ret += b * calc(width, right.minval, same, down, up, del_toplef,
                    del_botrig);
    return ret % mod;
  }
};

struct Line {
  Point pstart, pdir;
  int id;
  Point get(int x) const {
    return pstart + (pdir * (x - pstart.x));
  }
  bool operator<(const Line & rig) const {
    int x_common = max(pstart.x, rig.pstart.x);
    return get(x_common).y < rig.get(x_common).y;
  }
};

vector<Block> find_blocks(vector<Point> & vp) {
  auto y_events = find_top_bottom(vp);
  vector<int> last(y_events.size()), sign(y_events.size());
  vector<Line> lines(y_events.size());
  vector<pair<int, int>> x_events;
  for (int i = 0; i < (int)y_events.size(); ++i) {
    Point pleft, pright;
    tie(pleft, pright, sign[i]) = y_events[i];
    last[i] = pleft.x;
    lines[i] = {pleft, (pright.y > pleft.y ? Point(1, 1) : Point(1, 0)), i};
    x_events.emplace_back(pleft.x, i);
    x_events.emplace_back(pright.x+1, ~i);
  }
  sort(x_events.begin(), x_events.end());
  set<Line> st;
  vector<Block> result;
  auto check_block = [&](set<Line>::iterator nxt, int x) {
    if (nxt != st.begin() && nxt != st.end() && sign[nxt->id] > 0) {
      auto prv = prev(nxt);
      if (sign[prv->id] < 0 && min(last[prv->id], last[nxt->id]) < x) {
        assert(last[prv->id] == last[nxt->id]);
        int xleft = last[prv->id], xright = x - 1;
        result.emplace_back(
            Column(xleft, prv->get(xleft).y+1, nxt->get(xleft).y),
            Column(xright, prv->get(xright).y+1, nxt->get(xright).y));
        last[prv->id] = last[nxt->id] = x;
      }
    }
  };
  for (auto & e : x_events) {
    int id = e.second;
    if (id < 0) {
      id = ~id;
      auto it = st.upper_bound(lines[id]);
      if (it != st.end())
        check_block(it, e.first);
      assert(it != st.begin());
      --it;
      assert(it->id == id);
      check_block(it, e.first);
      st.erase(it);
    } else {
      auto nxt = st.lower_bound(lines[id]);
      check_block(nxt, e.first);
      st.insert(lines[id]);
    }
  }
  return result;
}

vector<Block> blocks;
vector<vector<pair<int, int>>> tree;

void construct_tree() {
  tree.resize(blocks.size());
  vector<tuple<int, int, int>> events;
  for (int i = 0; i < (int)blocks.size(); ++i) {
    events.emplace_back(blocks[i].left.x, blocks[i].left.bot, i);
    events.emplace_back(blocks[i].right.x+1, blocks[i].right.bot, ~i);
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
    if (idl >= 0 && idr >= 0 && blocks[idl].right.x+1 == blocks[idr].left.x) {
      if (blocks[idl].right.bot <= blocks[idr].left.top &&
          blocks[idr].left.bot <= blocks[idl].right.top) {
        tree[idl].emplace_back(idr, 1);
        tree[idr].emplace_back(idl, -1);
        ++cnt;
      }
    }
  }
  assert(cnt + 1 == (int)blocks.size());
}

int dfs(int v, int par, long long a, long long b) {
  int ret = 0;
  for (auto & e : tree[v]) {
    int u = e.first;
    if (u == par) continue;
    if (e.second > 0) {
      blocks[u].left.update(blocks[v].right.minval+1, blocks[v].right.mbot,
                            blocks[v].right.mtop+1);
      ret = (ret + blocks[u].move_right(a, b)) % mod;
    } else {
      blocks[u].right.update(blocks[v].left.minval+1, blocks[v].left.mbot-1,
                             blocks[v].left.mtop);
      ret = (ret + blocks[u].move_left(a, b)) % mod;
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
  blocks = find_blocks(vp);
  construct_tree();
  long long ans = 0;
  int root = -1;
  for (int i = 0; i < (int)blocks.size(); ++i) {
    if (blocks[i].left.x > 0) continue;
    if (blocks[i].right.x < 0) continue;
    int top = blocks[i].left.top, bot = blocks[i].left.bot;
    if (blocks[i].left.top < blocks[i].right.top)
      top += -blocks[i].left.x;
    if (blocks[i].left.bot < blocks[i].right.bot)
      bot += -blocks[i].left.x;
    if (bot <= 0 && 0 <= top) {
      assert(root == -1);
      root = i;
      Block left = blocks[root], right = blocks[root];
      left.right = right.left = Column(0, bot, top, 0, 0, 0);
      ans = (ans + left.move_left(A, B)) % mod;
      ans = (ans + right.move_right(A, B)) % mod;
      ans = (ans - calc_1(A, B, top+1) - calc_1(A+B, B, -bot)) % mod;
      blocks[root].left = left.left;
      blocks[root].right = right.right;
    }
  }
  assert(root != -1);
  ans = (ans + dfs(root, -1, A, B)) % mod;
  if (ans < 0)
    ans += mod;
  return ans;
}
