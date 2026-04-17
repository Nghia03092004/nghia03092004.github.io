/*
 * Problem 893: Steiner Systems
 * S(t,k,v): every t-subset in exactly one k-block.
 * b = C(v,t)/C(k,t), r = C(v-1,t-1)/C(k-1,t-1).
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll comb_val(int n, int r) {
    if (r < 0 || r > n) return 0;
    if (r > n - r) r = n - r;
    ll result = 1;
    for (int i = 0; i < r; i++) result = result * (n - i) / (i + 1);
    return result;
}

bool check_necessary(int t, int k, int v) {
    for (int i = 0; i <= t; i++) {
        ll num = comb_val(v - i, t - i);
        ll den = comb_val(k - i, t - i);
        if (den == 0 || num % den != 0) return false;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== Steiner System Parameters ===" << endl;
    vector<tuple<int,int,int>> systems = {{2,3,7},{2,3,9},{3,4,8},{4,5,11},{5,6,12},{5,8,24}};
    for (auto [t,k,v] : systems) {
        ll b = comb_val(v,t) / comb_val(k,t);
        ll r = comb_val(v-1,t-1) / comb_val(k-1,t-1);
        cout << "S(" << t << "," << k << "," << v << "): b=" << b << ", r=" << r << endl;
    }

    cout << "\n=== Steiner Triple Systems ===" << endl;
    for (int v = 3; v < 50; v++)
        if (check_necessary(2, 3, v))
            cout << "v=" << v << ": b=" << comb_val(v,2)/3 << endl;

    cout << "\nAnswer: S(5,8,24) has b=" << comb_val(24,5)/comb_val(8,5) << " blocks" << endl;
    return 0;
}
