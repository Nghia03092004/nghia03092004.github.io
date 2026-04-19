// IOI 1989 - Problem 2: Strings
// Count binary strings of length n with no two consecutive 1s.
// Recurrence: f(n) = f(n-1) + f(n-2), f(1)=2, f(2)=3 (Fibonacci variant)
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    scanf("%d", &n);

    if (n == 0) { printf("1\n"); return 0; }
    if (n == 1) { printf("2\n"); return 0; }

    long long prev2 = 2; // f(1)
    long long prev1 = 3; // f(2)
    for (int i = 3; i <= n; i++) {
        long long cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    printf("%lld\n", prev1);
    return 0;
}
