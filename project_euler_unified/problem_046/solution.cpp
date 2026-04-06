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

    for (int n = 9; n < LIMIT; n += 2) {
        if (is_prime[n]) continue;
        bool found = false;
        for (int k = 1; 2 * k * k < n; k++) {
            if (is_prime[n - 2 * k * k]) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << n << endl;
            return 0;
        }
    }
    return 0;
}
