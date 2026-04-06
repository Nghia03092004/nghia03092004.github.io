#include <bits/stdc++.h>
using namespace std;

// Multiply a big integer (stored as digits, least significant first) by a small integer x.
vector<int> multiply(const vector<int>& num, int x) {
    vector<int> result;
    int carry = 0;
    for (int d : num) {
        int prod = d * x + carry;
        result.push_back(prod % 10);
        carry = prod / 10;
    }
    while (carry > 0) {
        result.push_back(carry % 10);
        carry /= 10;
    }
    return result;
}

int digitSum(const vector<int>& num) {
    int s = 0;
    for (int d : num) s += d;
    return s;
}

int main() {
    int maxSum = 0;

    for (int a = 2; a < 100; a++) {
        vector<int> power = {1};  // a^0 = 1
        for (int b = 1; b < 100; b++) {
            power = multiply(power, a);
            int s = digitSum(power);
            if (s > maxSum) {
                maxSum = s;
            }
        }
    }

    cout << maxSum << endl;
    return 0;
}
