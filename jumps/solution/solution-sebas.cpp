#include "jumps.h"

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e5 + 5;
const int LOG = 18;

int N;
vector<int> H;

int jump_left[LOG][MAXN];
int jump_right[LOG][MAXN];
int jump_high[LOG][MAXN];

void init(int _N, std::vector<int> _H) {
    N = _N;
    H = _H;

    // Build first larger to the left
    stack<int> decreasing_h;
    for(int i = 0; i < N; i++){
        while(!decreasing_h.empty() && H[decreasing_h.top()] <= H[i])decreasing_h.pop();
        
        if(decreasing_h.empty())jump_left[0][i] = -1;
        else jump_left[0][i] = decreasing_h.top();

        decreasing_h.push(i);
    }
    
    while(!decreasing_h.empty())decreasing_h.pop();

    // Build first larger to the right
    for(int i = N - 1; i >= 0; --i){
        while(!decreasing_h.empty() && H[decreasing_h.top()] <= H[i])decreasing_h.pop();
        
        if(decreasing_h.empty())jump_right[0][i] = -1;
        else jump_right[0][i] = decreasing_h.top();

        decreasing_h.push(i);;
    }

    // Build jump to higher position
    for(int i = 0; i < N; i++){
        if(jump_left[0][i] == -1 && jump_right[0][i] == -1)jump_high[0][i] = -1;
        else if(jump_left[0][i] == -1)jump_high[0][i] = jump_right[0][i];
        else if(jump_right[0][i] == -1)jump_high[0][i] = jump_left[0][i];
        else {
            jump_high[0][i] = H[jump_left[0][i]] > H[jump_right[0][i]] ? jump_left[0][i] : jump_right[0][i];
        }

    }

    // Build sparse table
    for(int j = 1; j < LOG; j++){
        for(int i = 0; i < N; i++){
            jump_left[j][i] = jump_right[j][i] = jump_high[j][i] = -1;

            if(jump_left[j - 1][i] != -1){
                jump_left[j][i] = jump_left[j - 1][jump_left[j - 1][i]];
            }
            if(jump_right[j - 1][i] != -1){
                jump_right[j][i] = jump_right[j - 1][jump_right[j - 1][i]];
            }
            if(jump_high[j - 1][i] != -1){
                jump_high[j][i] = jump_high[j - 1][jump_high[j - 1][i]];
            }
            
        }
    }

}

int minimum_jumps(int A, int B, int C, int D) {
    int now = B;
    // Jump left as long as jump_right still <= D
    // and position to_left >= A
    for(int i = LOG - 1; i >= 0; --i){
        int to_left = jump_left[i][now];
        if(to_left != -1 && A <= to_left && 
                jump_right[0][to_left] <= D && jump_right[0][to_left] != -1){
            now = to_left;
        }
    }

    int jump = 0;
    // Jump to higher as long as jump_right still < C
    for(int i = LOG - 1; i >= 0; --i){
        int nx_pos = jump_high[i][now];
        if(nx_pos != -1 && jump_right[0][nx_pos] < C && jump_right[0][nx_pos] != -1){
            now = nx_pos;
            jump += (1 << i);
        }
    }

    // If by jumping high once more can go >= C but still <= D, then just jump
    // Without this should fail on testcases like :
    // 5 1
    // 4 1 2 3 5
    // 1 1 4 4
    if(jump_right[0][now] != -1 &&  jump_right[0][now] < C &&
        jump_right[0][jump_high[0][now]] <= D && jump_right[0][jump_high[0][now]] != -1){
        jump++;
        now = jump_high[0][now];
    }

    for(int i = LOG - 1; i >= 0; --i){
        int to_right = jump_right[i][now];
        if(to_right != -1 && to_right < C){
            now = to_right;
            jump += (1 << i);
        }
    }

    if(jump_right[0][now] == -1 || jump_right[0][now] > D)return -1;

    return jump + 1;
}
