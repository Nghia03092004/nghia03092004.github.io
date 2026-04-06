/*
 * Problem 897: Base-b Palindromes
 * Count palindromic numbers in base b up to N.
 * Key formula: C_b(k) = (b-1) * b^(floor((k-1)/2)) for k-digit palindromes.
 * Total palindromes up to N is O(sqrt(N)).
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<int> to_base(ll n, int b) {
    if (n == 0) return {0};
    vector<int> digits;
    while (n > 0) { digits.push_back(n % b); n /= b; }
    reverse(digits.begin(), digits.end());
    return digits;
}

bool is_palindrome(ll n, int b) {
    auto d = to_base(n, b);
    int sz = d.size();
    for (int i = 0; i < sz / 2; i++)
        if (d[i] != d[sz - 1 - i]) return false;
    return true;
}

ll count_k_digit(int k, int b) {
    // (b-1) * b^(floor((k-1)/2))
    ll result = b - 1;
    for (int i = 0; i < (k - 1) / 2; i++) result *= b;
    return result;
}

ll count_up_to_brute(ll N, int b) {
    ll count = 0;
    for (ll n = 1; n <= N; n++)
        if (is_palindrome(n, b)) count++;
    return count;
}

// Generate all base-b palindromes up to max_val
vector<ll> generate_palindromes(ll max_val, int b) {
    vector<ll> pals;
    for (int k = 1; k <= 40; k++) {
        int half = (k + 1) / 2;
        ll start = (half == 1) ? 1 : 1;
        // iterate over first-half values
        ll lo = 1;
        for (int i = 1; i < half; i++) lo *= b;
        ll hi = lo * b;
        for (ll h = lo; h < hi; h++) {
            auto hd = to_base(h, b);
            // build palindrome
            vector<int> pd = hd;
            if (k % 2 == 0) {
                for (int i = hd.size() - 1; i >= 0; i--) pd.push_back(hd[i]);
            } else {
                for (int i = (int)hd.size() - 2; i >= 0; i--) pd.push_back(hd[i]);
            }
            ll val = 0;
            for (int d : pd) val = val * b + d;
            if (val > max_val) return pals;
            pals.push_back(val);
        }
    }
    return pals;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Verify C_b(k) formula
    cout << "=== C_b(k) Verification ===" << endl;
    for (int b : {2, 3, 10}) {
        for (int k = 1; k <= 4; k++) {
            ll formula = count_k_digit(k, b);
            ll lo = 1;
            for (int i = 1; i < k; i++) lo *= b;
            ll hi = lo * b - 1;
            if (k == 1) lo = 1;
            ll direct = 0;
            for (ll n = lo; n <= hi && n <= 100000; n++)
                if (is_palindrome(n, b)) direct++;
            cout << "b=" << b << " k=" << k << ": formula=" << formula
                 << " direct=" << direct
                 << (formula == direct ? " OK" : " FAIL") << endl;
        }
    }

    // Count palindromes up to N
    cout << "\n=== Palindrome Counts ===" << endl;
    for (int b : {2, 10}) {
        for (ll N : {100LL, 1000LL, 10000LL}) {
            ll cnt = count_up_to_brute(N, b);
            cout << "P_" << b << "(" << N << ") = " << cnt << endl;
        }
    }

    // Generate and count
    auto pals = generate_palindromes(1000000, 10);
    cout << "\nP_10(10^6) = " << pals.size() << endl;

    cout << "\nAnswer: " << pals.size() << endl;
    return 0;
}
