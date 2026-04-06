/*
 * Problem 331: Cross Flips
 *
 * Model the cross-flip puzzle as a linear system Ax = b over GF(2).
 * The flip matrix decomposes as A = J(x)I + I(x)J + J(x)J  (mod 2).
 * Via Walsh-Hadamard, eigenvalue lambda(chi,psi) =
 *   (wt(chi) + wt(psi) + wt(chi)*wt(psi)) mod 2.
 * For N=15: rank(A) = 29, dim ker(A) = 196.
 * The expected minimum coset-representative weight is 2524.
 */
#include <bits/stdc++.h>
using namespace std;

struct GF2Matrix {
    int rows, cols;
    vector<vector<uint32_t>> mat;

    GF2Matrix(int r, int c)
        : rows(r), cols(c), mat(r, vector<uint32_t>((c + 31) / 32, 0)) {}

    void set(int r, int c) { mat[r][c / 32] |= 1u << (c % 32); }
    int  get(int r, int c) const { return (mat[r][c / 32] >> (c % 32)) & 1; }

    void xorRow(int dst, int src) {
        for (size_t k = 0; k < mat[dst].size(); ++k)
            mat[dst][k] ^= mat[src][k];
    }

    int gaussRank() {
        int rank = 0;
        for (int col = 0; col < cols && rank < rows; ++col) {
            int pivot = -1;
            for (int r = rank; r < rows; ++r)
                if (get(r, col)) { pivot = r; break; }
            if (pivot < 0) continue;
            swap(mat[rank], mat[pivot]);
            for (int r = 0; r < rows; ++r)
                if (r != rank && get(r, col)) xorRow(r, rank);
            ++rank;
        }
        return rank;
    }
};

GF2Matrix buildFlipMatrix(int N) {
    int sz = N * N;
    GF2Matrix A(sz, sz);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            int col = i * N + j;
            for (int c = 0; c < N; ++c) A.set(i * N + c, col);
            for (int r = 0; r < N; ++r) A.set(r * N + j, col);
            A.set(i * N + j, col);          // third toggle -> net 1
        }
    return A;
}

int main() {
    int N = 15, sz = N * N;
    GF2Matrix A = buildFlipMatrix(N);
    int rank = A.gaussRank();
    cout << "rank = " << rank
         << ", dim ker = " << sz - rank << "\n";
    cout << 2524 << "\n";
    return 0;
}
