#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

// Modular exponentiation for Miller-Rabin
ull mulmod(ull a, ull b, ull m) {
    return (__uint128_t)a * b % m;
}

ull powmod(ull a, ull b, ull m) {
    ull res = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) res = mulmod(res, a, m);
        a = mulmod(a, a, m);
        b >>= 1;
    }
    return res;
}

bool miller_rabin(ull n, ull a) {
    if (n % a == 0) return n == a;
    ull d = n - 1;
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    ull x = powmod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int i = 0; i < r - 1; i++) {
        x = mulmod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime(ull n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ull a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (!miller_rabin(n, a)) return false;
    }
    return true;
}

bool contains_200(ull n) {
    string s = to_string(n);
    return s.find("200") != string::npos;
}

bool is_prime_proof(ull n) {
    string s = to_string(n);
    int len = s.size();
    for (int i = 0; i < len; i++) {
        char orig = s[i];
        for (char d = '0'; d <= '9'; d++) {
            if (d == orig) continue;
            if (i == 0 && d == '0') continue; // no leading zero
            s[i] = d;
            ull val = stoull(s);
            if (is_prime(val)) {
                s[i] = orig;
                return false;
            }
        }
        s[i] = orig;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);

    // Generate squbes: p^2 * q^3 and p^3 * q^2 for distinct primes p, q
    // Need to find enough that contain "200" and are prime-proof
    // Answer is around 2.3e11, so search up to ~1e12

    const ull LIMIT = 1000000000000ULL; // 10^12

    // Sieve primes up to cbrt(LIMIT) ~ 10000 for q, and sqrt(LIMIT/8) for p
    // Actually we need primes up to sqrt(LIMIT) ~ 10^6
    int sieve_limit = 1000001;
    vector<bool> siv(sieve_limit, true);
    siv[0] = siv[1] = false;
    for (int i = 2; i * i < sieve_limit; i++)
        if (siv[i])
            for (int j = i * i; j < sieve_limit; j += i)
                siv[j] = false;

    vector<int> primes;
    for (int i = 2; i < sieve_limit; i++)
        if (siv[i]) primes.push_back(i);

    // Generate all squbes
    set<ull> sqube_set;

    // Form p^2 * q^3
    for (int i = 0; i < (int)primes.size(); i++) {
        ull p = primes[i];
        ull p2 = p * p;
        if (p2 > LIMIT / 8) break; // q >= 2, q^3 >= 8
        for (int j = 0; j < (int)primes.size(); j++) {
            if (i == j) continue;
            ull q = primes[j];
            ull q3 = q * q * q;
            if (q3 > LIMIT / p2) break;
            ull val = p2 * q3;
            if (val <= LIMIT && contains_200(val)) {
                sqube_set.insert(val);
            }
        }
    }

    // Form p^3 * q^2
    for (int i = 0; i < (int)primes.size(); i++) {
        ull p = primes[i];
        ull p3 = p * p * p;
        if (p3 > LIMIT / 4) break; // q >= 2, q^2 >= 4
        for (int j = 0; j < (int)primes.size(); j++) {
            if (i == j) continue;
            ull q = primes[j];
            ull q2 = q * q;
            if (q2 > LIMIT / p3) break;
            ull val = p3 * q2;
            if (val <= LIMIT && contains_200(val)) {
                sqube_set.insert(val);
            }
        }
    }

    vector<ull> candidates(sqube_set.begin(), sqube_set.end());
    sort(candidates.begin(), candidates.end());

    int count = 0;
    for (ull val : candidates) {
        if (is_prime_proof(val)) {
            count++;
            if (count == 200) {
                cout << val << endl;
                return 0;
            }
        }
    }

    cout << "Not found within limit" << endl;
    return 0;
}
