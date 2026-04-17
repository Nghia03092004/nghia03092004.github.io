#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<int> digits = {1};

    for (int n = 2; n <= 100; n++) {
        int carry = 0;
        for (int j = 0; j < (int)digits.size(); j++) {
            int val = digits[j] * n + carry;
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
