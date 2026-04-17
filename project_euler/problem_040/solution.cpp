#include <bits/stdc++.h>
using namespace std;

int champernowne_digit(int n) {
    int k = 1;
    long long count = 9;
    long long start = 1;

    while (n > k * count) {
        n -= k * count;
        k++;
        count *= 10;
        start *= 10;
    }

    // n is now the position within the k-digit block (1-indexed)
    long long number = start + (n - 1) / k;
    int digit_pos = (n - 1) % k;

    string s = to_string(number);
    return s[digit_pos] - '0';
}

int main() {
    long long product = 1;
    for (int exp = 0; exp <= 6; exp++) {
        int pos = 1;
        for (int i = 0; i < exp; i++) pos *= 10;
        int d = champernowne_digit(pos);
        product *= d;
    }

    cout << product << endl;
    return 0;
}
