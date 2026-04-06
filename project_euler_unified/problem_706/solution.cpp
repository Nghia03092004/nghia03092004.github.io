#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007;

// State: (c0 mod 3, c1 mod 3, c2 mod 3) -> 27 states
// But we also need to know current prefix sum mod 3 to determine which counter to increment.
// current prefix sum mod 3 is determined by which c_r we last incremented.
// Actually, we need to track prefix_sum mod 3 explicitly as part of the state.
// State: (c0 mod 3, c1 mod 3, c2 mod 3, prefix_sum mod 3) but prefix_sum mod 3
// is not independent - no, it tells us which counter gets incremented next.
// Actually prefix_sum is the current S_k mod 3. When we add digit d, new S = (S + d) mod 3,
// and we increment c_{new_S}. So we need to track current S mod 3.
// Total states: 27 * 3 = 81

typedef vector<vector<long long>> Matrix;

int stateIndex(int c0, int c1, int c2, int s) {
    return ((c0 * 3 + c1) * 3 + c2) * 3 + s;
}

Matrix multiply(const Matrix& A, const Matrix& B, int n) {
    Matrix C(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            if (A[i][k] == 0) continue;
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
        }
    return C;
}

Matrix matpow(Matrix M, long long p, int n) {
    Matrix result(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1;
    while (p > 0) {
        if (p & 1) result = multiply(result, M, n);
        M = multiply(M, M, n);
        p >>= 1;
    }
    return result;
}

int main() {
    const int N = 81; // 27 * 3
    const long long D = 100000; // 10^5 digits

    // Build transition matrix
    Matrix T(N, vector<long long>(N, 0));

    // digits mod 3: 0,3,6,9 -> residue 0 (4 digits)
    //               1,4,7   -> residue 1 (3 digits)
    //               2,5,8   -> residue 2 (3 digits)
    int digitCount[3] = {4, 3, 3};

    for (int c0 = 0; c0 < 3; c0++)
        for (int c1 = 0; c1 < 3; c1++)
            for (int c2 = 0; c2 < 3; c2++)
                for (int s = 0; s < 3; s++) {
                    int from = stateIndex(c0, c1, c2, s);
                    // Add a digit with residue r (0,1,2)
                    for (int r = 0; r < 3; r++) {
                        int newS = (s + r) % 3;
                        int nc0 = c0, nc1 = c1, nc2 = c2;
                        if (newS == 0) nc0 = (nc0 + 1) % 3;
                        else if (newS == 1) nc1 = (nc1 + 1) % 3;
                        else nc2 = (nc2 + 1) % 3;
                        int to = stateIndex(nc0, nc1, nc2, newS);
                        T[to][from] = (T[to][from] + digitCount[r]) % MOD;
                    }
                }

    // Initial state: c0=1 (for S_0=0), c1=0, c2=0, prefix_sum=0
    // But first digit can't be 0 (d-digit number), so we handle first digit separately
    // First digit: 1-9
    // digit residues for 1-9: residue 0: {3,6,9} -> 3 digits
    //                          residue 1: {1,4,7} -> 3 digits
    //                          residue 2: {2,5,8} -> 3 digits

    vector<long long> state(N, 0);
    // Initial: c0=1 (S_0=0 counted), c1=0, c2=0, current prefix_sum=0
    // After first digit d with residue r:
    // new prefix_sum = r, increment c_r
    for (int r = 0; r < 3; r++) {
        int c0 = 1, c1 = 0, c2 = 0;
        int newS = r;
        if (newS == 0) c0 = (c0 + 1) % 3; // c0 becomes 2
        else if (newS == 1) c1 = 1;
        else c2 = 1;
        int idx = stateIndex(c0 % 3, c1, c2, newS);
        int cnt = 3; // 3 digits for each residue among 1-9
        state[idx] = (state[idx] + cnt) % MOD;
    }

    // Apply transition matrix D-1 times for remaining digits
    if (D > 1) {
        Matrix Tp = matpow(T, D - 1, N);
        vector<long long> newState(N, 0);
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                newState[i] = (newState[i] + Tp[i][j] * state[j]) % MOD;
        state = newState;
    }

    // Sum states where binom(c0,2) + binom(c1,2) + binom(c2,2) = 0 mod 3
    // binom_mod3[c mod 3]: c=0->0, c=1->0, c=2->1
    int binom_mod3[3] = {0, 0, 1};

    long long answer = 0;
    for (int c0 = 0; c0 < 3; c0++)
        for (int c1 = 0; c1 < 3; c1++)
            for (int c2 = 0; c2 < 3; c2++) {
                int val = (binom_mod3[c0] + binom_mod3[c1] + binom_mod3[c2]) % 3;
                if (val == 0) {
                    for (int s = 0; s < 3; s++) {
                        int idx = stateIndex(c0, c1, c2, s);
                        answer = (answer + state[idx]) % MOD;
                    }
                }
            }

    cout << answer << endl;
    return 0;
}
