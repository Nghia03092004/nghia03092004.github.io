#include <bits/stdc++.h>
using namespace std;

/*
 * IOI 2019 - Rectangles
 *
 * A rectangle (r1, c1, r2, c2) with r1 < r2, c1 < c2 is "nice" if:
 *   Row cond: for each interior col c (c1<c<c2): max(a[r1+1..r2-1][c]) < min(a[r1][c], a[r2][c])
 *   Col cond: for each interior row r (r1<r<r2): max(a[r][c1+1..c2-1]) < min(a[r][c1], a[r][c2])
 *
 * Algorithm:
 *   1. For each column c, monotone stack finds all valid (r1, r2) row-boundary pairs.
 *   2. For each row r, monotone stack finds all valid (c1, c2) col-boundary pairs.
 *   3. For each (r1,r2), group valid columns into consecutive runs.
 *      For each run and each candidate (c1,c2), check column condition via interval lookup.
 *
 * Complexity: O(nm * alpha) where alpha depends on run structure; O(nm log n) typical.
 */

long long count_rectangles(vector<vector<int>> a) {
    int n = a.size(), m = a[0].size();
    if (n < 3 || m < 3) return 0;

    // ---- Step 1: Find valid (r1, r2) pairs per column using monotone stack ----
    // For each column c (interior: 1..m-2), stack on column values (decreasing).
    // When popping pos p due to pos i (a[i][c] > a[p][c]), if stack non-empty:
    //   top = stack.back(). Pair (top, i) is valid: max(a[top+1..i-1][c]) < min(a[top][c], a[i][c]).
    // Store as sorted vector per (r1, r2).

    // Use a 2D vector: row_cols[r1][r2] = sorted list of valid interior columns.
    // But n^2 = 6.25M entries for n=2500, most empty. Use flat storage.
    // Encode (r1, r2) -> index. Or just store triples and sort.

    // For memory efficiency, use vectors of pairs per r1.
    // row_data[r1] = sorted vector of (r2, c) pairs.
    vector<vector<pair<int,int>>> row_data(n);

    for (int c = 1; c <= m - 2; c++) {
        vector<int> stk;
        for (int r = 0; r < n; r++) {
            while (!stk.empty() && a[stk.back()][c] < a[r][c]) {
                stk.pop_back();
                if (!stk.empty()) {
                    int r1 = stk.back(), r2 = r;
                    if (r2 - r1 >= 2) {
                        row_data[r1].push_back({r2, c});
                    }
                }
            }
            stk.push_back(r);
        }
    }

    // Sort each row_data[r1] by (r2, c).
    for (int r1 = 0; r1 < n; r1++) {
        sort(row_data[r1].begin(), row_data[r1].end());
    }

    // ---- Step 2: Find valid (c1, c2) pairs per row using monotone stack ----
    // For each row r (interior: 1..n-2), stack on row values (decreasing).
    // Build interval lists for consecutive valid rows per (c1, c2).

    // col_data[c1] = sorted vector of (c2, r) pairs.
    vector<vector<pair<int,int>>> col_data(m);

    for (int r = 1; r <= n - 2; r++) {
        vector<int> stk;
        for (int c = 0; c < m; c++) {
            while (!stk.empty() && a[r][stk.back()] < a[r][c]) {
                stk.pop_back();
                if (!stk.empty()) {
                    int c1 = stk.back(), c2 = c;
                    if (c2 - c1 >= 2) {
                        col_data[c1].push_back({c2, r});
                    }
                }
            }
            stk.push_back(c);
        }
    }

    // Sort each col_data[c1] by (c2, r).
    for (int c1 = 0; c1 < m; c1++) {
        sort(col_data[c1].begin(), col_data[c1].end());
    }

    // Build interval lists: for each (c1, c2), consecutive runs of valid rows.
    // Store in a hash map: key = c1*m + c2, value = vector of (start, end) intervals.
    unordered_map<int, vector<pair<int,int>>> col_intervals;

    for (int c1 = 0; c1 < m; c1++) {
        int i = 0, sz = col_data[c1].size();
        while (i < sz) {
            int c2 = col_data[c1][i].first;
            int j = i;
            // Find all entries with this c2.
            while (j < sz && col_data[c1][j].first == c2) j++;
            // Entries i..j-1 have rows col_data[c1][i..j-1].second, sorted.
            int key = c1 * m + c2;
            auto& ivals = col_intervals[key];
            int k = i;
            while (k < j) {
                int rs = col_data[c1][k].second;
                int re = rs;
                k++;
                while (k < j && col_data[c1][k].second == re + 1) {
                    re++;
                    k++;
                }
                ivals.push_back({rs, re});
            }
            i = j;
        }
    }

    // Free col_data.
    { vector<vector<pair<int,int>>>().swap(col_data); }

    // Check if all rows in [lo, hi] are valid for column pair (c1, c2).
    auto check_col = [&](int c1, int c2, int lo, int hi) -> bool {
        if (lo > hi) return true;
        int key = c1 * m + c2;
        auto it = col_intervals.find(key);
        if (it == col_intervals.end()) return false;
        auto& ivals = it->second;
        // Binary search for interval with start <= lo.
        int left = 0, right = (int)ivals.size() - 1, found = -1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (ivals[mid].first <= lo) { found = mid; left = mid + 1; }
            else right = mid - 1;
        }
        return found >= 0 && ivals[found].second >= hi;
    };

    // ---- Step 3: Count valid rectangles ----
    // For each (r1, r2), find maximal runs of consecutive valid columns.
    // For each run [l, r], enumerate (c1, c2) with interior in [l, r], check col condition.

    long long answer = 0;

    for (int r1 = 0; r1 < n; r1++) {
        auto& data = row_data[r1];
        int i = 0, sz = data.size();
        while (i < sz) {
            int r2 = data[i].first;
            int j = i;
            while (j < sz && data[j].first == r2) j++;
            // Entries i..j-1: valid columns for (r1, r2), sorted by c.

            int need = r2 - r1 - 1; // number of interior rows

            // Find maximal runs of consecutive columns.
            int k = i;
            while (k < j) {
                int l = data[k].second; // run start (leftmost valid interior col)
                int kk = k;
                while (kk + 1 < j && data[kk + 1].second == data[kk].second + 1) kk++;
                int r_end = data[kk].second; // run end (rightmost valid interior col)

                // Run of valid interior columns: [l, r_end].
                // Rectangle boundary columns: c1 in [l-1, r_end-1], c2 in [c1+2, r_end+1].
                // Interior columns c1+1..c2-1 must be subset of [l, r_end].
                // c1+1 >= l => c1 >= l-1.
                // c2-1 <= r_end => c2 <= r_end+1.
                // c1 >= 0, c2 <= m-1.

                int c1_lo = max(0, l - 1);
                int c2_hi = min(m - 1, r_end + 1);

                for (int c2 = c1_lo + 2; c2 <= c2_hi; c2++) {
                    // c1 ranges from c1_lo to min(c2-2, r_end-1).
                    // But also c1+1 >= l: c1 >= l-1 = c1_lo.
                    // And c2-1 <= r_end: already ensured.
                    int c1_max = min(c2 - 2, r_end - 1);
                    // Also need c1+1 <= r_end: c1 <= r_end - 1. Already in c1_max.
                    // And c2-1 >= l: c2 >= l + 1.
                    if (c2 < l + 1) continue;

                    for (int c1 = c1_lo; c1 <= c1_max; c1++) {
                        if (check_col(c1, c2, r1 + 1, r2 - 1)) {
                            answer++;
                        }
                    }
                }

                k = kk + 1;
            }

            i = j;
        }
    }

    return answer;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    vector<vector<int>> a(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &a[i][j]);
    printf("%lld\n", count_rectangles(a));
    return 0;
}
