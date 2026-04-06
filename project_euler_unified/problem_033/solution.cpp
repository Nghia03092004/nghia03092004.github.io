#include <bits/stdc++.h>
using namespace std;

int main() {
    int num_prod = 1, den_prod = 1;
    for (int a = 1; a <= 9; a++) {
        for (int b = 1; b <= 9; b++) {
            int denom = 9 * a + b;
            int numer = 10 * a * b;
            if (numer % denom != 0) continue;
            int e = numer / denom;
            if (e < 1 || e > 9) continue;
            if (10 * a + b >= 10 * b + e) continue;
            num_prod *= a;
            den_prod *= e;
        }
    }
    cout << den_prod / __gcd(num_prod, den_prod) << endl;  // 100
    return 0;
}
