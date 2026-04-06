#include <bits/stdc++.h>
using namespace std;

bool is_prime(int n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; (long long)i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int main() {
    // By Theorem 2, only n=7 and n=4 can yield pandigital primes.
    // Search n=7 first in descending lexicographic order.
    string digits = "7654321";
    do {
        int num = stoi(digits);
        if (is_prime(num)) {
            cout << num << endl;
            return 0;
        }
    } while (prev_permutation(digits.begin(), digits.end()));

    // Fallback: search n=4
    digits = "4321";
    do {
        int num = stoi(digits);
        if (is_prime(num)) {
            cout << num << endl;
            return 0;
        }
    } while (prev_permutation(digits.begin(), digits.end()));

    return 0;
}
