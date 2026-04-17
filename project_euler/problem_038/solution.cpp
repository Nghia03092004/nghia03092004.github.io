#include <bits/stdc++.h>
using namespace std;

bool isPandigital(const string& s) {
    if (s.size() != 9) return false;
    int mask = 0;
    for (char c : s) {
        if (c == '0') return false;
        int d = c - '0';
        if (mask & (1 << d)) return false;
        mask |= (1 << d);
    }
    return mask == 0x3FE;
}

int main() {
    long long best = 0;
    for (int x = 1; x <= 9999; x++) {
        string concat;
        for (int n = 1; n <= 9; n++) {
            concat += to_string((long long)x * n);
            if (concat.size() > 9) break;
            if (n >= 2 && concat.size() == 9 && isPandigital(concat))
                best = max(best, stoll(concat));
        }
    }
    cout << best << endl;
    return 0;
}
