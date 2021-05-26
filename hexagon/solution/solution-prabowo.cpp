#include "hexagon.h"

#include <bits/stdc++.h>

using namespace std;

const int MOD = 1'000'000'007;

struct Point {
  int x, y;
  Point(int _x=0, int _y=0): x(_x), y(_y) {}
  Point operator + (const Point &p) { return Point(x + p.x, y + p.y); }
  Point operator += (const Point &p) { return Point(x += p.x, y += p.y); }
  Point operator * (int s) { return Point(s * x, s * y); }
};

struct Bound {
  int xl, xr, y, m, face;
  Bound(int _xl, int _xr, int _y, int _m, int _face):
      xl(_xl), xr(_xr), y(_y), m(_m), face(_face) {}

  inline int eval(int x) const { return y + (x - xl) * m; }

  bool operator < (const Bound &other) const {
    int x = max(xl, other.xl);
    int thisy = eval(x), othery = other.eval(x);
    if (thisy != othery) return thisy < othery;
    return face > other.face;
  }
};

struct Node {
  int xl, yl1, yl2, xr, yr1, yr2;
  Node(int _xl, int _yl1, int _yl2, int _xr, int _yr1, int _yr2):
      xl(_xl), yl1(_yl1), yl2(_yl2), xr(_xr), yr1(_yr1), yr2(_yr2) {}
};

Point dir[6] = {Point(0, 1), Point(1, 1), Point(1, 0),
                Point(0, -1), Point(-1, -1), Point(-1, 0)};

vector<Point> constructPolygon(int N, vector<int> D, vector<int> L) {
  vector<Point> polygon(N + 1);
  Point current(0, 0);
  long long area = 0;
  for (int i = 0; i < N; ++i) {
    polygon[i] = current;
    current += dir[D[i]] * L[i];
    area += 1LL * polygon[i].x * current.y - 1LL * current.x * polygon[i].y;
  }
  assert(current.x == 0 && current.y == 0);
  polygon.back() = current;

  // ensure ccw
  if (area < 0) reverse(polygon.begin(), polygon.end());
  return polygon;
}

vector<Bound> getTopBottomBounds(vector<Point> polygon) {
  int N = static_cast<int>(polygon.size()) - 1;
  vector<Bound> bounds;
  for (int i = 0; i < N; ++i) {
    Point p = polygon[i], q = polygon[i + 1];
    if (p.x == q.x) continue;

    Point prv = polygon[i == 0 ? N - 1 : i - 1],
          nxt = polygon[i + 2 > N ? 1 : i + 2];
    int m = p.y != q.y;
    if (p.x < q.x) {
      // bottom bound
      if (m == 0) {
        if (prv.x == p.x && prv.y < p.y) p += Point(1, 0);
        if (nxt.x > q.x || nxt.y < q.y) q += Point(-1, 0);
      } else {
        if (prv.x >= p.x && prv.y <= p.y) p += Point(1, 1);
        if (nxt.x > q.x || (nxt.x == q.x && nxt.y < q.y)) q += Point(-1, -1);
      }
      if (p.x <= q.x) {
        bounds.emplace_back(p.x, q.x, p.y, m, 1);
      }
    } else {
      // top bound
      if (m == 0) {
        if (prv.x == p.x && prv.y > p.y) p += Point(-1, 0);
        if (nxt.x < q.x || nxt.y > q.y) q += Point(1, 0);
      } else {
        if (prv.x <= p.x && prv.y >= p.y) p += Point(-1, -1);
        if (nxt.x < q.x || (nxt.x == q.x && nxt.y > q.y)) q += Point(1, 1);
      }
      if (q.x <= p.x) {
        bounds.emplace_back(q.x, p.x, q.y, m, -1);
      }
    }
  }
  return bounds;
}

