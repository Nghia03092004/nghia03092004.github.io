#include <bits/stdc++.h>
using namespace std;

int main() {
    // Problem 230: Fibonacci Words
    // F(1) = A (100 digits of pi), F(2) = B (next 100 digits of pi)
    // F(n) = F(n-2) . F(n-1) (concatenation, older first)
    // Find sum of D((127+19n)*7^n) * 10^n for n=0..17

    string A = "1415926535897932384626433832795028841971"
               "6939937510582097494459230781640628620899"
               "86280348253421170679";
    string B = "8214808651328230664709384460955058223172"
               "5359408128481117450284102701938521105559"
               "64462294895493038196";

    // Use __int128 for large positions
    typedef __int128 lll;
    const int MAXN = 200;
    lll L[MAXN + 1];
    L[1] = L[2] = 100;
    lll cap = (lll)1e25;
    for (int i = 3; i <= MAXN; i++) {
        L[i] = L[i-1] + L[i-2];
        if (L[i] > cap) {
            for (int j = i + 1; j <= MAXN; j++) L[j] = cap;
            break;
        }
    }

    // Compute 7^n using __int128
    auto power7 = [](int n) -> lll {
        lll result = 1;
        for (int i = 0; i < n; i++) result *= 7;
        return result;
    };

    // Find the k-th digit (1-indexed) of the Fibonacci word
    // F(n) = F(n-2) . F(n-1): first L[n-2] chars from F(n-2), rest from F(n-1)
    auto find_digit = [&](lll k) -> int {
        int n = 1;
        while (L[n] < k) n++;
        while (n > 2) {
            if (k <= L[n-2]) {
                n -= 2;
            } else {
                k -= L[n-2];
                n -= 1;
            }
        }
        if (n == 1) return A[(int)(k - 1)] - '0';
        else return B[(int)(k - 1)] - '0';
    };

    // Compute the answer
    long long answer = 0;
    long long pow10 = 1;
    for (int n = 0; n <= 17; n++) {
        lll k = (lll)(127 + 19 * n) * power7(n);
        int digit = find_digit(k);
        answer += pow10 * digit;
        pow10 *= 10;
    }

    printf("%lld\n", answer);
    return 0;
}
