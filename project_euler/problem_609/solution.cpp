#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    int N = 10000;
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= N; i++)
        if (is_prime[i])
            for (int j = i*i; j <= N; j += i)
                is_prime[j] = false;
    
    vector<int> pi(N + 1, 0);
    for (int i = 1; i <= N; i++)
        pi[i] = pi[i-1] + is_prime[i];
    
    map<int, int> counts;
    for (int n = 2; n <= N; n++) {
        int len = 0, x = n;
        while (x > 1) { x = pi[x]; len++; }
        counts[len]++;
    }
    
    ll MOD = 1000000007;
    ll product = 1;
    for (auto &[len, cnt] : counts)
        product = product * cnt % MOD;
    cout << product << endl;
    return 0;
}
