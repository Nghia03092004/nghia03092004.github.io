#include <bits/stdc++.h>
using namespace std;

int main() {
    // Iterative LCM via GCD-LCM identity: lcm(a,b) = a / gcd(a,b) * b
    long long L = 1;
    for (int k = 2; k <= 20; k++) {
        L = L / __gcd(L, (long long)k) * k;
    }
    cout << L << endl;
    return 0;
}
