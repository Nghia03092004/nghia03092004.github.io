#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    int n;
    vector<long long> mn, lazy;
    // mn[v] = minimum value in the segment
    // We also need to find the k-th position efficiently.

    // Alternative: maintain a sorted structure.
    // Actually, we use BIT on the count array with binary search.
    ;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<pair<int,int>> masts(N); // (height, num_sails)
    int H = 0;
    for(int i = 0; i < N; i++){
        cin >> masts[i].first >> masts[i].second;
        H = max(H, masts[i].first);
    }

    // Sort by height ascending
    sort(masts.begin(), masts.end());

    // c[k] = number of sails at height k (1-indexed)
    vector<int> c(H + 1, 0);

    // For each mast, place s sails among heights 1..h at positions
    // with smallest c values.
    // Since masts are processed in order of increasing height,
    // c is non-decreasing after each step (when sails are placed optimally).

    // Use a segment tree that supports:
    // 1. Find the s-th smallest position (by c-value) among [1, h]
    // 2. Range increment on a set of positions

    // Alternative O(N * H) approach (sufficient for N, H <= 100000 with care):
    // For each mast, sort heights 1..h by c[k], pick the s smallest,
    // increment them. But sorting takes O(H log H) per mast = too slow.

    // Efficient approach: note that c array is always "nearly sorted"
    // (non-decreasing if we process optimally). Actually, after processing
    // masts in order of height, the c array remains such that c[k] is
    // non-increasing for k = 1..H (taller heights get fewer sails).
    // Wait, the opposite: shorter heights are available to more masts,
    // so they get more sails. So c[k] is non-increasing: c[1] >= c[2] >= ...

    // If c is non-increasing, then for mast with height h and s sails,
    // the s positions with smallest c values among [1..h] are
    // positions h-s+1, h-s+2, ..., h (the tallest s positions available,
    // which have the smallest c values since c is non-increasing).

    // So we increment c[h-s+1..h] by 1.
    // After the increment, c might no longer be non-increasing.
    // We need to fix: if c[h-s+1] > c[h-s], swap/adjust.
    // Actually, incrementing a suffix might create a "bump" that needs smoothing.

    // After incrementing c[h-s+1..h]:
    // c[h-s+1] was previously c[h-s+1] (= c[h]), now becomes c[h]+1.
    // c[h-s] was previously >= c[h-s+1] = c[h]. Now c[h-s+1] = c[h]+1.
    // If c[h-s] >= c[h]+1, no problem. Otherwise, c[h-s] < c[h]+1 = c[h-s+1],
    // violating non-increasing. Need to extend the increment region.

    // More careful: after incrementing [h-s+1..h], some positions at the
    // boundary might need reordering. Since we're working with counts,
    // we can use the following approach:

    // Binary search for the lowest position p in [1..h] where c[p] == c[h-s+1].
    // Then among positions [p..h-s+1..some range] with the same value,
    // increment the rightmost s - (h - (h-s+1)) of them.

    // Clean implementation using BIT for prefix sums of the c distribution:

    // Let's use the simple O(N * H) approach with the non-increasing property:
    for(auto& [h, s] : masts){
        // c[1] >= c[2] >= ... >= c[h] >= ... >= c[H]
        // Want to increment s positions among [1..h] with smallest c values.
        // These are positions [h-s+1..h].
        // But after incrementing, need to maintain non-increasing order.

        // Find the value at position h-s+1 before increment
        int threshold = c[h - s + 1];

        // Find range of positions with value == threshold in [1..h]
        // All positions in [h-s+1..h] with c[k] == threshold will be incremented.
        // But some positions < h-s+1 might also have c[k] == threshold.
        // We need to choose: increment the RIGHTMOST positions with this value.

        // Positions with c[k] == threshold: find the leftmost in [1..h].
        int left = 1;
        while(left <= h && c[left] > threshold) left++;
        // Now [left..something] all have value == threshold (in the non-increasing array).
        // Actually, positions with c[k] == threshold form a contiguous range
        // [left..right] in the non-increasing array.
        int right = left;
        while(right <= h && c[right] == threshold) right++;
        right--; // now [left..right] has c[k] == threshold

        // Among [h-s+1..h], some have c == threshold, rest have c < threshold.
        // Positions with c < threshold (i.e., c[k] < threshold) are in [right+1..h]?
        // No: c is non-increasing so c[right+1] < threshold (or right+1 > h).
        // Positions in [h-s+1..right] have c == threshold.
        // Positions in [right+1..h] have c < threshold.
        // We increment all of [right+1..h] (they have c < threshold) => h - right positions.
        // Remaining increments needed: s - (h - right) positions from [left..right] with c == threshold.
        // These should be the RIGHTMOST to maintain sorted order.

        int full_inc = h - right; // positions with c < threshold (all incremented)
        int partial = s - full_inc; // positions with c == threshold to increment
        // Increment c[right+1..h] by 1
        for(int k = right + 1; k <= h; k++) c[k]++;
        // Increment rightmost 'partial' positions in [left..right] by 1
        for(int k = right - partial + 1; k <= right; k++) c[k]++;

        // Now re-sort to maintain non-increasing (the incremented positions
        // in [right-partial+1..right] now have value threshold+1, which should
        // be placed before positions with value == threshold).
        // Since we increment rightmost, and they're all equal, the array
        // remains valid: [left..right-partial] has threshold,
        // [right-partial+1..h] has at least threshold.
        // Hmm, [right-partial+1..right] now has threshold+1, and [right+1..h]
        // was incremented too (those had c < threshold, now c <= threshold).
        // This might not be sorted. Let's just sort the affected region.
        sort(c.begin() + 1, c.begin() + H + 1, greater<int>());
    }

    // Compute total inefficiency
    long long ans = 0;
    for(int k = 1; k <= H; k++){
        ans += (long long)c[k] * (c[k] - 1) / 2;
    }
    cout << ans << "\n";

    return 0;
}
