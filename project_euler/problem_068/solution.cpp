#include <bits/stdc++.h>
using namespace std;

int main() {
    // Magic 5-gon ring: inner = {1,2,3,4,5}, outer = {6,7,8,9,10}, S = 14.
    // Enumerate permutations of inner ring; outer[k] = 14 - inner[k] - inner[(k+1)%5].
    // Valid iff outer is a permutation of {6,7,8,9,10}.

    vector<int> inner = {1, 2, 3, 4, 5};
    string max_str = "";

    do {
        vector<int> outer(5);
        bool valid = true;
        set<int> outer_set;

        for (int k = 0; k < 5; k++) {
            outer[k] = 14 - inner[k] - inner[(k + 1) % 5];
            if (outer[k] < 6 || outer[k] > 10) { valid = false; break; }
            outer_set.insert(outer[k]);
        }

        if (!valid || outer_set.size() != 5) continue;

        // Start from the line with the smallest outer node
        int start = 0;
        for (int k = 1; k < 5; k++) {
            if (outer[k] < outer[start]) start = k;
        }

        string s = "";
        for (int k = 0; k < 5; k++) {
            int idx = (start + k) % 5;
            s += to_string(outer[idx]);
            s += to_string(inner[idx]);
            s += to_string(inner[(idx + 1) % 5]);
        }

        if (s.length() == 16 && s > max_str) {
            max_str = s;
        }

    } while (next_permutation(inner.begin(), inner.end()));

    cout << max_str << endl;

    return 0;
}
