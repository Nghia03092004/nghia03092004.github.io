#include <bits/stdc++.h>
using namespace std;

int main() {
    // Generate all C(10,6) = 210 possible cubes
    vector<vector<int>> cubes;
    for (int mask = 0; mask < 1024; mask++) {
        if (__builtin_popcount(mask) == 6) {
            vector<int> cube;
            for (int i = 0; i < 10; i++)
                if (mask & (1 << i))
                    cube.push_back(i);
            cubes.push_back(cube);
        }
    }

    // Required pairs for squares 01,04,09,16,25,36,49,64,81
    int pairs[][2] = {
        {0,1}, {0,4}, {0,9}, {1,6}, {2,5}, {3,6}, {4,9}, {6,4}, {8,1}
    };

    auto has = [](const vector<int>& cube, int d) -> bool {
        for (int x : cube) {
            if (x == d) return true;
            // 6 and 9 are interchangeable
            if (d == 6 && x == 9) return true;
            if (d == 9 && x == 6) return true;
        }
        return false;
    };

    int count = 0;
    int n = cubes.size();

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            bool valid = true;
            for (auto& p : pairs) {
                int a = p[0], b = p[1];
                bool ok = (has(cubes[i], a) && has(cubes[j], b)) ||
                          (has(cubes[j], a) && has(cubes[i], b));
                if (!ok) { valid = false; break; }
            }
            if (valid) count++;
        }
    }

    cout << count << endl;
    return 0;
}
