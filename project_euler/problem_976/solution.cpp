#include <bits/stdc++.h>
using namespace std;
int main() {
    // For 10^18 we need careful computation
    // Count distinct a^b with a>=1, b>=2, a^b <= 10^18
    // Use set of (base reduced to primitive form)
    long long N = 1000000000000000000LL;
    set<long long> seen;
    seen.insert(1);
    for (int b = 2; b <= 60; b++) {
        long long a = 2;
        while (true) {
            // Compute a^b, watch for overflow
            long long val = 1;
            bool overflow = false;
            for (int i = 0; i < b; i++) {
                if (val > N / a) { overflow = true; break; }
                val *= a;
            }
            if (overflow || val > N) break;
            seen.insert(val);
            a++;
        }
    }
    cout << (long long)seen.size() << endl;
    return 0;
}
