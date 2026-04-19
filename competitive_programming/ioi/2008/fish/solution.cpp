#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

long long modinv(long long a, long long mod) {
    return power(a, mod - 2, mod);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    vector<long long> sz(N);
    vector<int> sp(N); // species (0-indexed)
    for (int i = 0; i < N; i++) {
        cin >> sz[i] >> sp[i];
        sp[i]--;
    }

    // Sort by size
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return sz[a] < sz[b];
    });

    // For each fish i (in sorted order), find the position of the first fish
    // of the same species with size > 2 * sz[i].
    // This determines which fish of the same species can coexist with i.

    // Group fish by species (sorted by size within each species)
    vector<vector<int>> bySpecies(K);
    for (int i = 0; i < N; i++) {
        bySpecies[sp[order[i]]].push_back(i); // position in sorted order
    }

    // For each fish at sorted position i, find how many fish of the same
    // species have size in (sz[order[i]], 2 * sz[order[i]]]
    // (these can coexist with fish i as part of species sp[order[i]]).

    // Count valid subsets:
    // A valid subset S has the property that for each species t,
    // max_size(S, t) <= 2 * min_size(S, t).

    // Approach: iterate over all possible "minimum fish" m.
    // For a given minimum fish m with size s_m and species t_m:
    // - All fish in S must have size >= s_m (m is the global minimum).
    // - For species t_m: fish in S must have size in [s_m, 2*s_m].
    // - For other species t: fish in S can have size in [s_m, 2*min_t]
    //   where min_t is the smallest fish of species t in S.
    //   But min_t >= s_m, so the constraint is within species.
    // Actually, the constraint is per-species: for each species,
    // the ratio max/min <= 2. The constraints are INDEPENDENT per species.

    // So the total number of valid subsets = product over species of
    // (number of valid subsets of that species) - but that overcounts
    // subsets where multiple species have fish.

    // Actually, since the constraints are independent per species:
    // Total valid subsets (including empty) = product over species of
    // (number of valid subsets of that species, including the empty subset for that species).

    // For species t with fish sizes s_1 < s_2 < ... < s_m:
    // A valid subset of species t is any subset {s_i, s_{i+1}, ..., s_j}
    // where s_j <= 2 * s_i. (Since the sizes are sorted and distinct,
    // and a valid subset can be non-contiguous as long as max <= 2*min.)

    // Wait: actually ANY subset with max <= 2*min is valid, not just contiguous.
    // So for species t, count subsets of {s_1, ..., s_m} where max/min <= 2.

    // For each possible minimum s_i, count subsets where min = s_i and
    // all other elements are in (s_i, 2*s_i]. Let f(i) = number of elements
    // in (s_i, 2*s_i] (not counting s_i itself). Then the number of subsets
    // with min = s_i is 2^f(i) (choose any subset of the f(i) elements,
    // plus s_i is included). But we also need to ensure s_i is actually the min,
    // meaning no element < s_i is included.

    // Since we're considering subsets of species t only, and s_i is the
    // i-th smallest:
    // Subsets with minimum exactly s_i and max <= 2*s_i:
    //   - Include s_i.
    //   - Include any subset of {s_{i+1}, ..., s_{j}} where s_j <= 2*s_i.
    //   - Exclude s_1, ..., s_{i-1} (they're smaller, not relevant since
    //     we're choosing from the species's fish).
    // Actually, since we're partitioning by minimum, the count for
    // species t = sum over i of 2^{f(i)} where f(i) = |{j > i : s_j <= 2*s_i}|.
    // Plus 1 for the empty subset.

    // Total answer = product over species of (1 + sum_i 2^f(i)) - 1
    // (subtract 1 for the all-empty case, which is the empty subset overall)

    long long ans = 1;

    for (int t = 0; t < K; t++) {
        vector<long long> sizes;
        for (int idx : bySpecies[t]) {
            sizes.push_back(sz[order[idx]]);
        }
        sort(sizes.begin(), sizes.end());

        int m = sizes.size();
        long long speciesCount = 1; // 1 for empty subset

        int j = 0;
        for (int i = 0; i < m; i++) {
            // Find how many elements in (sizes[i], 2*sizes[i]]
            // = elements at indices i+1, ..., j-1 where sizes[j-1] <= 2*sizes[i]
            while (j < m && sizes[j] <= 2 * sizes[i]) j++;
            int f = j - i - 1; // elements in (sizes[i], 2*sizes[i]]
            speciesCount = (speciesCount + power(2, f, MOD)) % MOD;
        }

        ans = ans * speciesCount % MOD;
    }

    ans = (ans - 1 + MOD) % MOD; // subtract empty subset

    cout << ans << "\n";
    return 0;
}
