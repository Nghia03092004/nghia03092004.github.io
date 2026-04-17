/*
 * Project Euler 441: The Inverse Summation of Coprime Couples
 *
 * S(N) = sum_{M=2}^{N} R(M), where
 *   R(M) = sum_{1<=p<q<=M, p+q>=M, gcd(p,q)=1} 1/(pq).
 *
 * Via Mobius inversion and harmonic-number precomputation, we evaluate
 * S(10^7) in O(N log N) time.
 */
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

static const int MAXN = 10000001;

int mu[MAXN];
double H[MAXN];

void sieve_mobius() {
    vector<int> primes;
    vector<bool> is_comp(MAXN, false);
    mu[1] = 1;
    for (int i = 2; i < MAXN; i++) {
        if (!is_comp[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int j = 0; j < (int)primes.size() && (long long)i * primes[j] < MAXN; j++) {
            is_comp[i * primes[j]] = true;
            if (i % primes[j] == 0) {
                mu[i * primes[j]] = 0;
                break;
            }
            mu[i * primes[j]] = -mu[i];
        }
    }
}

void precompute_harmonics() {
    H[0] = 0.0;
    for (int i = 1; i < MAXN; i++)
        H[i] = H[i - 1] + 1.0 / i;
}

int main() {
    const int N = 10000000;
    sieve_mobius();
    precompute_harmonics();

    double ans = 0.0;

    for (int d = 1; d < MAXN; d++) {
        if (mu[d] == 0) continue;
        int Q = N / d;
        if (Q < 2) break;

        double contrib = 0.0;
        for (int q = 2; q <= Q; q++) {
            int pmax = q - 1;
            int boundary = Q - q;

            // Full pairs: p' in [1, min(pmax, boundary)]
            if (boundary >= 1) {
                int pf = min(pmax, boundary);
                contrib += (double)pf / ((double)d * q)
                         + H[pf] / ((double)d * d * q);
            }

            // Partial pairs: p' in [max(1, boundary+1), pmax]
            int ps = max(1, boundary + 1);
            if (ps <= pmax) {
                double factor = N - (double)d * q + 1.0;
                double harm = H[pmax] - H[ps - 1];
                contrib += factor * harm / ((double)d * d * q);
            }
        }

        ans += mu[d] * contrib;
    }

    printf("%.4f\n", ans);
    return 0;
}
