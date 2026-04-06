#include <bits/stdc++.h>
using namespace std;

const long long MOD = 500500507;
const int TARGET = 500500;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    // Sieve primes up to ~7.6 million (need about 500500 primes)
    const int LIMIT = 7800000;
    vector<bool> is_prime(LIMIT + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIMIT; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    vector<int> primes;
    for (int i = 2; i <= LIMIT; i++) {
        if (is_prime[i]) primes.push_back(i);
    }

    // Min-heap: (value, base_prime)
    // We store log values for comparison, actual modular product separately
    // Actually, we can use double for comparison and track modular arithmetic
    // Better: use a priority queue of (log_value, prime, current_power_of_two)

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

    int prime_idx = 0;
    // Push first prime
    pq.push({log((double)primes[0]), 0});
    prime_idx = 1;

    long long answer = 1;

    for (int i = 0; i < TARGET; i++) {
        // Ensure we have enough primes in the queue
        while (prime_idx < (int)primes.size() &&
               (pq.empty() || log((double)primes[prime_idx]) <= pq.top().first + 1e-9 || prime_idx <= i + 1)) {
            pq.push({log((double)primes[prime_idx]), prime_idx});
            prime_idx++;
        }

        auto [log_val, idx] = pq.top();
        pq.pop();

        // Multiply answer by primes[idx]^(2^j) mod MOD
        // The log_val encodes the actual value; the modular multiplication:
        // We need to compute primes[idx]^(round(exp(log_val)/1)) but that's imprecise.
        // Better approach: store the actual prime and exponent level

        // Actually, let's redo this properly
        // Store (log_value, prime_index, level) where the actual multiplier is prime^(2^level)
        // and log_value = 2^level * log(prime)

        // For now, compute modular value from prime and level
        // We need to track level. Let me restart with a cleaner approach.
        break;
    }

    // Clean approach: store (log_value, prime, level)
    // where multiplier = prime^(2^level), log_value = 2^level * log(prime)

    struct Entry {
        double log_val;
        int prime_idx;
        int level;
        bool operator>(const Entry& o) const { return log_val > o.log_val; }
    };

    priority_queue<Entry, vector<Entry>, greater<Entry>> pq2;

    for (int i = 0; i < min((int)primes.size(), TARGET + 100); i++) {
        pq2.push({log((double)primes[i]), i, 0});
    }

    answer = 1;
    for (int i = 0; i < TARGET; i++) {
        Entry e = pq2.top();
        pq2.pop();

        // Multiplier is primes[e.prime_idx]^(2^e.level)
        long long mult = power(primes[e.prime_idx], 1LL << e.level, MOD);
        answer = answer * mult % MOD;

        // Push next level
        pq2.push({e.log_val * 2, e.prime_idx, e.level + 1});
    }

    cout << answer << endl;

    return 0;
}
