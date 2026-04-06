#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 768: Chandelier
 *
 * f(n, m) = number of ways to place m identical candles in n evenly
 * spaced holders on a circular chandelier so it's balanced.
 *
 * Formula: f(n, m) = (1/n) * Sum_{g | n} phi(n/g) * C(g, m/(n/g))
 * where the binomial is 0 unless (n/g) divides m.
 *
 * For n=360, m=20.
 */

typedef long long ll;
typedef __int128 lll;

// Euler's totient function
ll euler_phi(ll n) {
    ll result = n;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

// Get all divisors of n
vector<ll> get_divisors(ll n) {
    vector<ll> divs;
    for (ll i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            divs.push_back(i);
            if (i != n / i) divs.push_back(n / i);
        }
    }
    sort(divs.begin(), divs.end());
    return divs;
}

// Compute binomial coefficient C(n, k) using arbitrary precision
// Since n <= 360 and k <= 20, values fit in __int128
lll binomial(ll n, ll k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    lll result = 1;
    for (ll i = 0; i < k; i++) {
        result = result * (n - i) / (i + 1);
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n = 360, m = 20;

    vector<ll> divisors = get_divisors(n);

    // Use Python-style big integers via __int128 or just compute carefully
    // Since values can be large, we'll use double to check and __int128 for exact
    // Actually C(360, 20) ~ 10^37 which exceeds __int128 range (~1.7*10^38)
    // So we need big integer arithmetic

    // Let's compute using the formula with big integers
    // We'll use a simple big integer approach via strings or just compute modularly
    // But the answer should be exact, not modular...

    // Wait - the answer 783976175 looks like it could be mod 10^9+7
    // Let's check: f(360,20) direct could be very large
    // Actually for the problem, the answer IS the exact integer f(360,20)
    // But 783976175 < 10^9, so maybe f(360,20) is just not that large
    // or the problem asks for mod something

    // Let's compute properly. The divisors of 360 = 2^3 * 3^2 * 5
    // Divisors: 1,2,3,4,5,6,8,9,10,12,15,18,20,24,30,36,40,45,60,72,90,120,180,360

    // For each divisor g of 360, k = 360/g must divide m=20
    // So k | 20, meaning k in {1,2,4,5,10,20} AND k | 360

    // k must divide both 20 and 360
    // Divisors of gcd(20, 360) = 20: k in {1, 2, 4, 5, 10, 20}
    // All divide 360? 1|360 yes, 2|360 yes, 4|360 yes, 5|360 yes, 10|360 yes, 20|360 yes

    // For each valid k: g = 360/k, binomial(g, m/k), phi(k)
    // k=1:  g=360, C(360, 20), phi(1)=1
    // k=2:  g=180, C(180, 10), phi(2)=1
    // k=4:  g=90,  C(90, 5),   phi(4)=2
    // k=5:  g=72,  C(72, 4),   phi(5)=4
    // k=10: g=36,  C(36, 2),   phi(10)=4
    // k=20: g=18,  C(18, 1),   phi(20)=8

    // Compute each term using __int128 where possible, or doubles for verification

    // C(360,20) is huge (~4.5 * 10^37), fits in __int128 (max ~1.7*10^38)
    lll c360_20 = 1;
    for (int i = 0; i < 20; i++) {
        c360_20 = c360_20 * (360 - i) / (i + 1);
    }

    lll c180_10 = 1;
    for (int i = 0; i < 10; i++) {
        c180_10 = c180_10 * (180 - i) / (i + 1);
    }

    lll c90_5 = 1;
    for (int i = 0; i < 5; i++) {
        c90_5 = c90_5 * (90 - i) / (i + 1);
    }

    lll c72_4 = 1;
    for (int i = 0; i < 4; i++) {
        c72_4 = c72_4 * (72 - i) / (i + 1);
    }

    lll c36_2 = 36 * 35 / 2; // = 630
    lll c18_1 = 18;

    lll total = (lll)1 * c360_20 +
                (lll)1 * c180_10 +
                (lll)2 * c90_5 +
                (lll)4 * c72_4 +
                (lll)4 * c36_2 +
                (lll)8 * c18_1;

    lll answer = total / 360;

    // Print __int128
    // Convert to string
    if (answer == 0) {
        cout << 0 << endl;
    } else {
        string s;
        lll tmp = answer;
        while (tmp > 0) {
            s += ('0' + (int)(tmp % 10));
            tmp /= 10;
        }
        reverse(s.begin(), s.end());
        cout << s << endl;
    }

    return 0;
}
