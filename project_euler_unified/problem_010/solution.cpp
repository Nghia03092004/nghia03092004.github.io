#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 2000000;
    vector<bool> sieve(N, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; (long long)i * i < N; i++)
        if (sieve[i])
            for (int j = i * i; j < N; j += i)
                sieve[j] = false;
    long long sum = 0;
    for (int i = 2; i < N; i++)
        if (sieve[i])
            sum += i;
    cout << sum << endl;
    return 0;
}
