#include <bits/stdc++.h>
using namespace std;

// Compute binomial coefficient C(n, k)
long long comb(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    long long result = 1;
    for (int i = 0; i < k; i++) {
        result = result * (n - i) / (i + 1);
    }
    return result;
}

// Catalan number C_k = C(2k,k) / (k+1)
long long catalan(int k) {
    return comb(2 * k, k) / (k + 1);
}

int main() {
    int n = 12;
    long long total = 0;

    for (int k = 2; k <= n / 2; k++) {
        long long choose_2k = comb(n, 2 * k);
        long long half_choose = comb(2 * k, k) / 2;
        long long cat_k = catalan(k);
        total += choose_2k * (half_choose - cat_k);
    }

    cout << total << endl;
    return 0;
}
