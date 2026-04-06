#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 115000; // Rosser's bound: p_10001 < 10001*(ln 10001 + ln ln 10001) < 114320
    vector<bool> sieve(N + 1, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= N; i++)
        if (sieve[i])
            for (int j = i * i; j <= N; j += i)
                sieve[j] = false;
    int count = 0;
    for (int i = 2; i <= N; i++) {
        if (sieve[i] && ++count == 10001) {
            cout << i << endl;
            return 0;
        }
    }
    return 0;
}
