#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n = 600851475143LL;
    for (long long d = 2; d * d <= n; d++)
        while (n % d == 0)
            n /= d;
    cout << n << endl;
    return 0;
}
