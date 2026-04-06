#include <bits/stdc++.h>
using namespace std;

int main() {
    // Build list of all possible dart scores with labels
    // Each dart is a (score, type) pair where type is used for canonical ordering
    // We use index for canonical ordering of non-final darts

    struct Dart {
        int score;
        // For ordering: singles first, then doubles, then trebles
    };

    vector<int> all_darts;   // all 62 dart scores (with distinct identity)
    vector<int> doubles;     // double scores only

    // Singles: S1..S20, S25
    for (int i = 1; i <= 20; i++) all_darts.push_back(i);
    all_darts.push_back(25);

    // Doubles: D1..D20, D25
    for (int i = 1; i <= 20; i++) { all_darts.push_back(2*i); doubles.push_back(2*i); }
    all_darts.push_back(50); doubles.push_back(50);

    // Trebles: T1..T20
    for (int i = 1; i <= 20; i++) all_darts.push_back(3*i);

    int n = all_darts.size(); // 62
    int count = 0;

    // 1-dart checkouts: just a double < 100
    for (int d : doubles) {
        if (d < 100) count++;
    }

    // 2-dart checkouts: one non-final + one double, total < 100
    for (int i = 0; i < n; i++) {
        for (int d : doubles) {
            if (all_darts[i] + d < 100) count++;
        }
    }

    // 3-dart checkouts: two non-final (unordered, i <= j) + one double, total < 100
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            for (int d : doubles) {
                if (all_darts[i] + all_darts[j] + d < 100) count++;
            }
        }
    }

    cout << count << endl;
    return 0;
}
