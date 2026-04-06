#include <bits/stdc++.h>
using namespace std;

typedef vector<int> BigInt;

BigInt from_int(int x) {
    BigInt r;
    while (x > 0) { r.push_back(x % 10); x /= 10; }
    if (r.empty()) r.push_back(0);
    return r;
}

BigInt add(const BigInt& a, const BigInt& b) {
    BigInt r;
    int carry = 0, n = max(a.size(), b.size());
    for (int i = 0; i < n || carry; i++) {
        int s = carry;
        if (i < (int)a.size()) s += a[i];
        if (i < (int)b.size()) s += b[i];
        r.push_back(s % 10);
        carry = s / 10;
    }
    return r;
}

int main() {
    BigInt a = from_int(1), b = from_int(1);
    int idx = 2;
    while ((int)b.size() < 1000) {
        BigInt c = add(a, b);
        a = b;
        b = c;
        idx++;
    }
    cout << idx << endl;
    return 0;
}
