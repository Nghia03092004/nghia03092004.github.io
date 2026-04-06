#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 253: Tidying Up
 *
 * Expected maximum number of segments when randomly filling a caterpillar of length 40.
 *
 * Approach: DP on gap configurations.
 *
 * State: a multiset of gap sizes (non-negative integers summing to remaining empty cells),
 *        plus the current number of segments and the max segments seen.
 *
 * Actually, the current number of segments s determines the number of gaps:
 *   - If s segments occupy the caterpillar, there are:
 *     - 0 or 1 gaps at the left end
 *     - 0 or 1 gaps at the right end
 *     - s-1 internal gaps (between consecutive segments)
 *   - More precisely: with s segments in a row of n cells, having placed k cells:
 *     The gaps form a composition. Let's classify:
 *     - "end gaps" (at the two ends): can be 0 or more
 *     - "internal gaps" (between segments): must be >= 1 each
 *
 * Configuration: The gaps between segments. If there are s segments, the cells look like:
 *   [gap_0] SEG_1 [gap_1] SEG_2 [gap_2] ... SEG_s [gap_s]
 *   where gap_0 and gap_s are end gaps (>= 0), gap_1 ... gap_{s-1} are internal gaps (>= 1).
 *   Each gap has a certain size.
 *
 * When we fill a cell (chosen uniformly at random from all empty cells):
 *   If the cell is in gap_0 (end gap, size g0):
 *     - If at position adjacent to SEG_1: extends SEG_1, gap_0 shrinks by 1.
 *       If g0 was 1: gap_0 disappears. Same s.
 *       If g0 > 1: gap_0 becomes (g0-1). Same s.
 *       Actually: the cell is at the inner end of gap_0.
 *       This extends SEG_1. s stays the same.
 *     - If at any other position in gap_0 (not adjacent to SEG_1): creates a new segment.
 *       gap_0 splits into an end gap and an internal gap (of size >= 1).
 *       s increases by 1.
 *     - If at the outer end of gap_0 (the boundary): creates a new segment if g0 > 1
 *       (splits gap_0 into [0, new_seg, remaining gap], which means end gap 0 and internal gap remaining-1).
 *       Wait, no: if we place at the leftmost position of gap_0:
 *         It's a new isolated segment (unless g0 = 1, in which case it joins SEG_1).
 *         If g0 = 1: placed at the only position, which is adjacent to SEG_1. Extends SEG_1. s unchanged.
 *         If g0 >= 2: placing at position 0 (leftmost of gap): new segment.
 *           New configuration: [0] NEW_SEG [gap_0 - 2 internal] SEG_1 ...
 *           Wait: gap_0 has g0 cells. Placing at leftmost: the cell to the right is still empty (if g0 >= 2).
 *           So the new segment is isolated, end gap on the left is 0, internal gap is g0-2 (if g0 >= 2)
 *           or the new segment merges with SEG_1 if g0 = 1... wait if g0 = 2: placing at leftmost,
 *           the rightmost of gap_0 (adjacent to SEG_1) is still empty. So gap becomes [0] NEW [gap=0 internal]...
 *           no: gap_0 has 2 cells. Place at left cell. New config: [0] NEW [0 internal] SEG_1... but internal gap of 0
 *           is not allowed (segments would be adjacent). Hmm.
 *           Actually an internal gap of size 0 means the segments are adjacent, i.e., they should be one segment.
 *           So gap_0 = 2, place leftmost: new segment at pos 0, empty pos 1, then SEG_1 at pos 2.
 *           So: end gap = 0, NEW_SEG (pos 0), internal gap = 1 (pos 1), SEG_1 starts at pos 2.
 *           Wait, the gap_0 was cells at positions 0..g0-1. SEG_1 starts at position g0.
 *           Placing at position 0: cell 0 is filled. Cell 1 to g0-1 are empty. Cell g0 is filled (SEG_1).
 *           So: end gap = 0 (nothing left of cell 0), filled cell 0, gap of (g0-2) cells (positions 1..g0-2),
 *           wait no: positions 1..g0-1 are empty, that's g0-1 cells. Then cell g0 is filled (start of SEG_1).
 *           So internal gap = g0 - 1. s increases by 1 (new segment added).
 *
 *     OK this is getting complicated. Let me think of it more simply.
 *
 * SIMPLER MODEL: represent the state as a sorted tuple of gap sizes, distinguishing
 * "end gaps" from "internal gaps". An end gap has one side open (touches the boundary),
 * an internal gap has both sides adjacent to segments.
 *
 * For a gap of size g:
 *   - End gap (one side is boundary, other side is a segment):
 *     Placing a cell in this gap at position i (0 = boundary side, g-1 = segment side):
 *     - i = g-1 (adjacent to segment): extends the segment. Gap becomes end gap of size g-1.
 *       If g-1 = 0: gap disappears. s unchanged.
 *     - 0 <= i < g-1: new segment. Gap splits into:
 *       End gap of size i, and internal gap of size g-1-i.
 *       Wait: positions 0..g-1 in the gap. Place at position i.
 *       Left side (0..i-1): end gap of size i.
 *       Right side (i+1..g-1): internal gap of size g-1-i-1+1 = g-i-1. Hmm.
 *       Actually: the filled cell at position i creates a new segment.
 *       Left: positions 0..i-1 = i cells, this is an end gap of size i.
 *       Right: positions i+1..g-1 = g-1-i cells. This is adjacent to both the new segment
 *              and the existing segment on the right. So it's an internal gap of size g-1-i.
 *       Wait, g-1-i could be 0 if i = g-1, which we handled above.
 *       For i < g-1: internal gap = g-1-i >= 1. And end gap = i >= 0.
 *       s increases by 1.
 *     So for end gap of size g: 1 cell placement extends (no change to s), g-1 create new segment.
 *
 *   - Internal gap (both sides adjacent to segments, size g >= 1):
 *     Placing at position i (0 = adjacent to left segment, g-1 = adjacent to right segment):
 *     - i = 0: extends left segment. Internal gap becomes internal gap of size g-1.
 *       If g-1 = 0: the two segments merge! s decreases by 1.
 *       If g-1 >= 1: s unchanged.
 *     - i = g-1: extends right segment. Same as above by symmetry.
 *       If g = 1: same cell as i=0 (g-1=0). Merges. s decreases by 1.
 *       If g >= 2: internal gap becomes g-1. s unchanged.
 *     - 0 < i < g-1 (only if g >= 3): new segment. Gap splits into:
 *       Internal gap of size i (left side), internal gap of size g-1-i (right side).
 *       s increases by 1.
 *     So for internal gap of size g:
 *       If g = 1: 1 cell, merges. s decreases by 1.
 *       If g = 2: 2 cells, both extend. s unchanged.
 *       If g >= 3: 2 cells extend (s unchanged), g-2 cells create new segment (s+1).
 *
 * This model allows us to do DP on gap configurations.
 *
 * State: sorted tuple of (end_gap_sizes, internal_gap_sizes), plus max_segments.
 * But the number of distinct gap configurations can be large.
 *
 * Alternative: use integer partitions. The state is:
 *   - Number of end gaps (0, 1, or 2) and their sizes.
 *   - Multiset of internal gap sizes (each >= 1).
 *   - Current number of segments s.
 *   - Max segments seen so far.
 *
 * With n = 40: max segments = 20. Gap sizes sum to 40 - (cells placed).
 * The number of distinct configurations is bounded by the number of partitions of remaining cells
 * times 3 (for end gap count) times 20 (for max). This could be manageable.
 *
 * Actually, let me think about this more carefully.
 * When we have s segments, we have:
 *   - 0, 1, or 2 end gaps (le = number of end gaps at the left, re at the right, so 0-2 total end gaps).
 *   Actually: the caterpillar has a left end and right end. If the leftmost cell is empty, there's
 *   a left end gap. If the rightmost cell is empty, there's a right end gap. Otherwise not.
 *   So there are 0, 1, or 2 end gaps.
 *   - s - 1 + (2 - num_end_gaps) internal gaps? No.
 *   With s segments: total gaps = s + 1 (between and at ends). But some gaps can have size 0.
 *   If an end gap has size 0, it doesn't really exist.
 *   Gaps with size 0 at the ends: segment touches the boundary.
 *   Gaps with size 0 internally: impossible (would mean segments are adjacent = one segment).
 *
 *   Let ne = number of end gaps with size > 0 (0, 1, or 2).
 *   Internal gaps: s - 1 gaps, each of size >= 1. Plus ne end gaps of size >= 1.
 *   Total gaps: (s - 1) + ne, sizes summing to n - k (remaining empty cells).
 *
 * For the DP, the state is: multiset of gap sizes, classified as end/internal, plus max_s.
 * But we can simplify: the behavior of an end gap of size g is different from an internal gap.
 * An end gap of size g: 1 extension, g-1 new segments.
 *   Of the g-1 new segments: placing at position i (0 <= i < g-1) from boundary side:
 *     end gap of size i, internal gap of size g-1-i.
 * An internal gap of size g >= 1:
 *   g = 1: merge.
 *   g >= 2: 2 extensions. g-2 new segments.
 *     Of the g-2 new segments: placing at position i (1 <= i <= g-2):
 *       internal gap of size i, internal gap of size g-1-i.

 * To make the DP tractable, represent the state as:
 *   (ne, multiset_of_end_gap_sizes, multiset_of_internal_gap_sizes, max_seg)
 * and track probability.
 *
 * Use a map from state to probability. The state can be encoded as a sorted vector.
 *
 * The number of states: bounded by partitions of (n-k) into parts, with classification.
 * For n=40, the number of partitions of 40 is 37338, but with classification and max_seg,
 * the total states might be ~10^6 or so.
 *
 * Let me encode the state as: (max_seg, ne, sorted_end_gaps, sorted_internal_gaps).
 * Use a map<tuple, double>.
 *
 * This should be feasible for n=40.
 */

