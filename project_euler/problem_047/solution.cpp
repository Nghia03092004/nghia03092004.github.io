#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 150000;
    vector<int> dpf(LIMIT, 0);

    for (int i = 2; i < LIMIT; i++) {
        if (dpf[i] == 0) {
            for (int j = i; j < LIMIT; j += i) {
                dpf[j]++;
            }
        }
    }

    int consecutive = 0;
    for (int i = 2; i < LIMIT; i++) {
        if (dpf[i] == 4) {
            consecutive++;
            if (consecutive == 4) {
                cout << i - 3 << endl;
                return 0;
            }
        } else {
            consecutive = 0;
        }
    }
    return 0;
}
