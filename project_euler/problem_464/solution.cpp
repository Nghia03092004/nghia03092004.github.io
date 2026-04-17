#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 464: Mobius Function and Intervals
 *
 * Count pairs (a,b) with 1 <= a <= b <= n where the counts of
 * mu=+1 and mu=-1 in [a,b] are within ratio 100:99.
 *
 * Reformulation: 2D dominance counting.
 * X(i) = 100*prefP(i) - 99*prefN(i)
 * Y(i) = 100*prefN(i) - 99*prefP(i)
 * Count pairs (j,b), j < b, X(b)>=X(j), Y(b)>=Y(j).
 *
 * Algorithm: CDQ divide-and-conquer with Fenwick tree.
 * Time: O(N log^2 N). Space: O(N).
 */

const int MAXN = 20000001;

int mu_arr[MAXN];
bool is_prime[MAXN];
int primes[1500000];
int pcnt = 0;

int X_arr[MAXN], Y_arr[MAXN];
int idx_arr[MAXN], tmp[MAXN];
int Y_comp[MAXN];
long long answer = 0;

// Fenwick tree
int bit[MAXN];
int bit_size;

void bit_update(int i, int v) {
    for (i++; i <= bit_size; i += i & (-i))
        bit[i] += v;
}

int bit_query(int i) {
    int s = 0;
    for (i++; i > 0; i -= i & (-i))
        s += bit[i];
    return s;
}

void bit_clear(int i) {
    for (i++; i <= bit_size; i += i & (-i))
        bit[i] = 0;
}

void mobius_sieve(int n) {
    memset(is_prime, true, sizeof(is_prime));
    mu_arr[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes[pcnt++] = i;
            mu_arr[i] = -1;
        }
        for (int j = 0; j < pcnt && (long long)i * primes[j] <= n; j++) {
            is_prime[i * primes[j]] = false;
            if (i % primes[j] == 0) {
                mu_arr[i * primes[j]] = 0;
                break;
            }
            mu_arr[i * primes[j]] = -mu_arr[i];
        }
    }
}

// CDQ divide and conquer
// arr[lo..hi) are indices, to be merge-sorted by X
void cdq(int lo, int hi) {
    if (hi - lo <= 1) return;
    int mid = (lo + hi) / 2;
    cdq(lo, mid);
    cdq(mid, hi);

    // Count cross: j in [lo,mid), b in [mid,hi), X[b]>=X[j], Y[b]>=Y[j]
    // Both halves are sorted by X after recursion
    int li = lo;
    vector<int> inserted;
    for (int ri = mid; ri < hi; ri++) {
        int b = idx_arr[ri];
        while (li < mid && X_arr[idx_arr[li]] <= X_arr[b]) {
            int j = idx_arr[li];
            bit_update(Y_comp[j], 1);
            inserted.push_back(Y_comp[j]);
            li++;
        }
        answer += bit_query(Y_comp[b]);
    }
    for (int yc : inserted) bit_clear(yc);

    // Merge sort by X
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (X_arr[idx_arr[i]] <= X_arr[idx_arr[j]])
            tmp[k++] = idx_arr[i++];
        else
            tmp[k++] = idx_arr[j++];
    }
    while (i < mid) tmp[k++] = idx_arr[i++];
    while (j < hi) tmp[k++] = idx_arr[j++];
    for (int t = lo; t < hi; t++) idx_arr[t] = tmp[t];
}

int main() {
    int N = 20000000;
    mobius_sieve(N);

    // Prefix sums
    int prefP = 0, prefN = 0;
    X_arr[0] = 0;
    Y_arr[0] = 0;
    for (int i = 1; i <= N; i++) {
        if (mu_arr[i] == 1) prefP++;
        else if (mu_arr[i] == -1) prefN++;
        X_arr[i] = 100 * prefP - 99 * prefN;
        Y_arr[i] = 100 * prefN - 99 * prefP;
    }

    // Coordinate compress Y
    vector<int> Y_vals(Y_arr, Y_arr + N + 1);
    sort(Y_vals.begin(), Y_vals.end());
    Y_vals.erase(unique(Y_vals.begin(), Y_vals.end()), Y_vals.end());
    bit_size = Y_vals.size();
    for (int i = 0; i <= N; i++) {
        Y_comp[i] = lower_bound(Y_vals.begin(), Y_vals.end(), Y_arr[i]) - Y_vals.begin();
    }

    // CDQ
    for (int i = 0; i <= N; i++) idx_arr[i] = i;
    memset(bit, 0, sizeof(bit));
    cdq(0, N + 1);

    cout << answer << endl;
    return 0;
}
