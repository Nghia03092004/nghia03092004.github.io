#include <bits/stdc++.h>
using namespace std;
typedef __int128 lll;

long long binom(int n, int k) {
    lll result = 1;
    for (int i = 0; i < k; i++)
        result = result * (n - i) / (i + 1);
    return (long long)result;
}

int main() {
    int n = 100;
    long long inc = binom(n + 9, 9) - 1;
    long long dec = binom(n + 10, 10) - 1 - n;
    long long overlap = 9LL * n;
    cout << inc + dec - overlap << endl;
    return 0;
}
