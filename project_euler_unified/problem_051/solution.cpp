#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 1000000;
    vector<bool> is_prime(LIMIT, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i < LIMIT; i++)
        if (is_prime[i])
            for (int j = i * i; j < LIMIT; j += i)
                is_prime[j] = false;

    for (int n = 100000; n < LIMIT; n++) {
        if (!is_prime[n]) continue;
        string s = to_string(n);
        int len = s.size();

        // Try masks where all masked positions share the same digit (0, 1, or 2)
        for (int mask = 1; mask < (1 << len); mask++) {
            int bits = __builtin_popcount(mask);
            if (bits % 3 != 0) continue;

            // All masked positions must share the same digit
            char common = 0;
            bool valid = true;
            for (int i = 0; i < len; i++) {
                if (mask & (1 << i)) {
                    if (common == 0) common = s[i];
                    else if (s[i] != common) { valid = false; break; }
                }
            }
            if (!valid) continue;
            // Only check digits 0, 1, 2 for smallest-member constraint
            if (common > '2') continue;

            int count = 0;
            int smallest = -1;
            for (char d = '0'; d <= '9'; d++) {
                string t = s;
                for (int i = 0; i < len; i++)
                    if (mask & (1 << i))
                        t[i] = d;
                if (t[0] == '0') continue;
                int num = stoi(t);
                if (num >= 100000 && is_prime[num]) {
                    count++;
                    if (smallest == -1) smallest = num;
                }
            }
            if (count == 8) {
                cout << smallest << endl;
                return 0;
            }
        }
    }
    return 0;
}
