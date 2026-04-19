// IOI 2018 - Combo
// Interactive: guess a secret string over {A, B, X, Y}.
// press(p) returns the length of the longest common prefix of secret and p.
// Strategy: 2 queries for first char, then 1 query per middle char
// using the trick of encoding 3 candidates in one query string.
// Total queries: N + 1 (or up to 2N in the simple fallback).
#include <bits/stdc++.h>
using namespace std;

// Grader-provided function
int press(string p);

string guess_sequence(int N) {
    string chars = "ABXY";

    // Step 1: Determine first character using at most 2 queries
    string S;
    int r = press("AB");
    if (r >= 1) {
        // First char is A or B
        S = (press("A") == 1) ? "A" : "B";
    } else {
        // First char is X or Y
        S = (press("X") == 1) ? "X" : "Y";
    }

    if (N == 1) return S;

    // The known first character, used as padding in the query trick
    char w = S[0];
    string others;
    for (char c : chars)
        if (c != w) others += c;
    // others = {a, b, c} -- the three other characters

    // Step 2: For positions 1 through N-2, use 1 query per character.
    // Query: P + a + w + P + a + b + P + a + c
    // - Result |P|+2: next char is 'a' and the one after is 'w' (learn 2 chars!)
    // - Result |P|+1: next char is 'a' (the one after is not 'w')
    // - Result |P|:   next char is 'w', 'b', or 'c' -- need 1 more query
    //
    // For the simple reliable approach: test two candidates per position.
    // Worst case 2 queries per char, but last char needs at most 2.

    char a = others[0], b = others[1], c = others[2];

    for (int k = (int)S.size(); k < N - 1; k++) {
        // Try the 3-candidate encoding trick first
        string P = S;
        string q = P + a + w + P + a + b + P + a + c;
        int res = press(q);
        int plen = (int)P.size();

        if (res == plen + 2) {
            // Next char is 'a', char after that is 'w'
            S += a;
            S += w;
            k++; // we learned two characters
        } else if (res == plen + 1) {
            // Next char is 'a'
            S += a;
        } else {
            // Next char is one of {w, b, c}
            res = press(P + b);
            if (res == plen + 1) {
                S += b;
            } else {
                res = press(P + c);
                if (res == plen + 1) S += c;
                else S += w;
            }
        }
    }

    // Last character (if not already determined)
    if ((int)S.size() < N) {
        int res = press(S + a);
        if (res == N) { S += a; }
        else {
            res = press(S + b);
            if (res == N) S += b;
            else {
                res = press(S + c);
                if (res == N) S += c;
                else S += w;
            }
        }
    }

    return S;
}

int main() {
    int N;
    scanf("%d", &N);
    string result = guess_sequence(N);
    printf("%s\n", result.c_str());
    return 0;
}
