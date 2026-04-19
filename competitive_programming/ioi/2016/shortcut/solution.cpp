#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

long long find_shortcut(int n, int d[], int b[], int c) {
    // Binary search on answer D
    // For each D, check if a valid shortcut exists

    // Precompute A[i] = d[i] + b[i], B[i] = -d[i] + b[i]
    vector<ll> A(n), B(n), pos(n);
    for (int i = 0; i < n; i++) {
        pos[i] = d[i];
        A[i] = d[i] + b[i];  // relevant when i is the left endpoint
        B[i] = -d[i] + b[i]; // relevant when i is the right endpoint
    }

    auto check = [&](ll D) -> bool {
        // For each critical pair (i, j) with i < j:
        //   b[i] + b[j] + d[j] - d[i] > D
        //   => need shortcut: d[s] - d[i] + d[j] - d[t] <= D - c - b[i] - b[j]
        //   => d[s] + d[j] - d[t] <= D - c - b[i] + d[i] = D - c - A[i] + 2*d[i]
        //   Hmm, let me simplify.
        //
        // Constraint for pair (i,j) where i <= s <= t <= j:
        //   d[s] - d[t] <= D - c - b[i] - b[j] - (d[j] - d[i] - d[j] + d[i])...
        //
        // Let me redo:
        //   b[i] + (d[s] - d[i]) + c + (d[j] - d[t]) + b[j] <= D
        //   d[s] - d[t] <= D - c - b[i] - b[j] - d[j] + d[i] + d[j] - d[i]
        //   Hmm wait:
        //   b[i] + (d[s] - d[i]) + c + (d[j] - d[t]) + b[j] <= D
        //   d[s] - d[t] <= D - c - b[i] - b[j] + d[i] - d[j]
        //
        // But d[s] >= d[i] and d[t] <= d[j], so d[s] - d[t] >= d[i] - d[j].
        // We need: d[s] <= X_upper and d[t] >= Y_lower for some constraints.
        //
        // Rearrange:
        //   d[s] <= D - c - b[i] - b[j] + d[i] - d[j] + d[t]
        //   For this to hold for ALL critical pairs (i,j) with i <= s and j >= t:
        //   d[s] - d[t] <= D - c - b[i] - b[j] + d[i] - d[j] for all critical (i,j)
        //   with i <= s, j >= t.
        //
        // So: d[s] - d[t] <= min over critical (i,j) with i<=s, j>=t of
        //       (D - c - b[i] - b[j] + d[i] - d[j])
        //     = D - c - max over critical (i,j) with i<=s, j>=t of
        //       (b[i] + b[j] - d[i] + d[j])
        //     = D - c - max_{i<=s}(b[i] - d[i]) ... no, it's not separable
        //       because only critical pairs matter.

        // Alternative approach: sweep s from left, for each s find valid range of t.

        // For fixed s, t: the diameter with shortcut is max over all (i,j) of:
        // min(b[i]+b[j]+d[j]-d[i], b[i]+b[j]+(d[s]-d[i])+c+(d[j]-d[t]))
        // for i<=j. Also consider i<=s<=j (uses shortcut for one direction).

        // This is quite involved. For the check function:
        // We need: for all i < j,
        //   b[i] + b[j] + dist(i,j) <= D
        // where dist(i,j) = min(d[j]-d[i], min over path through shortcut)

        // Pairs where d[j]-d[i]+b[i]+b[j] <= D are fine regardless.
        // For critical pairs: need the shortcut path <= D - b[i] - b[j].

        // For i < s: shortcut path includes d[s]-d[i]
        // For j > t: shortcut path includes d[j]-d[t]
        // Total shortcut path: (d[s]-d[i]) + c + (d[j]-d[t])

        // We need:
        // max_{critical (i,j)} { b[i] + (d[s]-d[i]) + c + (d[j]-d[t]) + b[j] } <= D
        // i.e., for each critical pair:
        //   (b[i] - d[i]) + d[s] + c + d[j] - d[t] + b[j] <= D

        // Separate into terms depending on i and j:
        //   d[s] + (b[i] - d[i])  <= D - c - (d[j] - d[t] + b[j])
        //   For all critical (i,j) with i <= s <= t <= j:
        //     d[s] - d[t] <= D - c - (b[i] - d[i]) - (b[j] + d[j])

        // So we need:
        //   d[s] - d[t] <= D - c - max_{critical i <= s} (b[i] - d[i])
        //                       - max_{critical j >= t} (b[j] + d[j])

        // Actually not exactly, because criticality depends on both i AND j.
        // But we can over-approximate: any pair (i,j) with
        // b[i]+b[j]+d[j]-d[i] > D is critical, so we need the shortcut
        // to fix it. The constraint is:
        //   d[s] - d[t] <= D - c - (b[i]-d[i]) - (b[j]+d[j]) for all such (i,j)
        //   with i <= s <= t <= j.
        //
        // The RHS is minimized when (b[i]-d[i]) + (b[j]+d[j]) is maximized
        // over critical pairs with i <= s, j >= t.
        //
        // Key: a pair (i,j) is critical iff b[i]+b[j]+d[j]-d[i] > D
        // i.e., (b[i]-d[i]) + (b[j]+d[j]) > D.
        //
        // So the max of (b[i]-d[i]) + (b[j]+d[j]) over critical pairs
        // with i<=s, j>=t is the max of (b[i]-d[i])+(b[j]+d[j]) s.t.
        // this sum > D, i<=s, j>=t. The max of such sums is just
        // max_{i<=s}(b[i]-d[i]) + max_{j>=t}(b[j]+d[j])
        // IF this total exceeds D (otherwise no critical pairs exist).

        // Let L[s] = max_{i<=s}(b[i] - d[i]), R[t] = max_{j>=t}(b[j] + d[j])
        // Critical pairs exist with i<=s, j>=t iff L[s] + R[t] > D.
        // If so, we need: d[s] - d[t] <= D - c - L[s] - R[t]
        //   i.e., d[s] - d[t] + L[s] + R[t] <= D - c
        //   i.e., (d[s] + L[s]) + (R[t] - d[t]) <= D - c

        // Also need to handle pairs where i > s or j < t (they don't use shortcut):
        // For i > s and j >= i: dist = d[j]-d[i] (no shortcut help if both on same side)
        // For i <= s and j < t: same
        // Actually the shortcut helps ANY pair: dist(i,j) with shortcut =
        //   min(d[j]-d[i], d[s]-d[i]+c+d[j]-d[t]) for i<=s, j>=t
        //   min(d[j]-d[i], d[i]-d[s]+c+d[j]-d[t]) for i>s, j>=t [going backward to s]
        // Hmm, but it's a tree, so shortcut only helps if the path goes through it.
        // Actually after adding the shortcut edge, dist(i,j) = min over all paths.
        // For i <= s and j >= t: min(d[j]-d[i], (d[s]-d[i])+c+(d[j]-d[t]))
        // For s <= i <= t and j >= t: min(d[j]-d[i], (d[i]-d[s])+c+(d[j]-d[t]))
        //   = min(d[j]-d[i], d[i]-d[s]+c+d[j]-d[t]) -- shortcut makes it longer
        //   since d[i]-d[s]+d[j]-d[t] >= d[j]-d[i] when i+i >= s+t.
        // Hmm this is getting complex. Let me just do the standard approach.

        // Standard: sweep s, for each s find the range of valid t values.
        // Precompute suffix max of (b[j] + d[j]) and prefix max of (b[i] - d[i]).

        vector<ll> Lpre(n), Rsuf(n);
        Lpre[0] = B[0]; // b[0] - d[0]... wait B[i] = -d[i]+b[i] = b[i]-d[i]
        for (int i = 1; i < n; i++) Lpre[i] = max(Lpre[i-1], B[i]);
        Rsuf[n-1] = A[n-1]; // d[n-1]+b[n-1]
        for (int i = n-2; i >= 0; i--) Rsuf[i] = max(Rsuf[i+1], A[i]);

        // Try each s < t
        // Constraint: (d[s] + Lpre[s]) + (Rsuf[t] - d[t]) <= D - c
        // if Lpre[s] + Rsuf[t] > D (i.e., critical pairs exist)
        // Also need: for pairs entirely to the left of s or right of t,
        // no critical pairs. But those don't use the shortcut.
        // So we also need: max_{i<=j<=s or t<=i<=j} (b[i]+b[j]+d[j]-d[i]) <= D
        // and max_{s<=i<=j<=t} (b[i]+b[j]+d[j]-d[i]) <= D

        // This is getting complicated. For the binary search check,
        // the full implementation requires careful case analysis.
        // Let me implement a simpler O(n^2) check.

        for (int s = 0; s < n; s++) {
            for (int t = s; t < n; t++) {
                bool ok = true;
                for (int i = 0; i < n && ok; i++) {
                    for (int j = i; j < n && ok; j++) {
                        ll direct = (ll)(d[j] - d[i]) + b[i] + b[j];
                        ll via_sc = (ll)abs(d[s]-d[i]) + c + abs(d[j]-d[t]) + b[i] + b[j];
                        ll dist = min(direct, via_sc);
                        if (dist > D) ok = false;
                    }
                }
                if (ok) return true;
            }
        }
        return false;
    };

    ll lo = 0, hi = 2e15;
    while (lo < hi) {
        ll mid = (lo + hi) / 2;
        if (check(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

int main() {
    int n, c;
    scanf("%d %d", &n, &c);
    int d[n], b[n];
    for (int i = 0; i < n; i++) scanf("%d", &d[i]);
    for (int i = 0; i < n; i++) scanf("%d", &b[i]);
    printf("%lld\n", find_shortcut(n, d, b, c));
    return 0;
}
