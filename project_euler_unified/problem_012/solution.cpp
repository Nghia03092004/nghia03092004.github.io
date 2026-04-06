#include <bits/stdc++.h>
using namespace std;

int countDivisors(long long n) {
    int count = 1;
    for (long long d = 2; d * d <= n; d++) {
        int e = 0;
        while (n % d == 0) { n /= d; e++; }
        count *= (e + 1);
    }
    if (n > 1) count *= 2;
    return count;
}

int main() {
    for (long long n = 1; ; n++) {
        long long a = (n % 2 == 0) ? n / 2 : n;
        long long b = (n % 2 == 0) ? n + 1 : (n + 1) / 2;
        if (countDivisors(a) * countDivisors(b) > 500) {
            cout << a * b << endl;
            return 0;
        }
    }
}
