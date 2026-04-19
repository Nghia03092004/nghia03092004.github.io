#include <bits/stdc++.h>
using namespace std;

namespace {

long long mod_pow(long long base, int exp, long long mod) {
    long long result = 1 % mod;
    while (exp > 0) {
        if (exp & 1) {
            result = result * base % mod;
        }
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

long long mod_inverse(long long a, long long mod) {
    long long b = mod;
    long long x0 = 1, x1 = 0;
    while (b != 0) {
        long long q = a / b;
        long long next_a = a - q * b;
        a = b;
        b = next_a;

        long long next_x = x0 - q * x1;
        x0 = x1;
        x1 = next_x;
    }
    x0 %= mod;
    if (x0 < 0) {
        x0 += mod;
    }
    return x0;
}

void solve() {
    long long b;
    int d;
    string a;
    cin >> b >> d >> a;

    int twos = 0;
    int fives = 0;
    long long temp = b;
    while (temp % 2 == 0) {
        ++twos;
        temp /= 2;
    }
    temp = b;
    while (temp % 5 == 0) {
        ++fives;
        temp /= 5;
    }

    const int n = int(a.size());
    vector<int> suffix_cmp(n + 1, 0);
    const char wanted = char('0' + d);
    for (int len = 1; len <= n; ++len) {
        char current = a[n - len];
        if (current < wanted) {
            suffix_cmp[len] = -1;
        } else if (current > wanted) {
            suffix_cmp[len] = 1;
        } else {
            suffix_cmp[len] = suffix_cmp[len - 1];
        }
    }

    long long repeated_mod_b = 0;
    int answer = 0;

    for (int len = 1; len <= n; ++len) {
        repeated_mod_b = (repeated_mod_b * 10 + d) % b;

        long long common = 1;
        for (int i = 0; i < min(twos, len); ++i) {
            common *= 2;
        }
        for (int i = 0; i < min(fives, len); ++i) {
            common *= 5;
        }

        if (repeated_mod_b % common != 0) {
            continue;
        }

        long long modulus = b / common;
        long long prefix_residue = 0;
        if (modulus != 1) {
            long long scale = mod_pow(2, len - min(twos, len), modulus);
            scale = scale * mod_pow(5, len - min(fives, len), modulus) % modulus;
            long long inv = mod_inverse(scale, modulus);
            long long rhs = (repeated_mod_b / common) % modulus;
            prefix_residue = (-rhs * inv) % modulus;
            if (prefix_residue < 0) {
                prefix_residue += modulus;
            }
        }

        long long need_prefix = prefix_residue;
        if (d == 0 && prefix_residue == 0) {
            need_prefix = modulus;
        }

        bool borrow = suffix_cmp[len] < 0;
        int prefix_len = n - len;
        bool enough = false;

        if (prefix_len == 0) {
            if (!borrow) {
                enough = (0 >= need_prefix);
            }
        } else if (prefix_len > 7) {
            enough = true;
        } else {
            long long prefix_value = 0;
            for (int i = 0; i < prefix_len; ++i) {
                prefix_value = prefix_value * 10 + (a[i] - '0');
            }
            prefix_value -= borrow ? 1 : 0;
            enough = (prefix_value >= need_prefix);
        }

        if (enough) {
            answer = len;
        }
    }

    cout << answer << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
