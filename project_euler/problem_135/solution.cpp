#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 1000000;
    vector<int> count(N, 0);

    // n = y*(4d - y), with y/4 < d < y, n > 0, n < N
    // Iterate over y and d
    for (int y = 1; y < N; y++) {
        int d_min = y / 4 + 1; // d > y/4
        int d_max = y - 1;     // d < y

        // Also n = y*(4d - y) < N => 4d - y < N/y => d < (N/y + y) / 4
        // But be careful with integer division
        if (y > 0) {
            long long upper = ((long long)(N - 1) / y + y);
            int d_bound = (int)(upper / 4);
            if (d_bound < d_max) d_max = d_bound;
        }

        for (int d = d_min; d <= d_max; d++) {
            int n = y * (4 * d - y);
            if (n > 0 && n < N)
                count[n]++;
        }
    }

    int result = 0;
    for (int i = 1; i < N; i++)
        if (count[i] == 10)
            result++;

    cout << result << endl;
    return 0;
}
