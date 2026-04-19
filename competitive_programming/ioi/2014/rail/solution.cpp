#include <bits/stdc++.h>
using namespace std;

// In the actual IOI grader, getDistance is provided.
int getDistance(int i, int j);

// We'll define the solution function as specified by the grader.
void findLocation(int n, int first, int location[], int stype[]) {
    // stype[i]: 1 = C, 2 = D
    // location[i]: position of station i
    // Station 0 is type C at position 'first'
    stype[0] = 1;
    location[0] = first;

    if (n == 1) return;

    // Query d(0, i) for all i
    vector<int> d0(n);
    d0[0] = 0;
    for (int i = 1; i < n; i++) {
        d0[i] = getDistance(0, i);
    }

    // Find nearest station to 0: this is the closest D-station to the right
    int r = 1;
    for (int i = 2; i < n; i++) {
        if (d0[i] < d0[r]) r = i;
    }
    stype[r] = 2; // type D
    location[r] = first + d0[r];

    // Classify remaining stations
    // Right-side C candidates and left-side D candidates
    vector<int> rightC, leftD;

    vector<int> dr(n, -1);
    for (int i = 1; i < n; i++) {
        if (i == r) continue;
        dr[i] = getDistance(r, i);
        if (d0[i] == d0[r] + dr[i]) {
            // Station i is to the right of r -> candidate C
            rightC.push_back(i);
        } else {
            // Station i is to the left of 0 -> candidate D
            leftD.push_back(i);
        }
    }

    // Process rightC: sorted by distance from r
    sort(rightC.begin(), rightC.end(), [&](int a, int b) {
        return dr[a] < dr[b];
    });

    // The nearest known D-station going right is r
    // For each candidate in rightC (sorted by distance from r):
    //   - It should be type C at position location[r] + dr[i]
    //   - But verify: check if d(lastD, i) is consistent
    int lastD = r;
    for (int i : rightC) {
        int expected_pos = location[r] + dr[i];
        // Check against lastD
        int dist_check = getDistance(lastD, i);
        int direct = expected_pos - location[lastD];
        if (dist_check == direct && direct >= 0) {
            stype[i] = 1; // C
            location[i] = expected_pos;
        } else {
            // It's actually a D-station
            stype[i] = 2;
            // position = location[lastD] - (dist_check)
            // d(lastD, i): lastD is D, goes left, bounces...
            // Actually: location[i] = location[0] + d0[i] (if it's between 0 and r)
            // Reconsider: d(0,i) = d(0,r) + d(r,i) still holds
            // but i is a D-station between 0 and r
            location[i] = location[lastD] - dist_check;
            lastD = i;
        }
    }

    // Process leftD: sorted by distance from 0
    sort(leftD.begin(), leftD.end(), [&](int a, int b) {
        return d0[a] < d0[b];
    });

    int lastC = 0;
    for (int i : leftD) {
        int expected_pos = location[0] - d0[i];
        int dist_check = getDistance(lastC, i);
        int direct = location[lastC] - expected_pos;
        if (dist_check == direct && direct >= 0) {
            stype[i] = 2; // D
            location[i] = expected_pos;
        } else {
            stype[i] = 1; // C
            location[i] = location[lastC] + dist_check;
            lastC = i;
        }
    }
}

int main() {
    // Grader handles I/O
    return 0;
}
