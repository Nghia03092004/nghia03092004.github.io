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

    vector<int> primes;
    for (int i = 2; i < LIMIT; i++)
        if (is_prime[i])
            primes.push_back(i);

    int n = primes.size();
    vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; i++)
        prefix[i + 1] = prefix[i] + primes[i];

    int best_len = 0;
    int best_prime = 0;

    for (int len = n; len > best_len; len--) {
        for (int start = 0; start + len <= n; start++) {
            long long s = prefix[start + len] - prefix[start];
            if (s >= LIMIT) break;
            if (is_prime[(int)s]) {
                best_len = len;
                best_prime = (int)s;
                break;
            }
        }
    }

    cout << best_prime << endl;
    return 0;
}
