#include <bits/stdc++.h>
using namespace std;

int main() {
    auto isLeap = [](int y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    };
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int w = 1, count = 0;  // 1 Jan 1900 = Monday
    for (int y = 1900; y <= 2000; y++) {
        for (int m = 0; m < 12; m++) {
            if (y >= 1901 && w == 0) count++;
            int d = (m == 1 && isLeap(y)) ? 29 : days[m];
            w = (w + d) % 7;
        }
    }
    cout << count << endl;
    return 0;
}
