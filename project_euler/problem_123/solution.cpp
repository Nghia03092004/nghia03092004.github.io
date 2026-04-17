#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 300000;
    vector<bool> is_prime(LIMIT + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= LIMIT; i++)
        if (is_prime[i])
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    vector<int> primes;
    for (int i = 2; i <= LIMIT; i++)
        if (is_prime[i]) primes.push_back(i);

    const long long threshold = 10000000000LL;

    for (int i = 0; i < (int)primes.size(); i++) {
        int n = i + 1;
        if (n % 2 == 0) continue;
        long long p = primes[i];
        if (2LL * n * p > threshold) {
            cout << n << endl;
            return 0;
        }
    }
    return 0;
}
