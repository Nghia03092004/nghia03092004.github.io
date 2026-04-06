#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

/*
 * Problem 102: Triangle Containment
 *
 * Test whether the origin lies inside each triangle using the
 * cross-product sign method: compute the z-components of
 * OA x OB, OB x OC, OC x OA and check that all share the same sign.
 */

bool contains_origin(int x1, int y1, int x2, int y2, int x3, int y3) {
    long long d1 = (long long)x1 * y2 - (long long)x2 * y1;
    long long d2 = (long long)x2 * y3 - (long long)x3 * y2;
    long long d3 = (long long)x3 * y1 - (long long)x1 * y3;
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(has_neg && has_pos);
}

int main() {
    ifstream fin("p102_triangles.txt");
    istream& in = fin.is_open() ? fin : cin;

    int count = 0;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        replace(line.begin(), line.end(), ',', ' ');
        istringstream iss(line);
        int x1, y1, x2, y2, x3, y3;
        if (iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3) {
            if (contains_origin(x1, y1, x2, y2, x3, y3))
                count++;
        }
    }

    cout << count << endl;
    return 0;
}
