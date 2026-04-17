#include <bits/stdc++.h>
using namespace std;

int sum_proper_divisors(int n) {
    if (n <= 1) return 0;
    int s = 1;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            s += i;
            if (i != n / i) s += n / i;
        }
    }
    return s;
}

int main() {
    int total = 0;
    for (int a = 2; a < 10000; a++) {
        int b = sum_proper_divisors(a);
        if (b != a && b > 0 && sum_proper_divisors(b) == a)
            total += a;
    }
    cout << total << endl;
    return 0;
}
