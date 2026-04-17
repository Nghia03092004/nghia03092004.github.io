#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 305: Reflexive Position
 *
 * S = "123456789101112131415..."
 * f(n) = starting position of the n-th occurrence of str(n) in S.
 * Find sum of f(3^k) for k = 1..13.
 *
 * Strategy:
 * 1. Count occurrences of a pattern string in S up to a given position.
 * 2. Binary search for the position of the n-th occurrence.
 *
 * Occurrences are either:
 * (a) Internal: pattern appears within digits of a single number m.
 * (b) Boundary: pattern spans the junction of consecutive numbers m, m+1.
 */

typedef long long ll;
typedef __int128 lll;

// Number of digits of m
int num_digits(ll m) {
    if (m == 0) return 1;
    int d = 0;
    while (m > 0) { d++; m /= 10; }
    return d;
}

// 10^k
ll pow10(int k) {
    ll r = 1;
    for (int i = 0; i < k; i++) r *= 10;
    return r;
}

// Position in S where number m starts (1-indexed)
// S = "1 2 3 ... 9 10 11 ... 99 100 ..."
// Pos(1) = 1
ll pos_of(ll m) {
    if (m <= 0) return 1;
    ll pos = 1;
    int d = num_digits(m);
    for (int i = 1; i < d; i++) {
        pos += (ll)9 * i * pow10(i - 1);
    }
    pos += (ll)d * (m - pow10(d - 1));
    return pos;
}

// Which number contains position p in S, and offset within that number
// Returns {number, offset} where offset is 0-indexed digit position within that number
pair<ll, int> number_at_pos(ll p) {
    // p is 1-indexed
    ll remaining = p;
    int d = 1;
    while (true) {
        ll count_d = 9LL * pow10(d - 1);
        ll chars_d = count_d * d;
        if (remaining <= chars_d) {
            ll idx = (remaining - 1) / d; // 0-indexed among d-digit numbers
            int off = (remaining - 1) % d;
            ll num = pow10(d - 1) + idx;
            return {num, off};
        }
        remaining -= chars_d;
        d++;
    }
}

// Get digit at position p in S (1-indexed), returns '0'-'9'
int digit_at(ll p) {
    auto [num, off] = number_at_pos(p);
    string s = to_string(num);
    return s[off] - '0';
}

// Extract substring of S from position lo to hi (1-indexed, inclusive)
string extract(ll lo, ll hi) {
    string res;
    for (ll p = lo; p <= hi; p++) {
        res += (char)('0' + digit_at(p));
    }
    return res;
}

// Count how many d-digit numbers have string pat as a substring starting at position j
// (0-indexed within the d-digit number), among numbers in [lo_num, hi_num].
// This counts numbers where digits[j..j+L-1] == pat, with the constraint that the number
// is in range [lo_num, hi_num].
// Instead of full range counting, we count among all d-digit numbers and then adjust.
//
// Actually, for this problem, we need a different approach:
// Count occurrences of pat in S[1..P] for some position P.

// --------------------------------------------------------------------------
// Approach: For a given pattern string `pat`, count occurrences in S[1..P].
//
// Let M be the number such that S[1..P] contains all of 1..M-1 fully,
// plus some prefix of M.
//
// Occurrences split into:
// 1. Internal in numbers 1..M-1: pat appears within digits of single number m.
// 2. Boundary between m and m+1 for m = 1..M-2.
// 3. Internal in the visible prefix of M.
// 4. Boundary between M-1 and M (if the visible prefix is long enough).
// --------------------------------------------------------------------------

// Count occurrences of pat as substring within the string representation of m.
int count_in_number(ll m, const string& pat) {
    string s = to_string(m);
    int cnt = 0;
    size_t pos = 0;
    while ((pos = s.find(pat, pos)) != string::npos) {
        cnt++;
        pos++;
    }
    return cnt;
}

// Count how many numbers in [A, B] contain pat as a substring of their digits,
// counting with multiplicity (how many times it appears in each number).
// For large ranges, we need digit DP.
//
// Actually, for this problem the patterns are short (at most 7 digits for 3^13=1594323),
// so the pattern length L <= 7. We can count using digit DP.