vector<Node> createNodes(vector<Bound> bounds) {
  vector<Node> nodes;
  vector<tuple<int, int, int>> sweep(bounds.size() * 2);
  vector<int> lstX(bounds.size());
  for (int i = 0; i < static_cast<int>(bounds.size()); ++i) {
    sweep[i*2 + 0] = make_tuple(bounds[i].xl, -1, i);
    sweep[i*2 + 1] = make_tuple(bounds[i].xr, 1, i);
    lstX[i] = bounds[i].xl;
  }
  sort(sweep.begin(), sweep.end());
  set<pair<Bound, int>> sweeping;
  for (int i = 0; i < static_cast<int>(sweep.size()); ++i) {
    auto [x, t, idx] = sweep[i];

    auto top = sweeping.lower_bound({bounds[idx], -1});
    auto bottom = top;
    bool match = true;

    if (top == sweeping.end()) {
      match = false;
    } else {
      if (t < 0) {
        if (bottom == sweeping.begin()) match = false;
        else --bottom;
      } else {
        if (bounds[idx].face > 0) {
          if (next(top) == sweeping.end()) match = false;
          else ++top;
        } else {
          if (bottom == sweeping.begin()) match = false;
          else --bottom;
        }
      }
      if (top->first.face > 0 || bottom->first.face < 0) match = false;
    }

    if (match) {
      if (t < 0) --x;

      if (lstX[top->second] <= x && lstX[bottom->second] <= x) {
        assert(lstX[top->second] == lstX[bottom->second]);
        // create node from the surrounding bound pair
        int px = lstX[top->second];
        nodes.emplace_back(px, bottom->first.eval(px), top->first.eval(px),
                           x, bottom->first.eval(x), top->first.eval(x));
        lstX[top->second] = lstX[bottom->second] = x + 1;
      }
    }

    if (t < 0) sweeping.insert({bounds[idx], idx});
    else sweeping.erase({bounds[idx], idx});
  }
  return nodes;
}

vector<vector<int>> buildTree(const vector<Node> &nodes) {
  map<int, vector<tuple<int, int, int>>> lranges, rranges;
  for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
    lranges[nodes[i].xl].emplace_back(nodes[i].yl1, nodes[i].yl2, i);
    rranges[nodes[i].xr].emplace_back(nodes[i].yr1, nodes[i].yr2, i);
  }

  for (auto &p : lranges) sort(p.second.begin(), p.second.end());
  for (auto &p : rranges) sort(p.second.begin(), p.second.end());

  vector<vector<int>> edges(nodes.size());
  for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
    vector<tuple<int, int, int>> &rangeR = rranges[nodes[i].xl - 1];
    int idx = lower_bound(rangeR.begin(), rangeR.end(), 
                  make_tuple(nodes[i].yl2 + 1, -2e9, -1)) - rangeR.begin();
    for (int j = idx - 1; j >= 0; --j) {
      if (get<1>(rangeR[j]) + 1 < nodes[i].yl1) break;
      edges[i].push_back(get<2>(rangeR[j]));
    }

    vector<tuple<int, int, int>> &rangeL = lranges[nodes[i].xr + 1];
    idx = lower_bound(rangeL.begin(), rangeL.end(), 
              make_tuple(nodes[i].yr2 + 2, -2e9, -1)) - rangeL.begin();
    for (int j = idx - 1; j >= 0; --j) {
      if (get<1>(rangeL[j]) < nodes[i].yr1) break;
      edges[i].push_back(get<2>(rangeL[j]));
    }
  }

  return edges;
}

pair<int, int> findRoot(const vector<Node> &nodes) {
  for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
    if (nodes[i].xl == 0 && nodes[i].yl1 <= 0 && 0 <= nodes[i].yl2) {
      return {i, 0};
    }
    if (nodes[i].xr == 0 && nodes[i].yr1 <= 0 && 0 <= nodes[i].yr2) {
      return {i, 1};
    }
  }
  assert(0);
  return {-1, -1};
}

int computeNode(int ylbot, int yltop, int yrbot, int yrtop, int width,
    int mini, int minibot, int minitop, int A, int B) {
  ++width;

  auto sum_i = [&](int n) {
    return 1LL * n * (n + 1) / 2 % MOD;
  };

  auto sum_i2 = [&](int n) {
    long long ret = 1LL * n * (n + 1) / 2;
    if (ret % 3 == 0) ret = ret / 3 % MOD * (n*2 + 1) % MOD;
    else ret = ret % MOD * (n*2 + 1) / 3 % MOD;
    return ret;
  };

  auto computeTriangle = [&](int mini, int width) {
    // sum((mini + i - 1) * i)
    return (sum_i2(width) + 1LL * (mini - 1) * sum_i(width)) % MOD;
  };

  auto computeRectangle = [&](int mini, int width, int height) {
    // sum_width(mini + i - 1) * height + sum_height(i - 1) * width
    return (1LL * width * sum_i(height - 1) + 1LL * height * sum_i(width - 1)
          + 1LL * width * height % MOD * mini) % MOD;
  };

  auto computeTriangleR = [&](int start, int width) {
    // sum((start + i) * (width - i)) = sum(start*width + (width-start)*i - i^2)
    int ret = (1LL * start * width % MOD * width +
                1LL * (width - start) * sum_i(width) - sum_i2(width)) % MOD;
    if (ret < 0) ret += MOD;
    return ret;
  };

  int ret = (computeTriangle(mini, width) +
             1LL * computeRectangle(mini, width, 1) * (minitop - minibot) +
             computeRectangle(mini + 1, width, yltop - minitop) +
             computeRectangle(mini + 1, width, minibot - ylbot)) % MOD;
  if (yrbot > ylbot) {
    ret = (ret - computeTriangleR(mini + minibot - ylbot, width) + MOD) % MOD;
    if (int start = width - (minibot - ylbot) - 1; start > 0) {
      ret = (ret - computeTriangleR(0, start) + MOD) % MOD;
    }
  }
  if (width > 1 && yrtop == yltop) {
    ret = (ret - computeTriangleR(mini + yltop - minitop, width) + MOD) % MOD;
    if (int start = width - (yltop - minitop) - 1; start > 0) {
      ret = (ret - computeTriangleR(0, start) + MOD) % MOD;
    }
  }
  ret = 1LL * ret * B % MOD;

  ret = (ret + 1LL * width * (yltop - ylbot + 1) % MOD * A) % MOD;
  if (ylbot == yrbot && yltop < yrtop) {
    ret = (ret + 1LL * sum_i(width - 1) * A) % MOD;
  }
  if (yltop == yrtop && ylbot < yrbot) {
    ret = (ret - 1LL * sum_i(width - 1) * A) % MOD;
    if (ret < 0) ret += MOD;
  }
  return ret;
}

