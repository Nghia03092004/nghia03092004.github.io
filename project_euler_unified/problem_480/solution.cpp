#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 480: The Last Question
 *
 * Count positions of words formed from letters of
 * "thereisasyetinsufficientdataforameaningfulanswer"
 * with at most 15 letters, in alphabetical order.
 */

typedef long long ll;
typedef __int128 lll;

const string phrase = "thereisasyetinsufficientdataforameaningfulanswer";

// Letter frequencies
map<char, int> getFreqs() {
    map<char, int> freq;
    for (char c : phrase) freq[c]++;
    return freq;
}

// Sorted unique letters
vector<char> letters;
vector<int> maxcount; // max available count for each letter

// Count number of distinct words of length exactly len
// that can be formed from the given available counts
// Uses DP: dp[i][j] = number of ways using first i letter types, total j letters
// Weighted by multinomial coefficients

// We need: sum over all valid selections of length len: len! / prod(s_i!)
// = len! * sum over selections: 1/prod(s_i!)

// Precompute factorials
double factorial[20];
ll fact[20];

void initFact() {
    fact[0] = 1;
    for (int i = 1; i <= 16; i++) fact[i] = fact[i-1] * i;
}

// Count words of length exactly `len` from available letter counts
// Returns the count as a long long
ll countWordsOfLength(const vector<int>& avail, int len) {
    int K = avail.size();
    if (len == 0) return 1; // empty word (but we don't count it)

    // dp[j] = sum over valid selections using first i letters totaling j: 1/prod(s_i!)
    // Actually we work with integers: dp[j] = number of words of length j
    // using generating functions: product of (sum_{s=0}^{avail[i]} x^s / s!) * len!

    // Use DP: dp[j] = sum of 1/(s_1!...s_i!) for selections summing to j
    // Then answer = fact[len] * dp[len]

    // But this gives rational numbers. Better: use the multinomial directly.
    // dp[j] = number of distinct permutations of j letters chosen from first i types
    // dp[j] = sum over s_i from 0 to min(avail[i], j): dp_prev[j - s_i] * C(j, s_i) ... no

    // Correct DP:
    // Let dp[i][j] = number of distinct words of length j using letter types 0..i-1
    // dp[0][0] = 1
    // dp[i][j] = sum_{s=0}^{min(avail[i-1], j)} dp[i-1][j-s] * j! / ((j-s)! * s!)
    //          = sum_{s=0}^{min(avail[i-1], j)} dp[i-1][j-s] * C(j, s)

    // Wait, that's not right either because dp[i-1][j-s] already accounts for ordering.
    // Actually: dp[i][j] = sum_{s=0}^{min(avail[i-1],j)} dp[i-1][j-s] * C(j, s)
    // where C(j,s) = j! / (s! * (j-s)!) chooses positions for the s copies of letter i

    // This IS correct: we're building a word of length j, choosing s positions for letter i,
    // and the remaining j-s positions are filled by letters 0..i-2.

    vector<ll> dp(len + 1, 0);
    dp[0] = 1;

    for (int i = 0; i < K; i++) {
        vector<ll> ndp(len + 1, 0);
        for (int j = 0; j <= len; j++) {
            if (dp[j] == 0) continue;
            for (int s = 0; s <= min(avail[i], len - j); s++) {
                // C(j+s, s) = (j+s)! / (j! * s!)
                ll comb = fact[j + s] / fact[j] / fact[s];
                ndp[j + s] += dp[j] * comb;
            }
        }
        dp = ndp;
    }
    return dp[len];
}

// Count all words of length 1..maxlen from available counts,
// that are lexicographically < word w (or equal to w up to position pos)
// Plus all words of length < len(w)

ll countWordsBefore(const string& w, const vector<int>& origAvail) {
    int K = letters.size();
    ll pos = 0;

    // Count all words of length 1 to len(w)-1
    vector<int> avail = origAvail;
    for (int len = 1; len < (int)w.size(); len++) {
        pos += countWordsOfLength(avail, len);
    }

    // Count words of same length that come before
    int wlen = w.size();
    vector<int> cur = origAvail;
    for (int i = 0; i < wlen; i++) {
        // For each letter smaller than w[i], count words starting with that letter
        // at position i, given remaining available letters
        for (int li = 0; li < K; li++) {
            if (letters[li] >= w[i]) break;
            if (cur[li] > 0) {
                cur[li]--;
                pos += countWordsOfLength(cur, wlen - i - 1);
                cur[li]++;
            }
        }
        // Use letter w[i]
        int idx = find(letters.begin(), letters.end(), w[i]) - letters.begin();
        if (idx >= K || cur[idx] <= 0) {
            // Word cannot be formed - error
            return -1;
        }
        cur[idx]--;
    }

    return pos + 1; // +1 for 1-indexed
}

string findWordAtPosition(ll target, const vector<int>& origAvail) {
    int K = letters.size();
    vector<int> avail = origAvail;

    // First determine the length
    ll cumulative = 0;
    int wordLen = 0;
    for (int len = 1; len <= 15; len++) {
        ll cnt = countWordsOfLength(avail, len);
        if (cumulative + cnt >= target) {
            wordLen = len;
            target -= cumulative;
            break;
        }
        cumulative += cnt;
    }

    // Now find the word of length wordLen at position target (1-indexed)
    string result;
    vector<int> cur = avail;
    for (int i = 0; i < wordLen; i++) {
        for (int li = 0; li < K; li++) {
            if (cur[li] > 0) {
                cur[li]--;
                ll cnt = countWordsOfLength(cur, wordLen - i - 1);
                if (cnt >= target) {
                    result += letters[li];
                    break;
                }
                target -= cnt;
                cur[li]++;
            }
        }
    }
    return result;
}

int main() {
    initFact();

    map<char, int> freq = getFreqs();
    for (auto& p : freq) {
        letters.push_back(p.first);
        maxcount.push_back(p.second);
    }

    // Compute positions
    ll p_legionary = countWordsBefore("legionary", maxcount);
    ll p_calorimeters = countWordsBefore("calorimeters", maxcount);
    ll p_annihilate = countWordsBefore("annihilate", maxcount);
    ll p_orchestrated = countWordsBefore("orchestrated", maxcount);
    ll p_fluttering = countWordsBefore("fluttering", maxcount);

    printf("P(legionary) = %lld\n", p_legionary);
    printf("P(calorimeters) = %lld\n", p_calorimeters);
    printf("P(annihilate) = %lld\n", p_annihilate);
    printf("P(orchestrated) = %lld\n", p_orchestrated);
    printf("P(fluttering) = %lld\n", p_fluttering);

    ll target = p_legionary + p_calorimeters - p_annihilate + p_orchestrated - p_fluttering;
    printf("Target position = %lld\n", target);

    string answer = findWordAtPosition(target, maxcount);
    printf("Answer: %s\n", answer.c_str());

    return 0;
}
