#include <bits/stdc++.h>
using namespace std;

// Grader functions
void move_inside(int i);
void move_outside(int i);
int press_button();

int min_cardinality(int N) {
    // Step 1: Find number of distinct types
    vector<bool> inside(N, false);
    int num_types = 0;

    for (int i = 0; i < N; i++) {
        move_inside(i);
        if (press_button() > 1) {
            move_outside(i);
        } else {
            inside[i] = true;
            num_types++;
        }
    }

    // Clear machine
    for (int i = 0; i < N; i++)
        if (inside[i]) {
            move_outside(i);
            inside[i] = false;
        }

    // Step 2: Binary search on answer k
    int lo = 1, hi = N / num_types;

    while (lo < hi) {
        int mid = (lo + hi) / 2;

        // Check: is minimum frequency <= mid?
        // Add insects allowing at most 'mid' per type
        int total_inside = 0;
        vector<bool> in_machine(N, false);

        for (int i = 0; i < N; i++) {
            move_inside(i);
            total_inside++;
            if (press_button() > mid) {
                move_outside(i);
                total_inside--;
            } else {
                in_machine[i] = true;
            }
        }

        // If total_inside < num_types * mid, some type has < mid insects
        bool has_rare = (total_inside < (long long)num_types * mid);

        // Clean up
        for (int i = 0; i < N; i++)
            if (in_machine[i]) {
                move_outside(i);
                in_machine[i] = false;
            }

        if (has_rare) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    return lo;
}
