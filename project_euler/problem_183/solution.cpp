#include <bits/stdc++.h>
using namespace std;

int main() {
    const double E = exp(1.0);
    long long total = 0;

    for (int N = 5; N <= 10000; N++) {
        int k1 = (int)(N / E);
        int k2 = k1 + 1;
        if (k1 < 1) k1 = 1;

        double v1 = k1 * log((double)N / k1);
        double v2 = k2 * log((double)N / k2);
        int k = (v1 >= v2) ? k1 : k2;

        int g = __gcd(N, k);
        int d = k / g;
        while (d % 2 == 0) d /= 2;
        while (d % 5 == 0) d /= 5;

        if (d == 1)
            total -= N;
        else
            total += N;
    }

    cout << total << endl;
    return 0;
}