// Digit DP: count total occurrences of pat in decimal representations of numbers in [1, N].
// State: position in number, how much of pat is matched (KMP automaton state),
//        tight constraint, leading zeros.

int fail_func[20]; // KMP failure function
int pat_len;
string global_pat;

void build_kmp(const string& pat) {
    global_pat = pat;
    pat_len = pat.size();
    fail_func[0] = -1;
    for (int i = 1; i < pat_len; i++) {
        int j = fail_func[i - 1];
        while (j >= 0 && pat[j + 1] != pat[i]) j = fail_func[j];
        fail_func[i] = (pat[j + 1] == pat[i]) ? j + 1 : -1;
    }
}

// KMP transition: given current match state `state` (number of chars matched - 1, or -1 for none),
// and next character c, return new state.
int kmp_trans(int state, char c) {
    while (state >= 0 && global_pat[state + 1] != c) state = fail_func[state];
    if (global_pat[state + 1] == c) state++;
    return state;
}

// Precompute KMP transition table for all states and digits
int kmp_table[20][10]; // kmp_table[state+1][digit] (shift state by 1 to avoid -1 index)

void build_kmp_table() {
    for (int s = -1; s < pat_len; s++) {
        for (int d = 0; d <= 9; d++) {
            kmp_table[s + 1][d] = kmp_trans(s, '0' + d);
        }
    }
}

// Digit DP to count total occurrences of pat in numbers [1, N]
// Returns total count of substring matches across all numbers in [1, N].
ll count_internal(ll N) {
    if (N <= 0) return 0;
    string num = to_string(N);
    int n = num.size();

    // dp[pos][kmp_state][tight][started] = {count of numbers, total occurrences}
    // This is complex. Let's use memoized recursion.

    // dp state: (pos, kmp_state, tight, started) -> (ways, total_occurrences)
    // pos: current digit position (0-indexed)
    // kmp_state: -1 to pat_len-1 (shifted to 0..pat_len in table)
    // tight: 0 or 1
    // started: 0 or 1 (have we placed a non-zero digit yet?)

    // Use map for memo
    map<tuple<int,int,int,int>, pair<ll,ll>> memo;

    function<pair<ll,ll>(int, int, int, int)> solve = [&](int pos, int kmp_st, int tight, int started) -> pair<ll,ll> {
        if (pos == n) {
            return {started ? 1 : 0, 0};
        }
        auto key = make_tuple(pos, kmp_st, tight, started);
        auto it = memo.find(key);
        if (it != memo.end()) return it->second;

        int limit = tight ? (num[pos] - '0') : 9;
        ll ways = 0, occ = 0;
        for (int d = 0; d <= limit; d++) {
            int new_tight = tight && (d == limit);
            int new_started = started || (d > 0);
            int new_kmp;
            if (!new_started) {
                new_kmp = -1; // haven't started, no matching
            } else {
                new_kmp = kmp_table[kmp_st + 1][d];
            }
            int match = (new_kmp == pat_len - 1) ? 1 : 0;
            // If we matched, reset kmp state
            int next_kmp = new_kmp;
            if (match) {
                // After a full match, continue from the failure of the last state
                next_kmp = fail_func[pat_len - 1];
            }
            auto [w, o] = solve(pos + 1, next_kmp, new_tight, new_started);
            ways += w;
            occ += o + (ll)match * w;
        }
        memo[key] = {ways, occ};
        return {ways, occ};
    };

    auto [w, o] = solve(0, -1, 1, 0);
    return o;
}

// Count boundary occurrences of pat between consecutive numbers up to M-1.
// A boundary occurrence happens when the last i digits of m concatenated with
// the first (L-i) digits of m+1 form pat, for some 1 <= i <= L-1.
// We need to count such m in [1, M-1].
//
// For each split point i (1 <= i <= L-1):
//   suffix of m (last i chars) = pat[0..i-1]
//   prefix of m+1 (first L-i chars) = pat[i..L-1]
//
// We need: last i digits of m equal pat[0..i-1], AND first (L-i) digits of (m+1) equal pat[i..L-1].

