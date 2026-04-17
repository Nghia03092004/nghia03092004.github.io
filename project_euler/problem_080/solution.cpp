#include <bits/stdc++.h>
using namespace std;

// Simple big integer class for this problem
// We use Python-style arbitrary precision via strings and manual arithmetic
// Actually, for C++ we'll use __int128 won't suffice (need ~200 digits).
// We'll implement a simple big integer with vector<int> digits.

struct BigInt {
    vector<int> d; // digits in base 10^9, least significant first

    static const int BASE = 1000000000;

    BigInt() {}
    BigInt(long long v) {
        if (v == 0) d.push_back(0);
        while (v > 0) {
            d.push_back(v % BASE);
            v /= BASE;
        }
    }

    bool isZero() const {
        return d.empty() || (d.size() == 1 && d[0] == 0);
    }

    // Compare: -1, 0, 1
    int cmp(const BigInt& o) const {
        if (d.size() != o.d.size()) return d.size() < o.d.size() ? -1 : 1;
        for (int i = (int)d.size() - 1; i >= 0; i--) {
            if (d[i] != o.d[i]) return d[i] < o.d[i] ? -1 : 1;
        }
        return 0;
    }

    bool operator>=(const BigInt& o) const { return cmp(o) >= 0; }
    bool operator>(const BigInt& o) const { return cmp(o) > 0; }
    bool operator<(const BigInt& o) const { return cmp(o) < 0; }
    bool operator==(const BigInt& o) const { return cmp(o) == 0; }

    BigInt operator+(const BigInt& o) const {
        BigInt res;
        int carry = 0;
        int n = max(d.size(), o.d.size());
        res.d.resize(n);
        for (int i = 0; i < n; i++) {
            long long s = carry;
            if (i < (int)d.size()) s += d[i];
            if (i < (int)o.d.size()) s += o.d[i];
            res.d[i] = s % BASE;
            carry = s / BASE;
        }
        if (carry) res.d.push_back(carry);
        return res;
    }

    BigInt operator*(const BigInt& o) const {
        BigInt res;
        res.d.assign(d.size() + o.d.size(), 0);
        for (int i = 0; i < (int)d.size(); i++) {
            long long carry = 0;
            for (int j = 0; j < (int)o.d.size(); j++) {
                long long cur = (long long)d[i] * o.d[j] + res.d[i + j] + carry;
                res.d[i + j] = cur % BASE;
                carry = cur / BASE;
            }
            if (carry) res.d[i + o.d.size()] += carry;
        }
        while (res.d.size() > 1 && res.d.back() == 0) res.d.pop_back();
        return res;
    }

    // Division by 2
    BigInt div2() const {
        BigInt res;
        res.d.resize(d.size());
        long long carry = 0;
        for (int i = (int)d.size() - 1; i >= 0; i--) {
            long long cur = carry * BASE + d[i];
            res.d[i] = cur / 2;
            carry = cur % 2;
        }
        while (res.d.size() > 1 && res.d.back() == 0) res.d.pop_back();
        return res;
    }

    // Division: this / o (integer division)
    BigInt divBy(const BigInt& o) const {
        // Simple long division
        BigInt res;
        BigInt rem;
        res.d.resize(d.size(), 0);

        for (int i = (int)d.size() - 1; i >= 0; i--) {
            // rem = rem * BASE + d[i]
            rem.d.insert(rem.d.begin(), d[i]);
            while (rem.d.size() > 1 && rem.d.back() == 0) rem.d.pop_back();

            // Binary search for quotient digit
            int lo = 0, hi = BASE - 1, best = 0;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                BigInt prod = o * BigInt(mid);
                if (prod.cmp(rem) <= 0) {
                    best = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
            res.d[i] = best;
            BigInt sub = o * BigInt(best);
            // rem = rem - sub
            long long borrow = 0;
            for (int j = 0; j < (int)rem.d.size(); j++) {
                long long val = (long long)rem.d[j] - borrow;
                if (j < (int)sub.d.size()) val -= sub.d[j];
                if (val < 0) {
                    val += BASE;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                rem.d[j] = val;
            }
            while (rem.d.size() > 1 && rem.d.back() == 0) rem.d.pop_back();
        }
        while (res.d.size() > 1 && res.d.back() == 0) res.d.pop_back();
        return res;
    }

    string toString() const {
        if (d.empty()) return "0";
        string s = to_string(d.back());
        for (int i = (int)d.size() - 2; i >= 0; i--) {
            string t = to_string(d[i]);
            s += string(9 - t.size(), '0') + t;
        }
        return s;
    }

    int digitSum(int numDigits) const {
        string s = toString();
        int sum = 0;
        for (int i = 0; i < min(numDigits, (int)s.size()); i++) {
            sum += s[i] - '0';
        }
        return sum;
    }
};

// Integer square root using Newton's method
BigInt isqrt(const BigInt& n) {
    if (n.isZero()) return BigInt(0);

    // Initial guess: start high
    BigInt x = n;
    BigInt y = (x + n.divBy(x)).div2();

    while (y < x) {
        x = y;
        y = (x + n.divBy(x)).div2();
    }
    return x;
}

int main() {
    // Compute 10^198
    BigInt power(1);
    for (int i = 0; i < 198; i++) {
        power = power * BigInt(10);
    }

    set<int> perfect_squares = {1, 4, 9, 16, 25, 36, 49, 64, 81, 100};
    int total = 0;

    for (int n = 1; n <= 100; n++) {
        if (perfect_squares.count(n)) continue;
        BigInt N = BigInt(n) * power;
        BigInt root = isqrt(N);
        total += root.digitSum(100);
    }

    cout << total << endl;
    return 0;
}
