#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

bool miller_rabin(ll n, ll a) {
    if (n % a == 0) return n == a;
    ll d = n - 1;
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    ll x = 1, base = a % n, dd = d;
    while (dd > 0) {
        if (dd & 1) x = (lll)x * base % n;
        base = (lll)base * base % n;
        dd >>= 1;
    }
    if (x == 1 || x == n - 1) return true;
    for (int i = 0; i < r - 1; i++) {
        x = (lll)x * x % n;
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime(ll n) {
    if (n < 2) return false;
    for (ll a : {2, 3, 5, 7, 11, 13})
        if (!miller_rabin(n, a)) return false;
    return true;
}

int main() {
    const int n = 10;
    ll total = 0;
    for (int d = 0; d <= 9; d++) {
        for (int k = n - 1; k >= 1; k--) {
            int free = n - k;
            vector<int> combo(free);
            ll sum_p = 0;
            function<void(int, int)> gen_combos = [&](int start, int idx) {
                if (idx == free) {
                    function<void(int, ll)> gen_digits = [&](int i, ll num) {
                        if (i == n) {
                            if (num >= 1000000000LL && is_prime(num))
                                sum_p += num;
                            return;
                        }
                        bool is_free = false;
                        for (int j = 0; j < free; j++)
                            if (combo[j] == i) { is_free = true; break; }
                        if (!is_free) {
                            gen_digits(i + 1, num * 10 + d);
                        } else {
                            for (int dd = 0; dd <= 9; dd++) {
                                if (dd == d) continue;
                                gen_digits(i + 1, num * 10 + dd);
                            }
                        }
                    };
                    gen_digits(0, 0);
                    return;
                }
                for (int i = start; i < n; i++) {
                    combo[idx] = i;
                    gen_combos(i + 1, idx + 1);
                }
            };
            gen_combos(0, 0);
            if (sum_p > 0) { total += sum_p; break; }
        }
    }
    cout << total << endl;
    return 0;
}