// Count numbers m in [1, M-1] such that:
// - last i digits of m are pat[0..i-1]
// - first (L-i) digits of m+1 are pat[i..L-1]
ll count_boundary(ll M, const string& pat) {
    int L = pat.size();
    ll total = 0;

    for (int i = 1; i < L; i++) {
        string suffix = pat.substr(0, i);    // last i digits of m must be this
        string prefix = pat.substr(i);       // first (L-i) digits of m+1 must be this

        // suffix as number
        ll suf_val = stoll(suffix);

        // For each digit length d of m (d >= i):
        // m mod 10^i == suf_val
        // m+1 starts with prefix
        //
        // m+1 starts with prefix means: if m+1 has D digits, then
        // floor(m+1 / 10^(D - (L-i))) == prefix_val (interpreted as number)
        // But prefix might have leading zeros if pat[i] == '0', which means
        // m+1 starts with '0'... that's impossible unless m+1 = 0, which doesn't happen.
        // Actually prefix of m+1 means the first (L-i) digits. If L-i > digits of m+1, skip.

        ll prefix_val = stoll(prefix);
        int plen = prefix.size(); // L - i

        // If prefix has leading zeros, then m+1 must start with zeros - impossible for positive numbers.
        // Unless plen == 1 and prefix == "0", but m+1 >= 2, starts with nonzero.
        // Actually, prefix is a string - "first L-i digits of m+1" means the leading digits.
        // If prefix starts with '0', no positive integer starts with '0', so count = 0 for this split.
        if (prefix[0] == '0') continue;

        // m+1 has D digits and starts with prefix_val in the top plen digits:
        // m+1 in [prefix_val * 10^(D-plen), (prefix_val+1) * 10^(D-plen))
        // equivalently m in [prefix_val * 10^(D-plen) - 1, (prefix_val+1) * 10^(D-plen) - 2]
        //
        // Also m mod 10^i == suf_val, and m >= 1, m <= M-1.

        // Iterate over digit lengths D of m+1
        for (int D = plen; D <= 16; D++) {
            ll lo_m1 = prefix_val * pow10(D - plen);       // m+1 >= lo_m1
            ll hi_m1 = (prefix_val + 1) * pow10(D - plen) - 1; // m+1 <= hi_m1

            // Check that lo_m1 has D digits
            if (lo_m1 < pow10(D - 1)) continue; // prefix_val too small for D digits
            if (lo_m1 >= pow10(D)) continue;     // prefix_val too large for D digits

            ll lo_m = lo_m1 - 1; // m >= lo_m
            ll hi_m = hi_m1 - 1; // m <= hi_m

            // Clamp to [1, M-1]
            lo_m = max(lo_m, 1LL);
            hi_m = min(hi_m, M - 1);
            if (lo_m > hi_m) continue;

            // m must have at least i digits for the suffix condition to make sense
            // (if m has < i digits, the "last i digits" would include leading zeros of m, which don't exist)
            // Actually, suffix is pat[0..i-1]. If pat[0] != '0', then suf_val has exactly i digits,
            // and m must be >= 10^(i-1) for last i digits to be suf_val (no leading zeros issue,
            // last i digits is just m mod 10^i).
            // If pat[0] == '0', then suf_val < 10^(i-1), and m mod 10^i == suf_val is fine.

            ll mod = pow10(i);
            // Count m in [lo_m, hi_m] with m % mod == suf_val
            // Need suf_val < mod
            if (suf_val >= mod) continue;

            // First m >= lo_m with m % mod == suf_val
            ll first_m = lo_m + ((suf_val - lo_m % mod) % mod + mod) % mod;
            if (first_m > hi_m) continue;

            ll cnt = (hi_m - first_m) / mod + 1;
            total += cnt;
        }
    }

    return total;
}

