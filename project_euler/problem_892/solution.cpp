/*
 * Problem 892: Recursive Digit Factorial
 * f(n) = sum of d! for digits of n. Factorions: f(n) = n.
 * All factorions in base 10: {1, 2, 145, 40585}.
 */
#include <bits/stdc++.h>
using namespace std;

const int FACT[] = {1,1,2,6,24,120,720,5040,40320,362880};

int digit_factorial(int n) {
    if (n == 0) return 1;
    int total = 0;
    while (n > 0) { total += FACT[n % 10]; n /= 10; }
    return total;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== Factorions ===" << endl;
    vector<int> factorions;
    for (int n = 1; n <= 2540160; n++) {
        if (digit_factorial(n) == n) {
            factorions.push_back(n);
            cout << n << " ";
        }
    }
    cout << endl;

    cout << "\n=== Verification ===" << endl;
    for (int n : {1, 2, 145, 40585}) {
        cout << "f(" << n << ") = " << digit_factorial(n) << endl;
    }

    // Cycles
    cout << "\n=== Cycles ===" << endl;
    for (int start : {169, 871, 872}) {
        cout << start;
        int n = start;
        for (int i = 0; i < 5; i++) {
            n = digit_factorial(n);
            cout << " -> " << n;
        }
        cout << endl;
    }

    cout << "\nAnswer: {1, 2, 145, 40585}" << endl;
    return 0;
}
