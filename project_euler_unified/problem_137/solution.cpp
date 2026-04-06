#include <bits/stdc++.h>
using namespace std;

int main() {
    // The k-th golden nugget is F(2k) * F(2k+1)
    // We need the 15th, so we need F(30) * F(31)

    // Compute Fibonacci numbers up to F(31)
    long long fib[32];
    fib[1] = 1;
    fib[2] = 1;
    for (int i = 3; i <= 31; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }

    // 15th golden nugget = F(30) * F(31)
    long long answer = fib[30] * fib[31];
    cout << answer << endl;

    return 0;
}
