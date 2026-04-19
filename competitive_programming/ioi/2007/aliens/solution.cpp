#include <bits/stdc++.h>
using namespace std;

// Interactive: examine(x, y) returns:
// 0 if (x, y) is the target
// 1 if target is to the upper-right
// 2 if target is to the upper-left
// 3 if target is to the lower-left
// 4 if target is to the lower-right
// (exact encoding depends on problem statement)

// Forward declaration for grader function
int examine(int x, int y);

void solve(int N) {
    // Binary search on x and y simultaneously
    int lo_x = 1, hi_x = N;
    int lo_y = 1, hi_y = N;

    while (lo_x < hi_x || lo_y < hi_y) {
        int mx = (lo_x + hi_x) / 2;
        int my = (lo_y + hi_y) / 2;

        int res = examine(mx, my);

        if (res == 0) return; // found!

        // Adjust based on response
        // Response encoding (example):
        // "NE" (1): target is north and east -> x > mx, y > my
        // "NW" (2): target is north and west -> x < mx, y > my
        // "SW" (3): target is south and west -> x < mx, y < my
        // "SE" (4): target is south and east -> x > mx, y < my

        if (res == 1) { // NE
            lo_x = mx + 1;
            lo_y = my + 1;
        } else if (res == 2) { // NW
            hi_x = mx - 1;
            lo_y = my + 1;
        } else if (res == 3) { // SW
            hi_x = mx - 1;
            hi_y = my - 1;
        } else if (res == 4) { // SE
            lo_x = mx + 1;
            hi_y = my - 1;
        }
    }

    examine(lo_x, lo_y); // final check
}

// For non-interactive testing:
int target_x, target_y;

int examine(int x, int y) {
    if (x == target_x && y == target_y) return 0;
    int res = 0;
    if (target_x > x && target_y > y) res = 1;
    else if (target_x < x && target_y > y) res = 2;
    else if (target_x < x && target_y < y) res = 3;
    else res = 4;
    return res;
}

int main(){
    int N;
    cin >> N >> target_x >> target_y;
    solve(N);
    cout << target_x << " " << target_y << "\n";
    return 0;
}
