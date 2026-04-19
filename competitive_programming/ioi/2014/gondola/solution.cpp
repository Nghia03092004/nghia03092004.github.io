#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 9;

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

// Subtask 1: Check validity
int valid(int n, int inputSeq[]) {
    set<int> seen;
    int offset = -1;
    for (int i = 0; i < n; i++) {
        if (seen.count(inputSeq[i])) return 0; // duplicate
        seen.insert(inputSeq[i]);
        if (inputSeq[i] <= n) {
            int off = ((inputSeq[i] - 1) - i % n + n) % n;
            if (offset == -1) offset = off;
            else if (offset != off) return 0;
        }
    }
    return 1;
}

// Subtask 2: Find replacement sequence
int replacement(int n, int inputSeq[], int replacementSeq[]) {
    // Find rotation offset
    int offset = 0;
    for (int i = 0; i < n; i++) {
        if (inputSeq[i] <= n) {
            offset = ((inputSeq[i] - 1) - i + n) % n;
            break;
        }
    }

    // original[i] = what gondola was originally at position i
    // For each position, if inputSeq[i] > n, we need to record the replacement chain
    vector<pair<int,int>> replacements; // (final value, position)
    for (int i = 0; i < n; i++) {
        if (inputSeq[i] > n) {
            replacements.push_back({inputSeq[i], i});
        }
    }
    sort(replacements.begin(), replacements.end());

    int len = 0;
    int nextId = n + 1;
    for (auto &[val, pos] : replacements) {
        int orig = (pos + offset) % n + 1;
        // The first replacement at this position replaces the original
        replacementSeq[len++] = orig;
        // Subsequent replacements up to val replace the intermediate gondola
        for (int id = nextId; id < val; id++) {
            // This intermediate gondola was at this position, then replaced
            // Actually, intermediates could be at this position
            replacementSeq[len++] = id;
        }
        nextId = val + 1;
    }
    return len;
}

// Subtask 3: Count possible sequences
int countReplacement(int n, int inputSeq[]) {
    // First check validity
    if (!valid(n, inputSeq)) return 0;

    // Find which positions have original values
    int offset = -1;
    for (int i = 0; i < n; i++) {
        if (inputSeq[i] <= n) {
            offset = ((inputSeq[i] - 1) - i + n) % n;
            break;
        }
    }

    // Collect positions that have replacement values (> n)
    vector<pair<int,int>> replacements; // (value, position_index)
    for (int i = 0; i < n; i++) {
        if (inputSeq[i] > n) {
            replacements.push_back({inputSeq[i], i});
        }
    }
    sort(replacements.begin(), replacements.end());

    int cntUnknown = (int)replacements.size();
    // If no original value is present, any rotation is possible -> multiply by n
    long long ans = 1;
    if (offset == -1 && cntUnknown > 0) {
        ans = n; // n possible rotations
    }

    long long prev = n; // last assigned replacement number
    for (int i = 0; i < (int)replacements.size(); i++) {
        long long cur = replacements[i].first;
        long long gap = cur - prev - 1; // number of intermediate replacements
        // Each intermediate replacement can go to any of cntUnknown positions
        ans = ans % MOD * power(cntUnknown, gap, MOD) % MOD;
        cntUnknown--;
        prev = cur;
    }
    return (int)(ans % MOD);
}

// Standalone main for testing
int main() {
    int n;
    scanf("%d", &n);
    int seq[n];
    for (int i = 0; i < n; i++) scanf("%d", &seq[i]);

    printf("Valid: %d\n", valid(n, seq));

    if (valid(n, seq)) {
        int rep[300000];
        int len = replacement(n, seq, rep);
        printf("Replacement sequence (%d): ", len);
        for (int i = 0; i < len; i++) printf("%d ", rep[i]);
        printf("\n");
        printf("Count: %d\n", countReplacement(n, seq));
    }
    return 0;
}
