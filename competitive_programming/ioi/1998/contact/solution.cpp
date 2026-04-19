// IOI 1998 - Contact
// Count bit patterns of length a..b in binary string, output top k frequency groups
// Time: O(n * b), Space: O(b * 2^b + n)
#include <bits/stdc++.h>
using namespace std;

char buf[200005];
int cnt[13][4096]; // cnt[len][val]

int main() {
    int a, b, k;
    scanf("%d %d %d", &a, &b, &k);

    // Read binary string (may span multiple lines)
    int n = 0;
    char line[100];
    while (scanf("%s", line) == 1) {
        for (int i = 0; line[i]; i++)
            buf[n++] = line[i] - '0';
    }

    memset(cnt, 0, sizeof(cnt));

    // Count all patterns of length a..b
    for (int i = 0; i < n; i++) {
        int val = 0;
        for (int len = 1; len <= b && i + len <= n; len++) {
            val = val * 2 + buf[i + len - 1];
            if (len >= a)
                cnt[len][val]++;
        }
    }

    // Collect all patterns with count > 0
    struct Pattern {
        int freq, len, val;
    };
    vector<Pattern> patterns;
    for (int len = a; len <= b; len++)
        for (int v = 0; v < (1 << len); v++)
            if (cnt[len][v] > 0)
                patterns.push_back({cnt[len][v], len, v});

    // Sort: freq desc, then len asc, then val asc
    sort(patterns.begin(), patterns.end(), [](const Pattern& x, const Pattern& y) {
        if (x.freq != y.freq) return x.freq > y.freq;
        if (x.len != y.len) return x.len < y.len;
        return x.val < y.val;
    });

    // Output top k frequency groups
    int groups = 0;
    int i = 0;
    while (i < (int)patterns.size() && groups < k) {
        int curFreq = patterns[i].freq;
        printf("%d\n", curFreq);

        int lineCount = 0;
        while (i < (int)patterns.size() && patterns[i].freq == curFreq) {
            if (lineCount > 0) printf(" ");
            // Print binary representation with leading zeros
            int len = patterns[i].len;
            int val = patterns[i].val;
            char bits[13];
            for (int j = len - 1; j >= 0; j--) {
                bits[j] = '0' + (val & 1);
                val >>= 1;
            }
            bits[len] = '\0';
            printf("%s", bits);
            lineCount++;
            i++;
        }
        printf("\n");
        groups++;
    }

    return 0;
}
