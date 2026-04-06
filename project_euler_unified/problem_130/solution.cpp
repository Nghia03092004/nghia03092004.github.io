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

int repunit_order(int n) {
    int r = 1;
    int k = 1;
    while (r != 0) {
        r = (10LL * r + 1) % n;
        k++;
    }
    return k;
}

int main() {
    int count = 0;
    long long total = 0;

    for (int n = 2; count < 25; n++) {
        if (n % 2 == 0 || n % 5 == 0) continue;
        if (is_prime(n)) continue;

        int an = repunit_order(n);
        if ((n - 1) % an == 0) {
            total += n;
            count++;
        }
    }

    cout << total << endl;
    return 0;
}