// Total occurrences of pat in S[1..P]
// P is a position in S.
ll count_occurrences(ll P, const string& pat) {
    int L = pat.size();
    if (P < L) return 0;

    // Find what number is at position P
    auto [M, off] = number_at_pos(P);
    // S[1..P] contains all of 1..M fully? Only if off == num_digits(M) - 1.
    // Otherwise S[1..P] contains 1..M-1 fully, plus digits 0..off of M.

    ll full_up_to; // we have complete numbers 1..full_up_to
    ll partial_num; // partial number (or 0 if P ends exactly at a number boundary)
    int partial_digits; // how many digits of partial_num are visible (1-indexed)

    if (off == num_digits(M) - 1) {
        full_up_to = M;
        partial_num = 0;
        partial_digits = 0;
    } else {
        full_up_to = M - 1;
        partial_num = M;
        partial_digits = off + 1;
    }

    ll total = 0;

    // 1. Internal occurrences in numbers 1..full_up_to
    total += count_internal(full_up_to);

    // 2. Boundary occurrences between m and m+1 for m in [1, full_up_to - 1]
    if (full_up_to >= 2) {
        total += count_boundary(full_up_to, pat);
    }

    // 3. Internal + boundary occurrences involving the partial number
    if (partial_num > 0 && partial_digits >= L) {
        // Check if pat appears in the visible prefix of partial_num
        string s = to_string(partial_num).substr(0, partial_digits);
        size_t pos = 0;
        while ((pos = s.find(pat, pos)) != string::npos) {
            total++;
            pos++;
        }
    }

    // 4. Boundary between full_up_to and full_up_to+1 (which might be partial_num)
    if (full_up_to >= 1) {
        // Check boundary between full_up_to and full_up_to+1
        // The boundary region: last few digits of full_up_to + first few digits of full_up_to+1
        // We have all of full_up_to, and at least 0 digits of full_up_to+1.
        string s_left = to_string(full_up_to);
        string s_right;
        if (partial_num == full_up_to + 1) {
            s_right = to_string(partial_num).substr(0, partial_digits);
        } else if (partial_num == 0) {
            // P ends at full_up_to, so no digits of next number visible
            // But boundary could still be checked with what we have
            s_right = "";
        } else {
            s_right = "";
        }

        // Check boundary: last i chars of s_left + first (L-i) chars of s_right == pat
        for (int i = 1; i < L; i++) {
            int need_right = L - i;
            if (i > (int)s_left.size()) continue;
            if (need_right > (int)s_right.size()) continue;
            string candidate = s_left.substr(s_left.size() - i) + s_right.substr(0, need_right);
            if (candidate == pat) total++;
        }
    }

    return total;
}

// Find f(n) = position of n-th occurrence of str(n) in S.
ll find_f(ll n) {
    string pat = to_string(n);
    int L = pat.size();

    build_kmp(pat);
    build_kmp_table();

    // Binary search for position P such that count_occurrences(P, pat) >= n
    // and count_occurrences(P-1, pat) < n.
    // The answer f(n) = P - L + 1 (starting position of the match).

    // Actually, f(n) is the starting position of the n-th occurrence.
    // An occurrence at starting position p means pat matches S[p..p+L-1].
    // count_occurrences(P, pat) counts occurrences whose starting position <= P - L + 1...
    // Actually no: it counts occurrences of pat that are fully within S[1..P].
    // An occurrence starting at position p is fully within S[1..P] iff p + L - 1 <= P, i.e., p <= P - L + 1.

    // So we want the smallest P such that count_occurrences(P, pat) >= n.
    // Then f(n) = P - L + 1? No...
    // count_occurrences(P, pat) = number of occurrences starting at positions 1..P-L+1.
    // We want the n-th starting position.

    // Let's redefine: count(pos) = number of occurrences of pat starting at position <= pos.
    // Then f(n) = smallest pos such that count(pos) >= n.
    // count(pos) = count_occurrences(pos + L - 1, pat).

    // Binary search on starting position.
    ll lo = 1, hi = 2e16; // upper bound

    // Verify that our count function works for small cases.
    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2;
        ll P = mid + L - 1; // end position of an occurrence starting at mid
        if (count_occurrences(P, pat) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }

    return lo;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll total = 0;
    ll power = 1;
    for (int k = 1; k <= 13; k++) {
        power *= 3;
        ll result = find_f(power);
        total += result;
    }
    cout << total << endl;
    return 0;
}
