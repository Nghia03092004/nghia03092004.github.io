// IOI 2001 - Score
// Given a decimal number string and K allowed adjacent swaps,
// find the maximum and minimum achievable numbers.
// Greedy: for each position, find best digit within swap range and bubble it in.
// Minimizing avoids leading zeros.
// Complexity: O(N * min(N, K)) time, O(N) space.

#include <bits/stdc++.h>
using namespace std;

string maximize(string s, int K) {
    int n = (int)s.size();
    for (int i = 0; i < n - 1 && K > 0; i++) {
        // Find position of the maximum digit in s[i..min(i+K, n-1)]
        int bestPos = i;
        for (int j = i + 1; j < n && j - i <= K; j++) {
            if (s[j] > s[bestPos]) {
                bestPos = j;
            }
        }
        // Bubble s[bestPos] to position i
        for (int j = bestPos; j > i; j--) {
            swap(s[j], s[j - 1]);
            K--;
        }
    }
    return s;
}

string minimize(string s, int K) {
    int n = (int)s.size();
    for (int i = 0; i < n - 1 && K > 0; i++) {
        int bestPos = i;
        for (int j = i + 1; j < n && j - i <= K; j++) {
            if (i == 0) {
                // Avoid leading zero: skip '0' candidates for first position
                if (s[j] == '0') continue;
                if (s[j] < s[bestPos] || s[bestPos] == '0') {
                    bestPos = j;
                }
            } else {
                if (s[j] < s[bestPos]) {
                    bestPos = j;
                }
            }
        }
        // Bubble s[bestPos] to position i
        for (int j = bestPos; j > i; j--) {
            swap(s[j], s[j - 1]);
            K--;
        }
    }
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string S;
    int K;
    cin >> S >> K;

    // Edge case: single digit
    if (S.size() <= 1) {
        cout << S << "\n" << S << "\n";
        return 0;
    }

    string maxResult = maximize(S, K);
    string minResult = minimize(S, K);

    cout << minResult << "\n";
    cout << maxResult << "\n";

    return 0;
}
