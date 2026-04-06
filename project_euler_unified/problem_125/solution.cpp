#include <bits/stdc++.h>
using namespace std;

bool isPalindrome(long long n) {
    string s = to_string(n);
    int l = 0, r = (int)s.size() - 1;
    while (l < r) {
        if (s[l] != s[r]) return false;
        l++; r--;
    }
    return true;
}

int main() {
    const long long LIMIT = 100000000LL;
    set<long long> palindromes;

    for (long long a = 1; a * a + (a + 1) * (a + 1) < LIMIT; a++) {
        long long s = a * a;
        for (long long b = a + 1; ; b++) {
            s += b * b;
            if (s >= LIMIT) break;
            if (isPalindrome(s))
                palindromes.insert(s);
        }
    }

    long long ans = 0;
    for (long long x : palindromes)
        ans += x;
    cout << ans << endl;
    return 0;
}
