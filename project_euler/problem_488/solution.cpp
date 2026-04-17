#include <bits/stdc++.h>
using namespace std;

int main() {
    const int MAX_H = 100;
    vector<int> G(MAX_H + 1, 0);

    for (int h = 2; h <= MAX_H; h++) {
        int low = (h + 1) / 2; // ceil(h/2)
        int high = h - 1;
        set<int> reachable;
        for (int j = low; j <= high; j++) {
            reachable.insert(G[j]);
        }
        int mex = 0;
        while (reachable.count(mex)) mex++;
        G[h] = mex;
    }

    // Print Grundy values
    cout << "Grundy values for Unbalanced Nim:" << endl;
    for (int h = 0; h <= 30; h++) {
        cout << "G(" << h << ") = " << G[h] << endl;
    }

    // Count two-heap P-positions (h1, h2 in [0, MAX_H])
    int p_count = 0;
    for (int h1 = 0; h1 <= MAX_H; h1++) {
        for (int h2 = 0; h2 <= MAX_H; h2++) {
            if ((G[h1] ^ G[h2]) == 0) p_count++;
        }
    }
    cout << "\nTwo-heap P-positions (0.." << MAX_H << "): " << p_count << endl;

    return 0;
}
