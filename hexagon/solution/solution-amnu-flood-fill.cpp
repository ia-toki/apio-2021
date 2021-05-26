#include "hexagon.h"
#include <bits/stdc++.h>
#define pii pair<int,int>
#define i1 first
#define i2 second
using namespace std;

const int MAXL=2e3+10;
const long long MOD=1e9+7;
const int dx[]={0,-1,-1,0,1,1,0};
const int dy[]={0,0,1,1,0,-1,-1};

int vis[MAXL][MAXL];
long long now, step, ans;
queue <pii> Q;

void add(int x,int y,int z) {
  if (x<0||y<0||x>=MAXL||y>=MAXL) {
    if (x+y==-2) {
      now=(now+step)%MOD;
      Q.push({x,y});
    }
  }
  else if (vis[x][y]<=z) {
    ans=(ans+now)%MOD;
    vis[x][y]=2;
    Q.push({x,y});
  }
}

void bfs(int x,int y,int z) {
  add(x,y,z);
  if (z) add(-1,-1,z);
  while ((int)Q.size()>z) {
    x=Q.front().i1;
    y=Q.front().i2;
    Q.pop();
    if (x<0) {
      add(x,y,z);
      continue;
    }
    for (int i=1;i<=6;i++) {
      add(x+dx[i],y+dy[i],z);
    }
  }
}

int draw_territory(int N, int A, int B, vector<int> D, vector<int> L) {
  int x=MAXL/2, y=MAXL/2;
  for (int i=0;i<N;i++) {
    for (int j=0;j<L[i];j++) {
      vis[x][y]=1;
      x+=dx[D[i]];
      y+=dy[D[i]];
    }
  }
  bfs(0,0,0);
  now=A;
  step=B;
  bfs(x,y,1);
  return ans;
}
