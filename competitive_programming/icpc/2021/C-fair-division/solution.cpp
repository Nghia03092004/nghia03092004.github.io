#include <bits/stdc++.h>
using namespace std;

namespace {

long long n;
long long m;

long long gcd_ll(long long a, long long b) {
    while (b != 0) {
        long long r = a % b;
        a = b;
        b = r;
    }
    return a;
}

long long capped_mul(long long a, long long b, long long cap) {
    if (a == 0 || b == 0) {
        return 0;
    }
    if (a > cap / b) {
        return cap + 1;
    }
    return a * b;
}

long long capped_divisor(long long q, long long rem_num, long long rounds, long long cap) {
    long long sum = 1;
    long long power = 1;

    for (long long i = 1; i < rounds; ++i) {
        power = capped_mul(power, rem_num, cap);
        if (power > cap) {
            return cap + 1;
        }

        long long scaled = capped_mul(sum, q, cap);
        if (scaled > cap || scaled > cap - power) {
            return cap + 1;
        }

        sum = scaled + power;
    }

    return sum;
}

void solve() {
    cin >> n >> m;

    if (n >= 60) {
        cout << "impossible\n";
        return;
    }

    for (long long q = 2;; ++q) {
        long long smallest_divisor = capped_divisor(q, 1, n, m);
        if (smallest_divisor > m) {
            break;
        }

        for (long long p = 1; p < q; ++p) {
            if (gcd_ll(p, q) != 1) {
                continue;
            }

            long long rem_num = q - p;
            long long divisor = capped_divisor(q, rem_num, n, m);
            if (divisor <= m && m % divisor == 0) {
                cout << p << ' ' << q << '\n';
                return;
            }
        }
    }

    cout << "impossible\n";
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
