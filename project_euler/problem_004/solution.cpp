#include <bits/stdc++.h>
using namespace std;

bool is_palindrome(int n) {
    string s = to_string(n);
    int lo = 0, hi = (int)s.size() - 1;
    while (lo < hi) {
        if (s[lo++] != s[hi--]) return false;
    }
    return true;
}

int main() {
    int best = 0;
    // By Theorem 1 + Euclid's lemma: one factor must be divisible by 11
    for (int x = 990; x >= 110; x -= 11) {
        for (int y = 999; y >= x; y--) {
            int P = x * y;
            if (P <= best) break;
            if (is_palindrome(P)) {
                best = P;
                break;
            }
        }
    }
    cout << best << endl;
    return 0;
}
