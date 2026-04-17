#include <bits/stdc++.h>
using namespace std;

int main() {
    // Problem 247: Find the largest n such that S_n has index (3,3)
    // Region: 1 <= x, 0 <= y <= 1/x
    // Greedily place largest square first

    auto calcSide = [](double x0, double y0) -> double {
        double diff = x0 - y0;
        return (sqrt(diff * diff + 4.0) - x0 - y0) / 2.0;
    };

    struct Square {
        double side, x, y;
        int left, below;
        bool operator<(const Square& o) const {
            return side < o.side; // min-heap: smallest on top
        }
    };

    priority_queue<Square> pq;
    double s0 = calcSide(1.0, 0.0);
    pq.push({s0, 1.0, 0.0, 0, 0});

    int targetLeft = 3, targetBelow = 3;
    int n = 0, lastN = -1;
    int viable = 1;

    while (viable > 0 && !pq.empty()) {
        Square sq = pq.top();
        pq.pop();
        n++;

        if (sq.left <= targetLeft && sq.below <= targetBelow)
            viable--;

        if (sq.left == targetLeft && sq.below == targetBelow)
            lastN = n;

        // Right child
        double rx = sq.x + sq.side, ry = sq.y;
        double rs = calcSide(rx, ry);
        if (rs > 1e-15) {
            pq.push({rs, rx, ry, sq.left + 1, sq.below});
            if (sq.left + 1 <= targetLeft && sq.below <= targetBelow)
                viable++;
        }

        // Top child
        double tx = sq.x, ty = sq.y + sq.side;
        double ts = calcSide(tx, ty);
        if (ts > 1e-15) {
            pq.push({ts, tx, ty, sq.left, sq.below + 1});
            if (sq.left <= targetLeft && sq.below + 1 <= targetBelow)
                viable++;
        }
    }

    cout << lastN << endl;
    return 0;
}