int main(){
    int n = 40;

    // State: max_seg, end_gaps (sorted), internal_gaps (sorted)
    // ne = end_gaps.size() (0, 1, or 2)
    // Segments s = internal_gaps.size() + 1
    // (if ne = 0: segments touch both ends. If ne = 1: one end has a gap. If ne = 2: both ends have gaps.)
    // Wait: ne end gaps and (s-1) internal gaps.
    // Actually: segments s, and the number of "open" ends is 2 - ne_touching.
    // Let me re-derive: with s segments in a row of n cells:
    //   Layout: [g0] SEG1 [g1] SEG2 [g2] ... SEGs [gs]
    //   g0 = left end gap (>= 0), gs = right end gap (>= 0), g1..g_{s-1} = internal gaps (>= 1).
    //   Total gaps: s+1 (g0 through gs), but g0 and gs can be 0.
    //   Cells placed: n - (g0 + g1 + ... + gs). Also cells placed in segments.
    //   Actually segments have lengths >= 1 summing to k (cells placed).
    //   Hmm, we don't track segment lengths, just gap structure.
    //   That's fine because we only care about which gap a new cell goes into.
    //
    // Better encoding: the state is (list_of_end_gaps, list_of_internal_gaps, max_seg).
    // end_gaps has 0, 1, or 2 elements (each >= 1). Can be 0 elements if segments touch both ends.
    // internal_gaps has >= 0 elements (each >= 1).
    // s = segments = 1 + internal_gaps.size()
    //   Hmm, is this right? With s segments: internal gaps = s-1.
    //   But the relationship between segments and gaps:
    //     ne_end_gaps + (s-1) internal gaps. Plus the two "zero gaps" at ends if segment touches boundary.
    //     Actually: layout always has g0, g1, ..., gs. g0 and gs >= 0, g1..g_{s-1} >= 1.
    //     Number of "non-zero end gaps" ne = (g0 > 0) + (gs > 0).
    //     Number of internal gaps = s - 1 (each >= 1).
    //     State: (sorted end gaps > 0, sorted internal gaps, max_seg). Plus ne implicitly.
    //     Oh and we need to distinguish left vs right end gaps? No, by symmetry we can sort them.
    //     But actually, when placing a cell in an end gap, the behavior is the same regardless
    //     of left vs right (it's symmetric). So we can just track the multiset.

    //     s = 1 + internal_gaps.size()... wait.
    //     With layout: [g0] SEG1 [g1] ... SEGs [gs]:
    //       s segments, s-1 internal gaps (g1..g_{s-1}), 2 end "slots" (g0, gs).
    //       Let end_gaps_list = list of g0, gs that are > 0.
    //       internal_gaps_list = [g1, ..., g_{s-1}], each >= 1.
    //     So s - 1 = internal_gaps_list.size(), hence s = internal_gaps_list.size() + 1.
    //     Yes.

    // Number of end gaps ne = 0, 1, or 2.
    // Remaining empty cells: sum(end_gaps) + sum(internal_gaps).

    // INITIAL STATE: 0 segments, but that doesn't fit the model.
    // Before any cell is placed: 0 segments, the entire row is a "gap".
    // It's a single end-to-end gap of size n. No segments.
    // Let's handle the first placement:
    //   Place in position i (0-indexed). This creates 1 segment.
    //   Left end gap = i, right end gap = n-1-i. Internal gaps: none.
    //   s = 1, max_seg = 1.
    //   Each position equally likely (1/n each).

    // State encoding: (max_seg, sorted_end_gaps, sorted_internal_gaps)
    // Use map<tuple<int, vector<int>, vector<int>>, double> but vector in tuple is bad for map.
    // Encode as map<vector<int>, double> where the vector is: [max_seg, ne, eg1, eg2, ni, ig1, ig2, ...]
    // Or more simply: [max_seg, ne, end_gap_sizes_sorted, -1 (separator), internal_gap_sizes_sorted]

    // Use a string or vector<int> as key.
    typedef vector<int> State;
    map<State, double> dp;

    // Initial: after placing first cell.
    // By symmetry, position i and position n-1-i give the same gap structure.
    for (int i = 0; i < n; i++) {
        int g0 = i, gs = n - 1 - i;
        // s = 1, max_seg = 1, 0 internal gaps.
        vector<int> eg;
        if (g0 > 0) eg.push_back(g0);
        if (gs > 0) eg.push_back(gs);
        sort(eg.begin(), eg.end());
        State state;
        state.push_back(1); // max_seg
        state.push_back(eg.size()); // ne
        for (int x : eg) state.push_back(x);
        state.push_back(-1); // separator
        // no internal gaps
        dp[state] += 1.0 / n;
    }

    // Process remaining n-1 steps
    for (int step = 1; step < n; step++) {
        // At this point, step cells have been placed. Place one more.
        // Total empty cells: n - step.
        map<State, double> new_dp;

        for (auto& [state, prob] : dp) {
            // Parse state
            int max_seg = state[0];
            int ne = state[1];
            vector<int> eg, ig;
            int idx = 2;
            for (int i = 0; i < ne; i++) eg.push_back(state[idx++]);
            idx++; // skip separator
            while (idx < (int)state.size()) ig.push_back(state[idx++]);

            int s = 1 + (int)ig.size();
            int total_empty = 0;
            for (int x : eg) total_empty += x;
            for (int x : ig) total_empty += x;

            if (total_empty == 0) {
                // No more cells to place. Just carry forward.
                new_dp[state] += prob;
                continue;
            }

            // For each gap, enumerate possible placements.
            // Total empty cells = total_empty. Each cell equally likely.

            // Process end gaps
            for (int gi = 0; gi < ne; gi++) {
                int g = eg[gi];
                // g cells in this end gap.

                // Placement: extend existing segment (1 way).
                {
                    // Place at the cell adjacent to the segment. Gap shrinks by 1.
                    double p = prob * 1.0 / total_empty;
                    vector<int> new_eg = eg;
                    vector<int> new_ig = ig;
                    if (g - 1 > 0) {
                        new_eg[gi] = g - 1;
                    } else {
                        new_eg.erase(new_eg.begin() + gi);
                    }
                    sort(new_eg.begin(), new_eg.end());
                    int new_max = max_seg;
                    // s unchanged
                    State ns;
                    ns.push_back(new_max);
                    ns.push_back(new_eg.size());
                    for (int x : new_eg) ns.push_back(x);
                    ns.push_back(-1);
                    sort(new_ig.begin(), new_ig.end());
                    for (int x : new_ig) ns.push_back(x);
                    new_dp[ns] += p;
                }

                // Placement: create new segment (g - 1 ways).
                // Place at position i (0 <= i <= g-2) from boundary side.
                // Creates end gap of size i and internal gap of size g-1-i-1 = g-2-i.
                // Wait: placing at position i creates:
                //   End gap (boundary side): size i
                //   New segment
                //   Internal gap (segment side): size g - 1 - i - 1? Let me recount.
                //   Gap has g cells: positions 0 (boundary) to g-1 (adjacent to segment).
                //   Place at position i (0 <= i <= g-2, since i=g-1 is the extension case).
                //   Left of placed cell: positions 0..i-1 = i cells. End gap of size i.
                //   Right of placed cell: positions i+1..g-1 = g-1-i cells. This is adjacent to
                //     the new segment on the left and the existing segment on the right.
                //     So it's an internal gap of size g-1-i.
                //   s increases by 1.
                for (int i = 0; i <= g - 2; i++) {
                    double p = prob * 1.0 / total_empty;
                    vector<int> new_eg = eg;
                    vector<int> new_ig = ig;

                    int end_size = i;
                    int int_size = g - 1 - i;

                    // Remove old end gap
                    new_eg.erase(new_eg.begin() + gi);
                    // Add new end gap if > 0
                    if (end_size > 0) new_eg.push_back(end_size);
                    // Add new internal gap (always >= 1 since i <= g-2 means g-1-i >= 1)
                    new_ig.push_back(int_size);

                    sort(new_eg.begin(), new_eg.end());
                    sort(new_ig.begin(), new_ig.end());

                    int new_s = s + 1;
                    int new_max = max(max_seg, new_s);

                    State ns;
                    ns.push_back(new_max);
                    ns.push_back(new_eg.size());
                    for (int x : new_eg) ns.push_back(x);
                    ns.push_back(-1);
                    for (int x : new_ig) ns.push_back(x);
                    new_dp[ns] += p;
                }
            }

            // Process internal gaps
            for (int gi = 0; gi < (int)ig.size(); gi++) {
                int g = ig[gi];

                if (g == 1) {
                    // Merge: 1 way. s decreases by 1.
                    double p = prob * 1.0 / total_empty;
                    vector<int> new_eg = eg;
                    vector<int> new_ig = ig;
                    new_ig.erase(new_ig.begin() + gi);
                    sort(new_ig.begin(), new_ig.end());
                    int new_s = s - 1;
                    int new_max = max_seg; // Can't increase.

                    State ns;
                    ns.push_back(new_max);
                    ns.push_back(new_eg.size());
                    for (int x : new_eg) ns.push_back(x);
                    ns.push_back(-1);
                    for (int x : new_ig) ns.push_back(x);
                    new_dp[ns] += p;
                } else {
                    // g >= 2.
                    // Extensions: 2 ways (left end or right end). Gap shrinks to g-1.
                    {
                        double p = prob * 2.0 / total_empty;
                        vector<int> new_eg = eg;
                        vector<int> new_ig = ig;
                        if (g - 1 >= 1) {
                            new_ig[gi] = g - 1;
                        } else {
                            // g-1 = 0: merge. But g >= 2 so g-1 >= 1.
                            new_ig.erase(new_ig.begin() + gi);
                        }
                        sort(new_ig.begin(), new_ig.end());
                        // Hmm wait: if g = 2, gap becomes 1. That's still valid internal gap.
                        // s unchanged.
                        int new_max = max_seg;
                        State ns;
                        ns.push_back(new_max);
                        ns.push_back(new_eg.size());
                        for (int x : new_eg) ns.push_back(x);
                        ns.push_back(-1);
                        for (int x : new_ig) ns.push_back(x);
                        new_dp[ns] += p;
                    }

                    // New segments: g - 2 ways.
                    // Place at position i (1 <= i <= g-2, 0-indexed from left segment side).
                    // Splits into internal gap of size i and internal gap of size g-1-i.
                    if (g >= 3) {
                        for (int i = 1; i <= g - 2; i++) {
                            double p = prob * 1.0 / total_empty;
                            vector<int> new_eg = eg;
                            vector<int> new_ig = ig;

                            int left_size = i;
                            int right_size = g - 1 - i;

                            new_ig.erase(new_ig.begin() + gi);
                            new_ig.push_back(left_size);
                            new_ig.push_back(right_size);
                            sort(new_ig.begin(), new_ig.end());

                            int new_s = s + 1;
                            int new_max = max(max_seg, new_s);

                            State ns;
                            ns.push_back(new_max);
                            ns.push_back(new_eg.size());
                            for (int x : new_eg) ns.push_back(x);
                            ns.push_back(-1);
                            for (int x : new_ig) ns.push_back(x);
                            new_dp[ns] += p;
                        }
                    }
                }
            }
        }

        dp = new_dp;
    }

    // Compute expected max segments
    double expected = 0;
    for (auto& [state, prob] : dp) {
        expected += state[0] * prob;
    }

    printf("%.6f\n", expected);
    return 0;
}
