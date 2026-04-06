#include <bits/stdc++.h>
using namespace std;

// Big integer as vector of digits (least significant first)
typedef vector<int> BigInt;

BigInt fromInt(int x) {
    BigInt result;
    if (x == 0) { result.push_back(0); return result; }
    while (x > 0) {
        result.push_back(x % 10);
        x /= 10;
    }
    return result;
}

BigInt add(const BigInt& a, const BigInt& b) {
    BigInt result;
    int carry = 0;
    int n = max(a.size(), b.size());
    for (int i = 0; i < n || carry; i++) {
        int sum = carry;
        if (i < (int)a.size()) sum += a[i];
        if (i < (int)b.size()) sum += b[i];
        result.push_back(sum % 10);
        carry = sum / 10;
    }
    return result;
}

int numDigits(const BigInt& a) {
    return (int)a.size();
}

int main() {
    // Recurrence: p_n = p_{n-1} + 2*q_{n-1}, q_n = p_{n-1} + q_{n-1}
    BigInt p = fromInt(3);
    BigInt q = fromInt(2);

    int count = 0;
    for (int i = 1; i <= 1000; i++) {
        if (numDigits(p) > numDigits(q)) {
            count++;
        }
        BigInt twoq = add(q, q);
        BigInt p_new = add(p, twoq);
        BigInt q_new = add(p, q);
        p = p_new;
        q = q_new;
    }

    cout << count << endl;
    return 0;
}
