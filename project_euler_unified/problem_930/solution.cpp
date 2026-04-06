#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 100000;
    vector<bool> sieve(N + 1, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= N; i++)
        if (sieve[i])
            for (int j = i*i; j <= N; j += i)
                sieve[j] = false;
    int prime_count = 0;
    for (int p = 3; p <= N; p++)
        if (sieve[p]) prime_count++;
    // R(2)=0, R(p)=-1 for odd primes
    cout << -prime_count << endl;
    return 0;
}
