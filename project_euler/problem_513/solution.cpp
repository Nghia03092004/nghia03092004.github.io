#include <bits/stdc++.h>
using namespace std;

int main() {
    // Count triangles (a <= b <= c <= N) with integer median m_a
    // m_a = (1/2)*sqrt(2b^2 + 2c^2 - a^2), need this to be a positive integer
    // => 2b^2 + 2c^2 - a^2 = (2m)^2 for some positive integer m

    long long N;
    cout << "Enter N: ";
    cin >> N;

    long long count = 0;
    for (long long b = 1; b <= N; b++) {
        for (long long c = b; c <= N; c++) {
            long long base = 2LL * b * b + 2LL * c * c;
            long long a_min = max(1LL, c - b + 1);
            long long a_max = b;
            for (long long a = a_min; a <= a_max; a++) {
                long long val = base - a * a;
                if (val <= 0) continue;
                long long sq = (long long)sqrt((double)val);
                // Adjust for floating-point errors
                while (sq * sq > val) sq--;
                while ((sq + 1) * (sq + 1) <= val) sq++;
                if (sq * sq == val && sq % 2 == 0) {
                    count++;
                }
            }
        }
    }

    cout << "T(" << N << ") = " << count << endl;
    return 0;
}
