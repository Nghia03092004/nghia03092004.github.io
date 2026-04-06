#include <bits/stdc++.h>
using namespace std;

int digit_sum(long long n) {
    int s = 0;
    while (n > 0) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

int main() {
    const long long LIMIT = 1e18;
    vector<long long> results;

    for (int s = 2; s <= 500; s++) {
        long long power = (long long)s * s; // s^2
        for (int k = 2; power <= LIMIT && power > 0; k++) {
            if (power >= 10 && digit_sum(power) == s) {
                results.push_back(power);
            }
            // Check for overflow before multiplying
            if (power > LIMIT / s) break;
            power *= s;
        }
    }

    sort(results.begin(), results.end());
    // Remove duplicates (e.g., 64 = 8^2 but also 4^3 -- digit sums differ so no dup expected, but just in case)
    results.erase(unique(results.begin(), results.end()), results.end());

    // a_1 is first, a_30 is index 29
    if ((int)results.size() >= 30) {
        cout << results[29] << endl;
    }

    return 0;
}
