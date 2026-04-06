#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 1000000;
    vector<bool> is_prime(LIMIT + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIMIT; i++)
        if (is_prime[i])
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    auto check_prime = [&](int n) -> bool {
        if (n < 2) return false;
        if (n <= LIMIT) return is_prime[n];
        for (int i = 2; (long long)i * i <= n; i++)
            if (n % i == 0) return false;
        return true;
    };

    int best_a = 0, best_b = 0, best_count = 0;

    for (int b = 2; b <= 1000; b++) {
        if (!is_prime[b]) continue;
        for (int a = -999; a < 1000; a++) {
            int n = 0;
            while (check_prime(n * n + a * n + b))
                n++;
            if (n > best_count) {
                best_count = n;
                best_a = a;
                best_b = b;
            }
        }
    }

    cout << (long long)best_a * best_b << endl;
    return 0;
}
