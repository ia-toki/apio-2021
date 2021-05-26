#include "hexagon.h"
#include <bits/stdc++.h>
using namespace std;

const long long MOD=1e9+7, ONE=1, HALF=(MOD+1)/2, THIRD=(MOD+1)/3;

int draw_territory(int, int A, int B, vector<int>, vector<int> L) {
  long long P=ONE*(L[1]+1)*(L[2]+2)%MOD;
  long long Q=HALF*A%MOD;
  long long R=(THIRD*B%MOD)*L[0]%MOD;
  return P*(Q+R)%MOD;
}
