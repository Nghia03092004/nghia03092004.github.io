#include <bits/stdc++.h>
using namespace std;

int main() {
    // Compute C(40, 20) via the telescoping product (Theorem 3).
    // Each intermediate result * (20+k) / k is exact (Corollary 1).
    long long result = 1;
    for (int k = 1; k <= 20; k++)
        result = result * (20 + k) / k;
    cout << result << endl;
    return 0;
}
