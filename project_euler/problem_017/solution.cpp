#include <bits/stdc++.h>
using namespace std;

int letterCount(int n) {
    static const int ones[] = {0, 3, 3, 5, 4, 4, 3, 5, 5, 4};
    static const int teens[] = {3, 6, 6, 8, 8, 7, 7, 9, 8, 8};
    static const int tens[] = {0, 0, 6, 6, 5, 5, 5, 7, 6, 6};

    if (n == 1000) return 11;
    int c = 0;
    if (n >= 100) {
        c += ones[n / 100] + 7;
        n %= 100;
        if (n > 0) c += 3;
    }
    if (n >= 20) {
        c += tens[n / 10];
        n %= 10;
        c += ones[n];
    } else if (n >= 10) {
        c += teens[n - 10];
    } else {
        c += ones[n];
    }
    return c;
}

int main() {
    int total = 0;
    for (int i = 1; i <= 1000; i++)
        total += letterCount(i);
    cout << total << endl;
    return 0;
}
