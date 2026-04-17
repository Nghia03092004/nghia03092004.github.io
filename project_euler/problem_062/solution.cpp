#include <bits/stdc++.h>
using namespace std;

int main() {
    map<string, vector<long long>> groups;
    int prev_digits = 0;
    long long answer = 0;

    for (long long n = 1; ; n++) {
        long long cube = n * n * n;
        string s = to_string(cube);
        int d = s.size();

        if (d > prev_digits && prev_digits > 0) {
            // Finalize all groups of the previous digit count
            for (auto& [key, vec] : groups) {
                if ((int)key.size() == prev_digits && vec.size() == 5) {
                    if (answer == 0 || vec[0] < answer)
                        answer = vec[0];
                }
            }
            if (answer) {
                cout << answer << endl;
                return 0;
            }
            prev_digits = d;
        }
        if (prev_digits == 0) prev_digits = d;

        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(cube);
    }
}
