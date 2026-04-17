#include <bits/stdc++.h>
using namespace std;

// E(n) = (n-1)(n+4)/12

int simulate(vector<int>& arr) {
    int moves = 0;
    int n = arr.size();
    while (true) {
        int found = -1;
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                found = i;
                break;
            }
        }
        if (found == -1) break;

        int elem = arr[found];
        arr.erase(arr.begin() + found);
        bool inserted = false;
        for (int j = 0; j < (int)arr.size(); j++) {
            if (elem <= arr[j]) {
                arr.insert(arr.begin() + j, elem);
                inserted = true;
                break;
            }
        }
        if (!inserted) arr.push_back(elem);
        moves++;
    }
    return moves;
}

int main() {
    // Print closed-form values: E(n) = (n-1)(n+4)/12
    cout << "E(n) = (n-1)(n+4)/12:" << endl;
    for (int n = 1; n <= 20; n++) {
        long long num = (long long)(n - 1) * (n + 4);
        if (num % 12 == 0)
            cout << "  E(" << n << ") = " << num / 12 << endl;
        else
            cout << "  E(" << n << ") = " << num << "/12" << endl;
    }

    // Verify by exhaustive enumeration for small n
    cout << "\nExhaustive verification:" << endl;
    for (int n = 1; n <= 8; n++) {
        vector<int> perm(n);
        iota(perm.begin(), perm.end(), 1);
        long long total = 0;
        long long count = 0;
        do {
            vector<int> tmp = perm;
            total += simulate(tmp);
            count++;
        } while (next_permutation(perm.begin(), perm.end()));

        double exact = (double)total / count;
        double closed = (double)(n - 1) * (n + 4) / 12.0;
        cout << "  n=" << n << ": exact=" << fixed << setprecision(6) << exact
             << ", closed=" << closed << endl;
    }

    return 0;
}
