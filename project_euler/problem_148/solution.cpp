#include <bits/stdc++.h>
using namespace std;

int main() {
    // Count entries in first N rows of Pascal's triangle not divisible by 7.
    // By Lucas' theorem: C(n,k) not divisible by 7 iff each base-7 digit of k <= that of n.
    // Row n contributes product(d_i + 1) where d_i are base-7 digits of n.
    //
    // S(N) = sum_{n=0}^{N-1} product(d_i(n)+1)
    // Recursion: S(d * 7^k + R) = d*(d+1)/2 * 28^k + (d+1) * S(R)
    // Process from LSB to MSB.

    long long N = 1000000000LL;

    // Convert N to base 7 (LSB first)
    vector<int> digits;
    {
        long long tmp = N;
        while (tmp > 0) {
            digits.push_back(tmp % 7);
            tmp /= 7;
        }
    }

    // Process from LSB (i=0) to MSB
    long long result = 0;
    long long pow28 = 1; // 28^i
    for (int i = 0; i < (int)digits.size(); i++) {
        int d = digits[i];
        result = (long long)d * (d + 1) / 2 * pow28 + (long long)(d + 1) * result;
        pow28 *= 28;
    }

    cout << result << endl;
    return 0;
}
