#include <bits/stdc++.h>
using namespace std;

// Hilbert curve coordinate conversion

pair<int,int> d2xy(int n, int d) {
    int x = 0, y = 0;
    for (int s = 1; s < (1 << n); s <<= 1) {
        int rx = (d & 2) ? 1 : 0;
        int ry = ((d & 1) ^ rx) ? 1 : 0;
        if (ry == 0) {
            if (rx == 1) {
                x = s - 1 - x;
                y = s - 1 - y;
            }
            swap(x, y);
        }
        x += s * rx;
        y += s * ry;
        d >>= 2;
    }
    return {x, y};
}

int xy2d(int n, int x, int y) {
    int d = 0;
    for (int s = (1 << n) >> 1; s > 0; s >>= 1) {
        int rx = (x & s) > 0 ? 1 : 0;
        int ry = (y & s) > 0 ? 1 : 0;
        d += s * s * ((3 * rx) ^ ry);
        if (ry == 0) {
            if (rx == 1) {
                x = s - 1 - x;
                y = s - 1 - y;
            }
            swap(x, y);
        }
    }
    return d;
}

int main() {
    int order;
    cout << "Enter Hilbert curve order: ";
    cin >> order;

    long long total = 1LL << (2 * order);
    cout << "Total cells: " << total << endl;

    // Example: count lights remaining after every k-th blackout
    for (int k : {2, 3, 5, 7}) {
        long long blacked = total / k;
        long long remaining = total - blacked;
        cout << "Blackout every " << k << ": " << remaining << " remaining" << endl;
    }

    // Verify coordinate mapping for small order
    if (order <= 4) {
        int side = 1 << order;
        cout << "\nHilbert curve path:" << endl;
        for (int d = 0; d < total; d++) {
            auto [x, y] = d2xy(order, d);
            int d2 = xy2d(order, x, y);
            if (d != d2) {
                cout << "ERROR: d=" << d << " -> (" << x << "," << y << ") -> " << d2 << endl;
            }
        }
        cout << "Coordinate mapping verified." << endl;
    }

    return 0;
}
