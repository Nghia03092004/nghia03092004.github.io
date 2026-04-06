#include <bits/stdc++.h>
using namespace std;

// Big integer class (base 10^9) for Pell's equation
// Needed since minimal x can be ~10^37
struct BigInt {
    vector<int> d; // digits in base 10^9, least significant first

    BigInt() {}
    BigInt(long long v) {
        if (v == 0) d.push_back(0);
        while (v > 0) { d.push_back(v % 1000000000); v /= 1000000000; }
    }

    bool operator>(const BigInt& o) const {
        if (d.size() != o.d.size()) return d.size() > o.d.size();
        for (int i = (int)d.size()-1; i >= 0; i--)
            if (d[i] != o.d[i]) return d[i] > o.d[i];
        return false;
    }

    BigInt operator+(const BigInt& o) const {
        BigInt res;
        int carry = 0;
        for (int i = 0; i < (int)max(d.size(), o.d.size()) || carry; i++) {
            long long s = carry;
            if (i < (int)d.size()) s += d[i];
            if (i < (int)o.d.size()) s += o.d[i];
            res.d.push_back(s % 1000000000);
            carry = s / 1000000000;
        }
        return res;
    }

    BigInt operator*(long long v) const {
        BigInt res;
        long long carry = 0;
        for (int i = 0; i < (int)d.size() || carry; i++) {
            long long cur = carry;
            if (i < (int)d.size()) cur += (long long)d[i] * v;
            res.d.push_back(cur % 1000000000);
            carry = cur / 1000000000;
        }
        while (res.d.size() > 1 && res.d.back() == 0) res.d.pop_back();
        return res;
    }
};

// Solve Pell's equation x^2 - D*y^2 = 1 using continued fractions
// Returns the fundamental (minimal positive) solution x
BigInt solve_pell(int D) {
    int a0 = (int)sqrt((double)D);
    if (a0 * a0 == D) return BigInt(0); // perfect square

    // Find period length of continued fraction of sqrt(D)
    int m = 0, d = 1, a = a0;
    int period = 0;
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period++;
    } while (a != 2 * a0);

    // If period is even, solution is convergent p_{period-1}
    // If period is odd, solution is convergent p_{2*period-1}
    int target = (period % 2 == 0) ? period - 1 : 2 * period - 1;

    // Compute convergent p_{target}
    m = 0; d = 1; a = a0;
    BigInt pm2(1), pm1(a0); // p_{-1}=1, p_0=a0

    if (target == 0) return pm1;

    for (int i = 1; i <= target; i++) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        BigInt pn = pm1 * a + pm2;
        pm2 = pm1;
        pm1 = pn;
    }

    return pm1;
}

int main() {
    ios_base::sync_with_stdio(false);

    BigInt max_x(0);
    int best_D = 0;

    for (int D = 2; D <= 1000; D++) {
        int s = (int)sqrt((double)D);
        if (s * s == D) continue;

        BigInt x = solve_pell(D);
        if (x > max_x) {
            max_x = x;
            best_D = D;
        }
    }

    cout << best_D << endl;

    return 0;
}
