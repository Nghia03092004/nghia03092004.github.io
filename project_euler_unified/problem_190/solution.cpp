#include <bits/stdc++.h>
using namespace std;

int main() {
    long long total = 0;

    for (int m = 2; m <= 15; m++) {
        // x_i = 2i / (m+1)
        // P_m = product of (2i/(m+1))^i for i=1..m
        double log_pm = 0.0;
        for (int i = 1; i <= m; i++) {
            log_pm += i * log(2.0 * i / (m + 1));
        }
        long long pm = (long long)floor(exp(log_pm));
        total += pm;
    }

    cout << total << endl;
    return 0;
}
