#include <bits/stdc++.h>
using namespace std;

int main() {
    const int s = 1000;
    for (int a = 1; a < s / 3; a++) {
        int num = s * s / 2 - s * a;
        int den = s - a;
        if (num % den == 0) {
            int b = num / den;
            int c = s - a - b;
            if (a < b && b < c) {
                cout << (long long)a * b * c << endl;
                return 0;
            }
        }
    }
    return 0;
}
