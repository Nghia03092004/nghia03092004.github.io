#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Count occurrences of digit d in all numbers from 1 to n
ll f(ll n, int d) {
    if (n <= 0) return 0;
    ll count = 0;
    ll power = 1;
    while (power <= n) {
        ll high = n / (power * 10);
        ll cur = (n / power) % 10;
        ll low = n % power;
        if (cur < d)
            count += high * power;
        else if (cur == d)
            count += high * power + low + 1;
        else
            count += (high + 1) * power;
        power *= 10;
    }
    return count;
}

ll total_sum = 0;

// g(n) = f(n,d) - n
// We want all n where g(n) = 0.
// f(n,d) is non-decreasing. f(n+1,d) - f(n,d) = number of d's in (n+1), which is 0..12ish.
// So g(n+1) - g(n) = (count of d in n+1) - 1, range [-1, ~11].
// g can oscillate but has long-term trend toward -infinity for large n.

void solve(ll lo, ll hi, int d) {
    if (lo > hi) return;

    ll glo = f(lo, d) - lo;
    ll ghi = f(hi, d) - hi;

    // Pruning based on bounds:
    // In interval [lo, hi], g can decrease by at most 1 per step (when digit d doesn't appear).
    // So min of g in [lo, hi] >= glo - (hi - lo) and also >= ghi - (hi - lo).
    // max of g in [lo, hi]: g can increase by at most (max_digits - 1) per step.
    // But a simpler bound: f is non-decreasing, so f(n,d) >= f(lo,d) for n >= lo.
    // g(n) = f(n,d) - n >= f(lo,d) - n >= f(lo,d) - hi = glo - (hi - lo).
    // Also g(n) = f(n,d) - n <= f(hi,d) - n <= f(hi,d) - lo = ghi + (hi - lo).

    ll g_min_bound = min(glo, ghi);  // not tight but...
    ll g_max_bound = max(glo, ghi);

    // Tighter: g(n) >= f(lo,d) - hi = glo + lo - hi = glo - (hi - lo)
    ll g_lower = glo - (hi - lo);
    // g(n) <= f(hi,d) - lo = ghi + hi - lo
    ll g_upper = ghi + (hi - lo);

    // If g is always positive or always negative, skip
    if (g_lower > 0) return;  // g(n) > 0 for all n in [lo, hi]
    if (g_upper < 0) return;  // g(n) < 0 for all n in [lo, hi]

    if (lo == hi) {
        if (glo == 0) {
            total_sum += lo;
        }
        return;
    }
    ll mid = lo + (hi - lo) / 2;
    solve(lo, mid, d);
    solve(mid + 1, hi, d);
}

int main() {
    ios::sync_with_stdio(false);
    ll LIMIT = 200000000000LL; // 2 * 10^11
    for (int d = 1; d <= 9; d++) {
        solve(1, LIMIT, d);
    }
    cout << total_sum << endl;
    return 0;
}
