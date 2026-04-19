// IOI 2010 - Cluedo (Interactive)
// Elimination strategy: guess current candidates, eliminate wrong component.
// O(M + W + L) queries.
#include <bits/stdc++.h>
using namespace std;

// Grader-provided function.
// Returns: 0 if correct, 1 if murderer wrong, 2 if weapon wrong, 3 if location wrong.
int Theory(int m, int w, int l);

void Solve() {
    // Standard Cluedo sizes; adjust if grader provides different values.
    const int M = 6, W = 6, L = 9;

    vector<int> murderers, weapons, locations;
    for (int i = 1; i <= M; i++) murderers.push_back(i);
    for (int i = 1; i <= W; i++) weapons.push_back(i);
    for (int i = 1; i <= L; i++) locations.push_back(i);

    int mi = 0, wi = 0, li = 0;

    while (true) {
        int result = Theory(murderers[mi], weapons[wi], locations[li]);
        if (result == 0) return; // found the answer

        if (result == 1) {
            murderers.erase(murderers.begin() + mi);
            if (mi >= (int)murderers.size()) mi = 0;
        } else if (result == 2) {
            weapons.erase(weapons.begin() + wi);
            if (wi >= (int)weapons.size()) wi = 0;
        } else {
            locations.erase(locations.begin() + li);
            if (li >= (int)locations.size()) li = 0;
        }
    }
}

// Stub main for compilation; in contest, grader calls Solve() directly.
int main() {
    Solve();
    return 0;
}

// Stub for standalone compilation.
int Theory(int /*m*/, int /*w*/, int /*l*/) { return 0; }
