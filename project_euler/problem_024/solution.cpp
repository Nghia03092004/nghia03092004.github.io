#include <bits/stdc++.h>
using namespace std;

int main() {
    const int n = 10;
    int k = 1000000 - 1;

    vector<int> fact(n, 1);
    for (int i = 1; i < n; i++)
        fact[i] = fact[i - 1] * i;

    vector<int> digits = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    string result;
    for (int i = 0; i < n; i++) {
        int f = fact[n - 1 - i];
        int idx = k / f;
        k %= f;
        result += to_string(digits[idx]);
        digits.erase(digits.begin() + idx);
    }

    cout << result << endl;
    return 0;
}
