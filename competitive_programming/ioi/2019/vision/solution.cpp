#include <vector>

#if defined(__has_include)
#if __has_include("vision.h")
#include "vision.h"
#else
int add_and(std::vector<int> inputs);
int add_or(std::vector<int> inputs);
int add_xor(std::vector<int> inputs);
int add_not(int input);
#endif
#else
#include "vision.h"
#endif

using namespace std;

void construct_network(int H, int W, int K) {
    // Cell (r, c) has wire index r * W + c.
    int num_cells = H * W;

    // Compute R[i] = OR of all cells in row i
    vector<int> R(H);
    for (int i = 0; i < H; i++) {
        vector<int> row_cells;
        for (int j = 0; j < W; j++)
            row_cells.push_back(i * W + j);
        R[i] = add_or(row_cells);
    }

    // Compute C[j] = OR of all cells in column j
    vector<int> C(W);
    for (int j = 0; j < W; j++) {
        vector<int> col_cells;
        for (int i = 0; i < H; i++)
            col_cells.push_back(i * W + j);
        C[j] = add_or(col_cells);
    }

    // For row distance d: S_R(d) = OR of (R[i] AND R[i+d]) for all valid i
    // For column distance d: S_C(d) = OR of (C[j] AND C[j+d]) for all valid j

    // We need: OR over d_r + d_c = K of (S_R(d_r) AND S_C(d_c))

    // Compute S_R[d] for d = 0..min(K, H-1)
    // S_R[0]: at least one row has 2 cells -> use XOR trick
    // Actually R[i] = OR, so if both cells in same row, R[i] = 1 for one row.
    // Two cells in same row: exactly one R[i] is 1.
    // Two cells in different rows: exactly two R[i] are 1.
    // S_R(0) = 1 iff both cells share a row.
    // S_R(0) = NOT(XOR of all R[i])... no.
    // If exactly one R[i]=1: both cells in same row, d_r=0.
    // If exactly two R[i]=1: cells in different rows.
    // XOR of all R[i]: 1 if odd number of 1s (i.e., 1), 0 if even (i.e., 2).
    // So S_R(0) = NOT(XOR of all R[i])? No: XOR=0 means 2 ones (different rows),
    // XOR=1 means 1 one (same row). So S_R(0) = XOR of all R[i].
    // Wait, S_R(0) should be 1 if d_r=0, i.e., same row. That's XOR of all R = 1.
    // Hmm but XOR of all R when there's 1 one = 1. When there are 2 ones, XOR=0. Yes.

    // S_R(d) for d>0: OR of (R[i] AND R[i+d])
    vector<int> SR;
    // SR[0] = both in same row
    SR.push_back(add_xor(vector<int>(R.begin(), R.end()))); // 1 if same row

    for (int d = 1; d <= min(K, H - 1); d++) {
        vector<int> ands;
        for (int i = 0; i + d < H; i++) {
            ands.push_back(add_and({R[i], R[i + d]}));
        }
        SR.push_back(add_or(ands));
    }

    // SC[d] for d = 0..min(K, W-1)
    vector<int> SC;
    SC.push_back(add_xor(vector<int>(C.begin(), C.end()))); // 1 if same column

    for (int d = 1; d <= min(K, W - 1); d++) {
        vector<int> ands;
        for (int j = 0; j + d < W; j++) {
            ands.push_back(add_and({C[j], C[j + d]}));
        }
        SC.push_back(add_or(ands));
    }

    // Result: OR over d_r from 0 to K of (SR[d_r] AND SC[K - d_r])
    // where K - d_r must be in range [0, W-1] and d_r in range [0, H-1]
    vector<int> terms;
    for (int dr = 0; dr <= min(K, H - 1); dr++) {
        int dc = K - dr;
        if (dc < 0 || dc > W - 1) continue;
        if (dr >= (int)SR.size() || dc >= (int)SC.size()) continue;
        terms.push_back(add_and({SR[dr], SC[dc]}));
    }

    add_or(terms); // This becomes the output (last added gate)
}
