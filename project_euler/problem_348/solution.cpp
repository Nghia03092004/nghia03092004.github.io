#include <bits/stdc++.h>
using namespace std;

bool is_palindrome(long long n) {
    string s = to_string(n);
    string r = s;
    reverse(r.begin(), r.end());
    return s == r;
}

int main() {
    // We search palindromes up to some limit
    const long long LIMIT = 1000000000LL; // 10^9 should be enough

    // For each palindrome, count ways to write it as a^2 + b^3
    // We'll generate palindromes and check each

    vector<long long> results;

    // Generate palindromes in order by iterating through possible values
    // Instead, iterate all numbers and check palindrome + count
    // More efficient: generate palindromes directly

    // Generate palindromes by building from half
    vector<long long> palindromes;

    // Single digit palindromes
    for (int d = 1; d <= 9; d++) palindromes.push_back(d);

    // Generate multi-digit palindromes
    for (int half_len = 1; half_len <= 5; half_len++) {
        long long start = 1;
        for (int i = 1; i < half_len; i++) start *= 10;
        long long end_ = start * 10;

        // Even length: half_len * 2
        for (long long h = start; h < end_; h++) {
            string s = to_string(h);
            string r = s;
            reverse(r.begin(), r.end());
            long long p = stoll(s + r);
            if (p < LIMIT) palindromes.push_back(p);
        }

        // Odd length: half_len + 1 + half_len - but we handle it as:
        // left half (half_len digits) + middle digit + reversed left half
        // Actually simpler: for odd length 2*half_len+1
        for (long long h = start; h < end_; h++) {
            string s = to_string(h);
            string r = s.substr(0, s.size() - 1);
            reverse(r.begin(), r.end());
            long long p = stoll(s + r);
            if (p < LIMIT) palindromes.push_back(p);
        }
    }

    sort(palindromes.begin(), palindromes.end());
    palindromes.erase(unique(palindromes.begin(), palindromes.end()), palindromes.end());

    for (long long pal : palindromes) {
        if (pal < 2) continue; // need a>=1, b>=1, so min is 1+1=2
        int count = 0;
        for (long long b = 1; b * b * b < pal; b++) {
            long long rem = pal - b * b * b;
            long long a = (long long)round(sqrt((double)rem));
            if (a >= 1 && a * a == rem) {
                count++;
            }
        }
        if (count == 4) {
            results.push_back(pal);
            if (results.size() == 5) break;
        }
    }

    long long sum = 0;
    for (long long v : results) sum += v;
    cout << sum << endl;

    return 0;
}
