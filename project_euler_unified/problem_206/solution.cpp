#include <bits/stdc++.h>
using namespace std;

int main() {
    // We need n such that n^2 has the form 1_2_3_4_5_6_7_8_9_0
    // n must end in 30 or 70 (since n divisible by 10, and (n/10)^2 ends in 9)
    // n^2 has 19 digits, so n is roughly 1.01e9 to 1.39e9

    // Use __int128 or just unsigned long long (19 digits fits in ull)
    // max n^2 ~ 1.93e18 which fits in unsigned long long (max ~1.84e19)

    auto check = [](long long n) -> bool {
        long long sq = n * n;
        // Check pattern: digits at positions (from right):
        // pos 0 -> 0, pos 2 -> 9, pos 4 -> 8, ..., pos 18 -> 1
        // i.e., (sq / 10^(2k)) % 10 == (10 - k) for k=0..9, with digit 10-0=10->0 special
        // Actually: the pattern from left is 1_2_3_4_5_6_7_8_9_0
        // From right (position 0 = units): pos 0 = 0, pos 2 = 9, pos 4 = 8, ...
        // pos 2*i should be (10 - i) % 10 for i = 0..9

        for (int i = 0; i <= 9; i++) {
            int digit = sq % 10;
            int expected = (10 - i) % 10;
            if (digit != expected) return false;
            sq /= 100; // skip one digit (the underscore)
        }
        return true;
    };

    // Search range
    long long lo = (long long)ceil(sqrt(1020304050607080900.0));
    long long hi = (long long)floor(sqrt(1929394959697989990.0));

    // Adjust lo to end in 30 or 70
    for (long long n = lo; n <= hi; n++) {
        int r = n % 100;
        if (r == 30 || r == 70) {
            lo = n;
            break;
        }
    }

    for (long long n = lo; n <= hi; n += 10) {
        // n ends in X0, we want X=3 or X=7
        int tens = (n / 10) % 10;
        if (tens != 3 && tens != 7) continue;

        if (check(n)) {
            cout << n << endl;
            return 0;
        }
    }

    return 0;
}
