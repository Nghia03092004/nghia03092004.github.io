#include <bits/stdc++.h>
using namespace std;

int main() {
    int count = 0;
    for (int N = 2; N <= 10000; N++) {
        int a0 = (int)sqrt((double)N);
        if (a0 * a0 == N) continue;

        int m = 0, d = 1, a = a0;
        int period = 0;
        do {
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;
            period++;
        } while (a != 2 * a0);

        if (period % 2 == 1) count++;
    }
    cout << count << endl;
    return 0;
}
