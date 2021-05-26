#include "hexagon.h"
#include <bits/stdc++.h>
using namespace std;

const long long MOD=1e9+7;

int dx[]={0,-1,-1,0,1,1,0};
int dy[]={0,0,1,1,0,-1,-1};
long long area, sum;
long long xa, ya, xb, yb;

int draw_territory(int N, int A, int, vector<int> D, vector<int> L) {
  for (int i=0;i<N;i++) {
    xb=xa+dx[D[i]]*L[i];
    yb=ya+dy[D[i]]*L[i];
    area+=xa*yb-ya*xb;
    sum+=L[i];
    xa=xb;
    ya=yb;
  }
  if (area<0) {
    area=-area;
  }
  area=((area+sum)/2+1)%MOD;
  return area*A%MOD;
}
