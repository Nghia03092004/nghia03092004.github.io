#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 302: Strong Achilles Numbers below 10^18.
 *
 * Enumerate powerful numbers n < 10^18 via DFS over prime factorizations.
 * Check: (1) all exponents >= 2, (2) gcd of exponents = 1 (Achilles),
 * (3)-(4) phi(n) is also Achilles.
 */

typedef long long ll;
typedef __int128 lll;
const ll LIMIT = 1000000000000000000LL;

vector<int> primes;

void gen_primes(int lim) {
    vector<bool> sieve(lim + 1, false);
    for (int i = 2; i <= lim; i++) {
        if (!sieve[i]) {
            primes.push_back(i);
            for (ll j = (ll)i * i; j <= lim; j += i)
                sieve[j] = true;
        }
    }
}

vector<pair<int, int>> factorize(ll n) {
    vector<pair<int, int>> res;
    for (int p : primes) {
        if ((ll)p * p > n) break;
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; e++; }
            res.push_back({p, e});
        }
    }
    if (n > 1) res.push_back({(int)n, 1});
    return res;
}

unordered_map<int, vector<pair<int,int>>> pm1_cache;

const vector<pair<int,int>>& get_pm1_fac(int p) {
    auto it = pm1_cache.find(p);
    if (it != pm1_cache.end()) return it->second;
    pm1_cache[p] = factorize(p - 1);
    return pm1_cache[p];
}

ll answer = 0;

void dfs(int idx, lll current_n, int num_primes, int gcd_exp,
         unordered_map<int, int>& phi_fac) {
    if (num_primes >= 2 && gcd_exp == 1) {
        bool powerful = true;
        int phi_gcd = 0;
        for (auto& [p, e] : phi_fac) {
            if (e < 2) { powerful = false; break; }
            phi_gcd = __gcd(phi_gcd, e);
        }
        if (powerful && phi_gcd == 1)
            answer++;
    }

    for (int i = idx; i < (int)primes.size(); i++) {
        ll p = primes[i];
        if (current_n * (lll)p * p > LIMIT) break;

        const auto& pm1_fac = get_pm1_fac(p);
        lll power = (lll)p * p;
        for (int e = 2; current_n * power <= LIMIT; e++) {
            int new_gcd = (gcd_exp == 0) ? e : __gcd(gcd_exp, e);
            phi_fac[p] += (e - 1);
            for (auto& [q, f] : pm1_fac) phi_fac[q] += f;

            dfs(i + 1, current_n * power, num_primes + 1, new_gcd, phi_fac);

            for (auto& [q, f] : pm1_fac) {
                phi_fac[q] -= f;
                if (phi_fac[q] == 0) phi_fac.erase(q);
            }
            phi_fac[p] -= (e - 1);
            if (phi_fac[p] == 0) phi_fac.erase(p);

            if (power > (lll)LIMIT / p) break;
            power *= p;
        }
    }
}

int main() {
    gen_primes(1000000);
    unordered_map<int, int> phi_fac;
    dfs(0, 1, 0, 0, phi_fac);
    printf("%lld\n", answer);
    return 0;
}
