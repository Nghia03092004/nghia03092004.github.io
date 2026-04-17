#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 105: Special Subset Sums: Testing
 *
 * For each of 100 sets (sizes 7-12), check:
 *   Condition 2: min-sum of (k+1)-subset > max-sum of k-subset
 *   Condition 1: all 2^n - 1 nonempty subset sums are distinct
 *
 * Sum S(A) for all passing sets.
 * Complexity: O(100 * 2^12) = O(409600).
 */

bool isSpecialSumSet(vector<int>& a) {
    int n = a.size();
    sort(a.begin(), a.end());

    // Condition 2: for k = 1..n/2, sum of first k+1 > sum of last k
    for (int k = 1; k <= n / 2; k++) {
        int sumLow = 0, sumHigh = 0;
        for (int i = 0; i <= k; i++) sumLow += a[i];
        for (int i = 0; i < k; i++) sumHigh += a[n - 1 - i];
        if (sumLow <= sumHigh) return false;
    }

    // Condition 1: all subset sums must be distinct
    set<int> sums;
    for (int mask = 1; mask < (1 << n); mask++) {
        int s = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) s += a[i];
        }
        if (sums.count(s)) return false;
        sums.insert(s);
    }
    return true;
}

int main() {
    // Try to read from file, fall back to stdin
    ifstream fin("p105_sets.txt");
    istream& in = fin.is_open() ? fin : cin;

    int total = 0;
    int specialCount = 0;
    string line;

    while (getline(in, line)) {
        if (line.empty()) continue;
        // Remove carriage return if present
        if (!line.empty() && line.back() == '\r') line.pop_back();
        replace(line.begin(), line.end(), ',', ' ');
        istringstream iss(line);
        vector<int> a;
        int x;
        while (iss >> x) a.push_back(x);
        if (a.empty()) continue;

        if (isSpecialSumSet(a)) {
            int s = 0;
            for (int v : a) s += v;
            total += s;
            specialCount++;
        }
    }

    assert(total == 73702);
    cout << total << endl;
    return 0;
}
