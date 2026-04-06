#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

/*
 * Problem 303: Multiples with Small Digits
 *
 * For each n from 1 to 10000, find the smallest multiple of n
 * using only digits {0, 1, 2}. Sum up f(n)/n.
 *
 * BFS on remainders mod n, building numbers digit by digit.
 * We track f(n)/n by also tracking the quotient during reconstruction.
 */

// BFS to find f(n)/n
// Returns f(n)/n as a long long (fits since f(n)/n < 3^30 or so)
ull solve(int n) {
    if (n == 1) return 1;
    if (n == 2) return 1;

    // BFS: state = remainder mod n
    // parent[r] = {previous remainder, digit appended}
    vector<int> parent_rem(n, -1);
    vector<int> parent_dig(n, -1);
    vector<bool> visited(n, false);

    queue<int> q;
    // Leading digits 1 and 2
    for (int d = 1; d <= 2; d++) {
        int r = d % n;
        if (r == 0) return d; // f(n) = d, f(n)/n = d/n... but d < n usually
        if (!visited[r]) {
            visited[r] = true;
            parent_rem[r] = -2; // sentinel for root
            parent_dig[r] = d;
            q.push(r);
        }
    }

    while (!q.empty()) {
        int r = q.front();
        q.pop();
        for (int d = 0; d <= 2; d++) {
            int nr = (10LL * r + d) % n;
            if (nr == 0) {
                // Reconstruct the number and compute f(n)/n
                // Build digits in reverse
                vector<int> digits;
                digits.push_back(d);
                int cur = r;
                while (parent_rem[cur] != -2) {
                    digits.push_back(parent_dig[cur]);
                    cur = parent_rem[cur];
                }
                digits.push_back(parent_dig[cur]); // root digit
                reverse(digits.begin(), digits.end());

                // Compute f(n) as big number, then divide by n
                // f(n)/n: we can compute this by long division
                // Since f(n) might not fit in ull, use __int128
                // Actually for n <= 10000, f(n) can have ~30 digits, which is > 64 bits
                // Use __int128 which handles up to ~38 digits
                lll fn = 0;
                for (int dig : digits) {
                    fn = fn * 10 + dig;
                }
                return (ull)(fn / n);
            }
            if (!visited[nr]) {
                visited[nr] = true;
                parent_rem[nr] = r;
                parent_dig[nr] = d;
                q.push(nr);
            }
        }
    }
    return 0; // should not reach here
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ull total = 0;
    for (int n = 1; n <= 10000; n++) {
        total += solve(n);
    }
    cout << total << endl;
    return 0;
}