int dfs(int u, int p, int side, int mini, int ybot, int ytop, int A, int B,
    const vector<Node> &nodes, const vector<vector<int>> &edges) {
  int ret = 0;
  int width = nodes[u].xr - nodes[u].xl;

  // compute node value
  if (side == 0) {
    ret = computeNode(nodes[u].yl1, nodes[u].yl2, nodes[u].yr1, nodes[u].yr2,
                      width, mini, ybot, ytop, A, B);
  } else {
    ret = computeNode(-nodes[u].yr2, -nodes[u].yr1, -nodes[u].yl2, -nodes[u].yl1,
                      width, mini, -ytop, -ybot, A, B);
  }

  int otherMini = mini + width, otherYbot, otherYtop;
  if (side == 0) {
    otherYbot = max(ybot, nodes[u].yr1);
    otherYtop = min(ytop + width, nodes[u].yr2);
  } else {
    otherYbot = max(ybot - width, nodes[u].yl1);
    otherYtop = min(ytop, nodes[u].yl2);
  }

  int ylbot = ybot, yltop = ytop, minil = mini;
  int yrbot = otherYbot, yrtop = otherYtop, minir = otherMini;
  if (side == 1) {
    swap(ylbot, yrbot); swap(yltop, yrtop); swap(minil, minir);
  }

  for (int v : edges[u]) {
    if (v == p) continue;
    if (nodes[v].xr < nodes[u].xl) {
      // go left
      if (nodes[v].yr1 > yltop) {
        int ny = nodes[v].yr1;
        ret = (ret + dfs(v, u, 1, minil + ny - yltop + 1, ny, ny, A, B, nodes, edges)) % MOD;
      } else if (nodes[v].yr2 + 1 < ylbot) {
        int ny = nodes[v].yr2;
        ret = (ret + dfs(v, u, 1, minil + ylbot - ny, ny, ny, A, B, nodes, edges)) % MOD;
      } else {
        ret = (ret + dfs(v, u, 1, minil + 1, max(nodes[v].yr1, ylbot - 1), min(nodes[v].yr2, yltop), A, B, nodes, edges)) % MOD;
      }
    } else {
      // go right
      if (yrtop + 1 < nodes[v].yl1) {
        int ny = nodes[v].yl1;
        ret = (ret + dfs(v, u, 0, minir + ny - yrtop, ny, ny, A, B, nodes, edges)) % MOD;
      } else if (yrbot > nodes[v].yl2) {
        int ny = nodes[v].yl2;
        ret = (ret + dfs(v, u, 0, minir + yrbot - ny + 1, ny, ny, A, B, nodes, edges)) % MOD;
      } else {
        ret = (ret + dfs(v, u, 0, minir + 1, max(yrbot, nodes[v].yl1), min(yrtop + 1, nodes[v].yl2), A, B, nodes, edges)) % MOD;
      }
    }
  }
  return ret;
}

int draw_territory(int N, int A, int B, vector<int> D, vector<int> L) {
  for (int &d : D) --d;
  vector<Node> nodes = createNodes(getTopBottomBounds(constructPolygon(N, D, L)));
  vector<vector<int>> edges = buildTree(nodes);
  pair<int, int> root = findRoot(nodes);
  return dfs(root.first, -1, root.second, 0, 0, 0, A, B, nodes, edges);
}
