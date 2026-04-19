#include <bits/stdc++.h>
using namespace std;

// IOI 2020 - Mushrooms
// Determine the count of type-A mushrooms using limited machine queries.
// Strategy: grow known pools, then batch-classify using interleaving.
// Mushroom 0 is known type A. use_machine returns number of adjacent
// different-type pairs in the sequence.

int use_machine(vector<int> x); // provided by grader

int count_mushrooms(int n) {
    if (n == 1) return 1;
    if (n == 2) return 1 + (1 - use_machine({0, 1}));

    // known[0] = type A indices, known[1] = type B indices
    vector<int> known[2];
    known[0].push_back(0);

    int countA = 1;
    int idx = 1;

    while (idx < n) {
        // Use the larger pool for batching
        int t = ((int)known[1].size() > (int)known[0].size()) ? 1 : 0;
        int k = (int)known[t].size();

        if (k < 2 || n - idx < 2) {
            // Identify one mushroom at a time
            int r = use_machine({known[t][0], idx});
            int type_idx = (r == 0) ? t : (1 - t);
            if (type_idx == 0) countA++;
            known[type_idx].push_back(idx);
            idx++;
        } else {
            // Batch: interleave known[t] mushrooms with unknowns
            int batch = min(k, n - idx);
            vector<int> seq;
            for (int i = 0; i < batch; i++) {
                seq.push_back(known[t][i]);
                seq.push_back(idx + i);
            }
            int r = use_machine(seq);

            // Decode transitions:
            // Each unknown (except last) is flanked by two known mushrooms,
            // contributing 2 to r if different from type t, 0 if same.
            // Last unknown contributes 1 if different, 0 if same.
            int last_diff = r % 2;
            int rest_diff = (r - last_diff) / 2;
            int diff_count = rest_diff + last_diff;
            int same_count = batch - diff_count;

            if (t == 0) {
                countA += same_count;  // same as type A
            } else {
                countA += diff_count;  // different from type B = type A
            }

            // We know the last unknown's type; add it to the pool
            int last_type = last_diff ? (1 - t) : t;
            known[last_type].push_back(idx + batch - 1);

            idx += batch;
        }
    }

    return countA;
}

int main() {
    int n;
    scanf("%d", &n);
    printf("%d\n", count_mushrooms(n));
    return 0;
}
