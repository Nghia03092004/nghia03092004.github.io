#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<int> digits = {1};

    for (int i = 0; i < 1000; i++) {
        int carry = 0;
        for (int j = 0; j < (int)digits.size(); j++) {
            int val = digits[j] * 2 + carry;
            digits[j] = val % 10;
            carry = val / 10;
        }
        while (carry > 0) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }

    int sum = 0;
    for (int d : digits) sum += d;
    cout << sum << endl;
    return 0;
}
