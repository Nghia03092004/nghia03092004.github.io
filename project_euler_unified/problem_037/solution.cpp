#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 1000000;
    vector<bool> is_prime(LIMIT, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i < LIMIT; i++)
        if (is_prime[i])
            for (int j = i * i; j < LIMIT; j += i)
                is_prime[j] = false;

    auto isRightTruncatable = [&](int n) -> bool {
        while (n >= 10) {
            n /= 10;
            if (n >= LIMIT || !is_prime[n]) return false;
        }
        return is_prime[n];
    };

    auto isLeftTruncatable = [&](int n) -> bool {
        int pow = 10;
        while (pow < n) {
            if (!is_prime[n % pow]) return false;
            pow *= 10;
        }
        return true;
    };

    int count = 0;
    long long total = 0;
    for (int i = 11; i < LIMIT && count < 11; i++) {
        if (!is_prime[i]) continue;
        if (isRightTruncatable(i) && isLeftTruncatable(i)) {
            total += i;
            count++;
        }
    }

    cout << total << endl;
    return 0;
}
