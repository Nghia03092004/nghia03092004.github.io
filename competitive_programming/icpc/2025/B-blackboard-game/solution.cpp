#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    vector<int> n(t);
    int mx = 0;
    for (int i = 0; i < t; ++i) {
        cin >> n[i];
        mx = max(mx, n[i]);
    }

    const int LIM = 83;
    int small[LIM + 1] = {
        0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 0, 4, 4, 0, 0, 2, 0, 0, 0,
        4, 4, 4, 4, 0, 0, 2, 2, 2, 2, 2, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0,
        2, 2, 2, 0, 2, 2, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 0, 4, 0, 0, 2, 0, 0, 0
    };

    int up = mx / 3;
    vector<int> prev_prime(up + 1, 0);
    vector<char> is_prime(up + 1, 1);
    if (up >= 0) is_prime[0] = 0;
    if (up >= 1) is_prime[1] = 0;
    for (int i = 2; 1LL * i * i <= up; ++i) {
        if (!is_prime[i]) continue;
        for (int j = i * i; j <= up; j += i) {
            is_prime[j] = 0;
        }
    }
    int last = 0;
    for (int i = 2; i <= up; ++i) {
        if (is_prime[i]) {
            last = i;
        }
        prev_prime[i] = last;
    }

    for (int x : n) {
        if (x <= LIM) {
            if (small[x] == 0) {
                cout << "second\n";
            } else {
                cout << "first " << small[x] << '\n';
            }
        } else {
            cout << "first " << 2 * prev_prime[x / 3] << '\n';
        }
    }
    return 0;
}
