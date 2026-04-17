#include <bits/stdc++.h>
using namespace std;

long long power_mod(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (__int128)result * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    const int LIMIT = 1100000; // need primes a bit above 1000003
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

    long long total = 0;

    for (int i = 0; i + 1 < (int)primes.size(); i++) {
        int p1 = primes[i];
        int p2 = primes[i + 1];
        if (p1 < 5) continue;
        if (p1 >= 1000003) break;

        // Number of digits of p1
        long long pow10 = 1;
        int temp = p1;
        while (temp > 0) {
            pow10 *= 10;
            temp /= 10;
        }

        // k = (-p1 * modinv(pow10, p2)) mod p2
        long long inv = power_mod(pow10, p2 - 2, p2);
        long long k = (long long)(-(long long)p1 % p2 + p2) % p2 * inv % p2;

        long long S = p1 + k * pow10;
        total += S;
    }

    cout << total << endl;
    return 0;
}
