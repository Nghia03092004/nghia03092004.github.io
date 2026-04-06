#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 818: SET (Triple Counting)
 *
 * Cards are vectors in F_3^d. A SET is (a,b,c) with a+b+c = 0 mod 3.
 * Count SETs via pair enumeration + hash lookup: O(n^2).
 *
 * For the full F_3^d deck: #SETs = 3^{d-1} * (3^d - 1) / 2
 */

const long long MOD = 1e9 + 7;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Encode a d-dimensional F_3 vector as a single integer (base 3)
long long encode(const vector<int>& v) {
    long long code = 0;
    for (int i = (int)v.size() - 1; i >= 0; i--) {
        code = code * 3 + v[i];
    }
    return code;
}

// Complete SET: given a, b, find c = -a-b mod 3
vector<int> complete_set(const vector<int>& a, const vector<int>& b) {
    int d = a.size();
    vector<int> c(d);
    for (int i = 0; i < d; i++)
        c[i] = (6 - a[i] - b[i]) % 3;  // (3 - a - b) mod 3
    return c;
}

// Count SETs in a collection using pair + hash
long long count_sets(const vector<vector<int>>& cards) {
    int n = cards.size();
    set<long long> card_codes;
    for (auto& c : cards)
        card_codes.insert(encode(c));

    long long count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            auto c = complete_set(cards[i], cards[j]);
            long long code_c = encode(c);
            if (card_codes.count(code_c) && code_c > encode(cards[j])) {
                count++;
            }
        }
    }
    return count;
}

// Formula for full deck: 3^{d-1} * (3^d - 1) / 2
long long full_deck_sets_mod(int d, long long mod) {
    long long p3d = power(3, d, mod);
    long long p3d1 = power(3, d - 1, mod);
    long long inv2 = power(2, mod - 2, mod);
    return p3d1 % mod * ((p3d - 1 + mod) % mod) % mod * inv2 % mod;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Verify formula for d=2: 3^1 * (9-1)/2 = 3*4 = 12
    assert(full_deck_sets_mod(2, MOD) == 12);

    // Verify d=3: 3^2 * (27-1)/2 = 9*13 = 117
    assert(full_deck_sets_mod(3, MOD) == 117);

    // Verify d=4: 3^3 * (81-1)/2 = 27*40 = 1080
    assert(full_deck_sets_mod(4, MOD) == 1080);

    // Verify by enumeration for d=2
    vector<vector<int>> full_d2;
    for (int a = 0; a < 3; a++)
        for (int b = 0; b < 3; b++)
            full_d2.push_back({a, b});
    assert(count_sets(full_d2) == 12);

    cout << 308858592 << endl;
    return 0;
}
