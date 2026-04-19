#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, K;
    // K is the balance bound (e.g., K=2 in the IOI problem)
    // M is the modulus
    cin >> N >> M >> K;

    string S;
    cin >> S;

    // Balance: L = +1, R = -1
    // Constraint: balance at every position is in [-K, K]

    // Precompute f[len][balance] = number of valid sequences of length len
    // starting from balance b, all intermediate balances in [-K, K]
    // f[0][b] = 1 for all valid b
    // f[len][b] = f[len-1][b+1] + f[len-1][b-1] (if b+1 and b-1 in range)

    int offset = K; // shift so indices are 0..2K
    int states = 2 * K + 1;

    // f[b] for current length (space optimized)
    vector<long long> f(states, 1); // f[0][b] = 1

    // We need f for lengths 0, 1, ..., N
    // Store f[len][b] for all lengths (we'll need f at various positions)
    vector<vector<long long>> F(N + 1, vector<long long>(states, 0));
    for (int b = 0; b < states; b++) F[0][b] = 1;

    for (int len = 1; len <= N; len++) {
        for (int b = 0; b < states; b++) {
            F[len][b] = 0;
            // Choose L (+1): new balance = b + 1 (relative), index b+1
            if (b + 1 < states) F[len][b] = (F[len][b] + F[len-1][b+1]) % M;
            // Choose R (-1): new balance = b - 1, index b-1
            if (b - 1 >= 0) F[len][b] = (F[len][b] + F[len-1][b-1]) % M;
        }
    }

    // Count valid sequences <= S
    long long ans = 0;
    int balance = 0; // current balance (0 = centered)
    bool valid = true;

    for (int i = 0; i < N; i++) {
        if (!valid) break;

        if (S[i] == 'R') {
            // The smaller choice is 'L' (balance += 1)
            int newBal = balance + 1;
            if (abs(newBal) <= K) {
                // Count valid completions of length N - i - 1 from balance newBal
                int remaining = N - i - 1;
                ans = (ans + F[remaining][newBal + offset]) % M;
            }
            // Continue with S[i] = 'R': balance -= 1
            balance -= 1;
        } else {
            // S[i] = 'L', no smaller option at this position
            balance += 1;
        }

        if (abs(balance) > K) {
            valid = false;
        }
    }

    if (valid) {
        ans = (ans + 1) % M; // count S itself
    }

    cout << ans << "\n";
    return 0;
}
