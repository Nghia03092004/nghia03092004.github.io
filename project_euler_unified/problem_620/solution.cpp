#include <bits/stdc++.h>
using namespace std;

int main() {
    int max_teeth = 100;
    int count = 0;
    for (int s = 1; s <= max_teeth; s++) {
        for (int r = s + 1; r <= max_teeth; r++) {
            if ((r - s) % 2 != 0) continue;
            int p = (r - s) / 2;
            if (p < 1) continue;
            if (r % s == 0) {  // integer ratio
                count++;
            }
        }
    }
    cout << "Integer ratio configs: " << count << endl;
    return 0;
}
