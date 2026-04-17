#include <bits/stdc++.h>
using namespace std;

// Simple big integer using vector of digits (least significant first)
struct BigInt {
    vector<int> d; // digits, d[0] is least significant

    BigInt(int x = 0) {
        if (x == 0) d.push_back(0);
        while (x > 0) { d.push_back(x % 10); x /= 10; }
    }

    BigInt operator+(const BigInt& o) const {
        BigInt res;
        res.d.clear();
        int carry = 0;
        for (int i = 0; i < (int)max(d.size(), o.d.size()) || carry; i++) {
            int s = carry;
            if (i < (int)d.size()) s += d[i];
            if (i < (int)o.d.size()) s += o.d[i];
            res.d.push_back(s % 10);
            carry = s / 10;
        }
        return res;
    }

    BigInt operator*(int x) const {
        BigInt res;
        res.d.clear();
        long long carry = 0;
        for (int i = 0; i < (int)d.size() || carry; i++) {
            long long s = carry;
            if (i < (int)d.size()) s += (long long)d[i] * x;
            res.d.push_back(s % 10);
            carry = s / 10;
        }
        while (res.d.size() > 1 && res.d.back() == 0) res.d.pop_back();
        return res;
    }

    int digit_sum() const {
        int s = 0;
        for (int x : d) s += x;
        return s;
    }
};

int main() {
    // Continued fraction of e: [2; 1, 2, 1, 1, 4, 1, 1, 6, ...]
    // a[0] = 2, a[k] for k>=1: if k%3==2 then 2*(k+1)/3 else 1
    auto a = [](int k) -> int {
        if (k == 0) return 2;
        if (k % 3 == 2) return 2 * (k + 1) / 3;
        return 1;
    };

    // Compute 100th convergent (index 99)
    BigInt h_prev2(1); // h_{-1} = 1
    BigInt h_prev1(a(0)); // h_0 = a_0 = 2

    for (int i = 1; i <= 99; i++) {
        BigInt h_new = h_prev1 * a(i) + h_prev2;
        h_prev2 = h_prev1;
        h_prev1 = h_new;
    }

    cout << h_prev1.digit_sum() << endl;
    return 0;
}
