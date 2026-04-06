#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 381: (prime-k) factorial
 *
 * For prime p, S(p) = sum of (p-k)! mod p for k=1..5.
 * Using Wilson's theorem to derive closed forms, then sieve primes.
 *
 * Answer: 139602943319822
 */

typedef long long ll;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int N = 100000000;
    vector<bool> is_prime(N, true);
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; (ll)i * i < N; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += i)
                is_prime[j] = false;
        }
    }

    // For prime p >= 5:
    // (p-1)! = p-1 mod p
    // (p-2)! = 1 mod p
    // (p-3)! = (p-1) * inv(2) mod p   [since (p-2)!/(p-2) = 1/(p-2) = -inv(2) ... let's be careful]
    // Actually:
    // (p-1)! = p-1
    // (p-2)! = (p-1)!/(p-1) = (p-1)/(p-1) = 1 ... wait, (p-1) mod p = -1
    // So (p-2)! = -1 / (p-1) = -1 * (-1) = 1 mod p. Good.
    // (p-3)! = (p-2)!/(p-2) = 1/(p-2) = inv(p-2) = -inv(2) mod p = (p - (p+1)/2) ...
    // Better: inv(p-2) = inv(-2) = -inv(2) = -(p+1)/2 mod p = (p - (p+1)/2) = (p-1)/2
    // (p-4)! = (p-3)!/(p-3) = (p-1)/2 * inv(p-3) = (p-1)/2 * (-inv(3)) = -(p-1)/(2*3) = -(p-1)*inv(6)
    // (p-5)! = (p-4)!/(p-4) = [-(p-1)*inv(6)] * inv(p-4) = [-(p-1)*inv(6)] * (-inv(4))
    //        = (p-1)*inv(24) mod p = (p-1)*inv(24)

    // S(p) = (p-1) + 1 + (p-1)/2 + (-(p-1)*inv(6)) + (p-1)*inv(24) mod p
    // S(p) = p + (p-1)*(1 + 1/2 - 1/6 + 1/24) mod p
    // Wait, let me recompute:
    // S(p) = (p-1) + 1 + (p-1)*inv(2) - (p-1)*inv(6) + (p-1)*inv(24)
    // = p + (p-1) * (inv(2) - inv(6) + inv(24))
    // = p + (p-1) * (12 - 4 + 1) * inv(24)
    // = p + (p-1) * 9 * inv(24)
    // = p + (p-1) * 3 * inv(8)
    // mod p: = 0 + (-1)*3*inv(8) = -3*inv(8) mod p = p - 3*inv(8) mod p

    // inv(8) mod p:  use Fermat's little theorem or direct
    // For p=5: inv(8) = inv(3) = 2. S = 5 - 6 = -1 = 4 mod 5.
    // Check: (4!+3!+2!+1!+0!) mod 5 = (24+6+2+1+1) mod 5 = 34 mod 5 = 4. Correct!

    ll total = 0;
    for (int p = 5; p < N; p++) {
        if (!is_prime[p]) continue;
        ll pp = p;
        // Compute inv(8) mod p
        // 8 * inv8 = 1 mod p => inv8 = (p+1)/8 if 8|(p+1), else use pow
        // Use Fermat: inv8 = pow(8, p-2, p)
        // But that's slow for every prime. Better: use the formula directly.
        // S(p) = (-3 * inv(8)) mod p
        // inv(8) = inv(2)^3. inv(2) = (p+1)/2.
        ll inv2 = (pp + 1) / 2;
        ll inv8 = inv2 % pp;
        inv8 = inv8 * inv2 % pp;
        inv8 = inv8 * inv2 % pp;
        ll s = (pp - 3 * inv8 % pp) % pp;
        total += s;
    }

    cout << total << endl;

    return 0;
}
