// IOI 2010 - Hotter Colder (Interactive)
// Binary search on hidden position using distance comparisons.
// O(log N) queries.
#include <bits/stdc++.h>
using namespace std;

// Grader-provided: returns 1 (hotter), -1 (colder), 0 (same distance).
int Guess(int G);

int HC(int N) {
    int lo = 1, hi = N;
    int prev = 1; // starting position

    while (lo < hi) {
        int mid = (lo + hi) / 2;
        // Choose G so that the midpoint of (prev, G) splits [lo, hi].
        int G;
        if (prev <= mid) {
            G = 2 * mid + 1 - prev;
        } else {
            G = 2 * mid - prev;
        }
        G = max(1, min(N, G));

        int result = Guess(G);

        if (result == 0) {
            return (prev + G) / 2;
        }

        int boundary = (prev + G) / 2;
        if (result == 1) {
            // X is closer to G: on G's side of boundary.
            if (G > prev) lo = boundary + 1;
            else          hi = boundary;
        } else {
            // X is closer to prev: on prev's side of boundary.
            if (G > prev) hi = boundary;
            else          lo = boundary + 1;
        }

        prev = G;
    }

    return lo;
}

// Stub main for standalone compilation.
int main() {
    int N;
    cin >> N;
    cout << HC(N) << endl;
    return 0;
}

int Guess(int /*G*/) { return 0; }
