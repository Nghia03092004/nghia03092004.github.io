#include <bits/stdc++.h>
using namespace std;

bool isBinaryPalindrome(int n) {
    int rev = 0, orig = n;
    while (n > 0) {
        rev = (rev << 1) | (n & 1);
        n >>= 1;
    }
    return rev == orig;
}

int main() {
    int total = 0;

    // 1-digit palindromes
    for (int a = 1; a <= 9; a++)
        if (isBinaryPalindrome(a)) total += a;

    // 2-digit: aa
    for (int a = 1; a <= 9; a++) {
        int n = a * 11;
        if (isBinaryPalindrome(n)) total += n;
    }

    // 3-digit: aba
    for (int a = 1; a <= 9; a++)
        for (int b = 0; b <= 9; b++) {
            int n = a * 101 + b * 10;
            if (isBinaryPalindrome(n)) total += n;
        }

    // 4-digit: abba
    for (int a = 1; a <= 9; a++)
        for (int b = 0; b <= 9; b++) {
            int n = a * 1001 + b * 110;
            if (isBinaryPalindrome(n)) total += n;
        }

    // 5-digit: abcba
    for (int a = 1; a <= 9; a++)
        for (int b = 0; b <= 9; b++)
            for (int c = 0; c <= 9; c++) {
                int n = a * 10001 + b * 1010 + c * 100;
                if (isBinaryPalindrome(n)) total += n;
            }

    // 6-digit: abccba
    for (int a = 1; a <= 9; a++)
        for (int b = 0; b <= 9; b++)
            for (int c = 0; c <= 9; c++) {
                int n = a * 100001 + b * 10010 + c * 1100;
                if (isBinaryPalindrome(n)) total += n;
            }

    cout << total << endl;
    return 0;
}
