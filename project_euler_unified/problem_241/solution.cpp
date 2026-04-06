/*
 * Project Euler Problem 241: Perfection Quotients
 *
 * Find all n <= 10^18 with sigma(n)/n a half-integer, i.e., 2*sigma(n)/n
 * is an odd integer >= 3.  We build n by DFS over its prime factorisation,
 * tracking the ratio 2*sigma(n)/n as a reduced fraction num/den.
 */

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

static const ll LIMIT = 1000000000000000000LL; // 10^18

ll my_gcd(ll a, ll b) {
    while (b) { a %= b; swap(a, b); }
    return a;
}

vector<ll> results;
vector<int> small_primes;

bool is_prime_check(ll n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (ll i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}

void search(ll n_val, ll num, ll den, ll min_prime) {
    if (den == 1 && num % 2 == 1 && num >= 3 && n_val > 1)
        results.push_back(n_val);

    if (num > 200LL * den) return;

    ll max_p = LIMIT / n_val;
    if (max_p < min_prime) return;

    auto try_prime = [&](ll p) {
        if (p < min_prime || p > max_p) return;
        ll pe = 1;
        ll sig_pe = 1;
        for (int e = 1; pe <= max_p / p; e++) {
            pe *= p;
            sig_pe += pe;
            ll new_n = n_val * pe;
            if (new_n > LIMIT) break;

            ll g1 = my_gcd(num, pe);
            ll g2 = my_gcd(sig_pe, den);
            ll a_num = num / g1, a_pe = pe / g1;
            ll a_sig = sig_pe / g2, a_den = den / g2;

            if (a_num > 0 && a_sig > (ll)4e18 / a_num) break;
            ll new_num = a_num * a_sig;
            if (a_den > 0 && a_pe > (ll)4e18 / a_den) break;
            ll new_den = a_den * a_pe;

            ll g3 = my_gcd(new_num, new_den);
            new_num /= g3;
            new_den /= g3;

            search(new_n, new_num, new_den, p + 1);
        }
    };

    for (int p : small_primes) {
        if (p > max_p) break;
        if (p < min_prime) continue;
        try_prime(p);
    }

    if (den > 1) {
        set<ll> candidates;
        for (ll d = 1; d * d <= den; d++) {
            if (den % d == 0) {
                for (ll c : {d - 1, d + 1, 2*d - 1, 2*d + 1,
                             den/d - 1, den/d + 1, 2*(den/d)-1, 2*(den/d)+1}) {
                    if (c >= min_prime && c <= max_p && c > 2000000)
                        candidates.insert(c);
                }
            }
        }
        for (ll c : candidates)
            if (is_prime_check(c))
                try_prime(c);
    }
}

int main() {
    ios_base::sync_with_stdio(false);

    vector<bool> sieve(2000001, true);
    for (int i = 2; i * i <= 2000000; i++)
        if (sieve[i])
            for (int j = i*i; j <= 2000000; j += i)
                sieve[j] = false;
    for (int i = 2; i <= 2000000; i++)
        if (sieve[i]) small_primes.push_back(i);

    search(1, 2, 1, 2);

    ll ans = 0;
    for (ll x : results) ans += x;
    cout << ans << endl;
    return 0;
}
