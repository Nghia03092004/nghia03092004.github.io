#include <bits/stdc++.h>
using namespace std;

// Check if the string s[pos..end] can be split into parts summing to target
bool canSplit(const string& s, int pos, long long target, bool hasSplit) {
    if (pos == (int)s.size()) {
        return target == 0 && hasSplit;
    }
    if (target < 0) return false;

    long long val = 0;
    for (int i = pos; i < (int)s.size(); i++) {
        val = val * 10 + (s[i] - '0');
        if (val > target) break;
        // Don't use the entire string as one part (need at least 2 parts)
        if (i == (int)s.size() - 1 && !hasSplit) continue;
        if (canSplit(s, i + 1, target - val, true)) return true;
    }
    return false;
}

int main() {
    long long N = 1000000; // sqrt(10^12)
    long long total = 0;

    for (long long k = 2; k <= N; k++) {
        // Mod 9 pruning: k(k-1) must be divisible by 9
        int r = k % 9;
        if (r != 0 && r != 1) continue;

        long long n = k * k;
        string s = to_string(n);

        if (canSplit(s, 0, k, false)) {
            total += n;
        }
    }

    cout << total << endl;
    return 0;
}
