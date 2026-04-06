#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 10000;
    vector<bool> is_prime(LIMIT, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i < LIMIT; i++)
        if (is_prime[i])
            for (int j = i * i; j < LIMIT; j += i)
                is_prime[j] = false;

    map<string, vector<int>> groups;
    for (int p = 1000; p < LIMIT; p++) {
        if (!is_prime[p]) continue;
        string s = to_string(p);
        sort(s.begin(), s.end());
        groups[s].push_back(p);
    }

    for (auto& [key, primes] : groups) {
        if ((int)primes.size() < 3) continue;
        set<int> pset(primes.begin(), primes.end());
        for (int i = 0; i < (int)primes.size(); i++) {
            for (int j = i + 1; j < (int)primes.size(); j++) {
                int third = 2 * primes[j] - primes[i];
                if (third < LIMIT && pset.count(third) && primes[i] != 1487) {
                    cout << primes[i] << primes[j] << third << endl;
                    return 0;
                }
            }
        }
    }
    return 0;
}
