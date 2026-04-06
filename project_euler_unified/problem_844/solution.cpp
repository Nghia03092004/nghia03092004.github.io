#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef vector<vector<ll>> Mat;

const ll MOD = 1e9 + 7;

Mat mat_mul(const Mat& A, const Mat& B, ll mod) {
    int n = A.size(), m = B[0].size(), k = B.size();
    Mat C(n, vector<ll>(m, 0));
    for (int i = 0; i < n; i++)
        for (int l = 0; l < k; l++) if (A[i][l])
            for (int j = 0; j < m; j++)
                C[i][j] = (C[i][j] + A[i][l] * B[l][j]) % mod;
    return C;
}

Mat mat_pow(Mat M, ll p, ll mod) {
    int n = M.size();
    Mat R(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) R[i][i] = 1;
    while (p > 0) {
        if (p & 1) R = mat_mul(R, M, mod);
        M = mat_mul(M, M, mod);
        p >>= 1;
    }
    return R;
}

ll count_sequences(int sigma, int k, ll n) {
    int states = 1;
    for (int i = 0; i < k; i++) states *= sigma;

    Mat M(states, vector<ll>(states, 0));
    for (int s = 0; s < states; s++) {
        // Decode k-gram
        vector<int> ds(k);
        int v = s;
        for (int i = k-1; i >= 0; i--) { ds[i] = v % sigma; v /= sigma; }
        for (int c = 0; c < sigma; c++) {
            int t = 0;
            for (int i = 1; i < k; i++) t = t * sigma + ds[i];
            t = t * sigma + c;
            M[s][t] = 1;
        }
    }

    if (n < k) {
        ll ans = 1;
        for (ll i = 0; i < n; i++) ans = ans * sigma % MOD;
        return ans;
    }

    Mat Mp = mat_pow(M, n - k, MOD);
    ll total = 0;
    for (int i = 0; i < states; i++)
        for (int j = 0; j < states; j++)
            total = (total + Mp[i][j]) % MOD;
    return total;
}

int main() {
    // Verify: sigma=2, k=1, n=10 => 1024
    assert(count_sequences(2, 1, 10) == 1024);
    // sigma=3, k=1, n=8 => 6561
    assert(count_sequences(3, 1, 8) == 6561);

    cout << count_sequences(2, 2, 100) << endl;
    return 0;
}
