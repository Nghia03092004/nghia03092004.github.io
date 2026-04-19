// IOI 2000 - Palindrome
// Find minimum character insertions to make string S a palindrome.
// Answer = N - LPS(S), where LPS = longest palindromic subsequence.
// LPS(S) = LCS(S, reverse(S)), computed with O(N) space rolling array.
// Complexity: O(N^2) time, O(N) space.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    string S;
    cin >> S;

    // Edge case: empty or single character
    if (N <= 1) {
        cout << 0 << "\n";
        return 0;
    }

    // LPS(S) = LCS(S, reverse(S))
    string T(S.rbegin(), S.rend());

    // LCS with O(N) space using rolling arrays
    vector<int> prev(N + 1, 0), curr(N + 1, 0);
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (S[i - 1] == T[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j - 1]);
            }
        }
        swap(prev, curr);
        fill(curr.begin(), curr.end(), 0);
    }

    int lps = prev[N];
    cout << N - lps << "\n";

    return 0;
}
