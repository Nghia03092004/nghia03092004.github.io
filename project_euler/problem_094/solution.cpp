#include <bits/stdc++.h>
using namespace std;

int main() {
    // Almost equilateral triangles: sides (a, a, a+/-1) with integral area.
    // Both cases reduce to Pell equation X^2 - 3Y^2 = 1.
    // Fundamental solution: (X,Y) = (2,1).
    // Recurrence: X' = 2X + 3Y, Y' = X + 2Y.
    //
    // X mod 3 == 1 => Case 1: a = (2X+1)/3, perimeter = 3a+1
    // X mod 3 == 2 => Case 2: a = (2X-1)/3, perimeter = 3a-1

    const long long LIMIT = 1000000000LL;
    long long total = 0;
    long long x = 2, y = 1;

    while (true) {
        long long a, perim;
        if (x % 3 == 1) {
            a = (2 * x + 1) / 3;
            perim = 3 * a + 1;
        } else {
            a = (2 * x - 1) / 3;
            perim = 3 * a - 1;
        }
        if (perim > LIMIT) break;
        if (a >= 2) total += perim;

        long long nx = 2 * x + 3 * y;
        long long ny = x + 2 * y;
        x = nx;
        y = ny;
    }

    cout << total << endl;
    return 0;
}
