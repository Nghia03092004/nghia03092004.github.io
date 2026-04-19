#include <bits/stdc++.h>
using namespace std;

// IOI 2021 - Registers
// Use bitwise register operations to find minimum (s=0) or sort (s=1)
// n values of k bits packed in register 0 (b=2000 bit registers).

// Grader-provided functions
void append_move(int t, int x);
void append_store(int t, vector<bool> v);
void append_and(int t, int x, int y);
void append_or(int t, int x, int y);
void append_xor(int t, int x, int y);
void append_not(int t, int x);
void append_left(int t, int x, int p);
void append_right(int t, int x, int p);
void append_add(int t, int x, int y);

const int B = 2000;

void construct_instructions(int s, int n, int k, int /*q*/) {
    // Create constant masks
    vector<bool> mask_k(B, false);
    for (int i = 0; i < k; i++) mask_k[i] = true;

    vector<bool> one(B, false);
    one[0] = true;

    if (s == 0) {
        // Find minimum using pairwise tournament
        append_store(2, mask_k);  // k-bit mask

        for (int step = 1; step < n; step *= 2) {
            // Shift to align pairs
            append_right(1, 0, step * k);

            // Extract both values to k bits
            append_and(3, 0, 2);
            append_and(4, 1, 2);

            // Compare: compute r[3] + (2^k - r[4])
            // If result has bit k set, r[3] >= r[4]; otherwise r[3] < r[4]
            append_not(5, 4);
            append_and(5, 5, 2);       // k-bit complement of r[4]
            append_store(6, one);
            append_add(5, 5, 6);       // 2^k - r[4]
            append_add(5, 3, 5);       // r[3] - r[4] + 2^k

            // Extract comparison bit (bit k)
            append_right(7, 5, k);
            append_and(7, 7, 6);       // r[7] = 1 if r[3] >= r[4]

            // We want to select r[3] if r[3] < r[4] (r[7]=0), else r[4]
            append_xor(7, 7, 6);       // r[7] = 1 if r[3] < r[4]
            append_and(7, 7, 6);

            // Expand r[7] to k-bit selection mask
            append_move(8, 7);
            for (int bit = 0; (1 << bit) < k; bit++) {
                append_left(9, 8, 1 << bit);
                append_or(8, 8, 9);
            }
            append_and(8, 8, 2);

            // Select: r[0] = (r[3] & mask) | (r[4] & ~mask)
            append_and(9, 3, 8);
            append_not(10, 8);
            append_and(10, 10, 2);
            append_and(10, 4, 10);
            append_or(0, 9, 10);
        }
    } else {
        // Full sort using bubble sort with register compare-and-swap
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - 1 - i; j++) {
                // Masks for positions j and j+1
                vector<bool> mask_j(B, false), mask_j1(B, false);
                for (int b = 0; b < k; b++) {
                    mask_j[j * k + b] = true;
                    mask_j1[(j + 1) * k + b] = true;
                }
                append_store(2, mask_j);
                append_store(3, mask_j1);

                // Extract values at positions j and j+1, shift to position 0
                append_and(4, 0, 2);
                append_right(4, 4, j * k);
                append_and(5, 0, 3);
                append_right(5, 5, (j + 1) * k);

                // Constants for comparison
                vector<bool> one_v(B, false);
                one_v[0] = true;
                append_store(6, one_v);

                vector<bool> kmask(B, false);
                for (int b = 0; b < k; b++) kmask[b] = true;
                append_store(7, kmask);

                // Compare: r[4] - r[5] + 2^k
                append_not(8, 5);
                append_and(8, 8, 7);
                append_add(8, 8, 6);
                append_add(8, 4, 8);

                // Bit k: 1 if r[4] >= r[5] (need swap)
                append_right(9, 8, k);
                append_and(9, 9, 6);

                // Build conditional XOR-swap mask
                append_xor(10, 4, 5);  // diff between the two values

                // Expand swap flag to k bits
                append_move(11, 9);
                for (int bit = 0; (1 << bit) < k; bit++) {
                    append_left(12, 11, 1 << bit);
                    append_or(11, 11, 12);
                }
                append_and(11, 11, 7);
                append_and(10, 10, 11);  // conditional diff

                // Apply XOR swap to both positions
                append_left(12, 10, j * k);
                append_left(13, 10, (j + 1) * k);
                append_xor(0, 0, 12);
                append_xor(0, 0, 13);
            }
        }
    }
}
