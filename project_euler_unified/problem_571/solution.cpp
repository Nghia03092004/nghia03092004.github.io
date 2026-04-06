#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

bool isPandigital(ll n, int b) {
    int mask = 0, target = (1 << b) - 1;
    while (n > 0) {
        mask |= 1 << (n % b);
        n /= b;
        if (mask == target) return true;
    }
    return mask == target;
}

int main() {
    const int NEED = 10;
    ll pow12[12];
    pow12[0] = 1;
    for (int i = 1; i < 12; i++) pow12[i] = pow12[i - 1] * 12;

    vector<ll> results;

    for (int first = 1; first <= 11; first++) {
        vector<int> rest;
        for (int i = 0; i <= 11; i++)
            if (i != first) rest.push_back(i);
        sort(rest.begin(), rest.end());

        do {
            ll n = (ll)first * pow12[11];
            for (int i = 0; i < 11; i++)
                n += (ll)rest[i] * pow12[10 - i];

            bool ok = true;
            for (int b = 11; b >= 2; b--) {
                if (!isPandigital(n, b)) { ok = false; break; }
            }
            if (ok) {
                results.push_back(n);
                if (first == 1 && (int)results.size() >= NEED) goto done;
            }
        } while (next_permutation(rest.begin(), rest.end()));
    }

done:
    sort(results.begin(), results.end());
    ll total = 0;
    for (int i = 0; i < min((int)results.size(), NEED); i++)
        total += results[i];

    cout << total << endl;
    return 0;
}
