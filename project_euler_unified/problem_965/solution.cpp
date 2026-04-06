#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 10000;
    long long total_x = 0;
    for (int n = 2; n <= LIMIT; n++) {
        bool found = false;
        for (int x = (n + 3) / 4; x <= 3 * n && !found; x++) {
            long long num = 4LL * x - n;
            long long den = (long long)n * x;
            if (num <= 0) continue;
            int y_min = max((long long)x, (den + num - 1) / num);
            int y_max = 2 * den / num;
            for (int y = y_min; y <= y_max && !found; y++) {
                long long z_num = num * y - den;
                long long z_den = den * (long long)y;
                if (z_num > 0 && z_den % z_num == 0) {
                    long long z = z_den / z_num;
                    if (z >= y) {
                        total_x += x;
                        found = true;
                    }
                }
            }
        }
    }
    cout << total_x << endl;
    return 0;
}
