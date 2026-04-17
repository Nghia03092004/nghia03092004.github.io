#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 163: Cross-hatched Triangles
 *
 * Count triangles in a cross-hatched equilateral triangle of size n = 36.
 * Six line families in oblique coordinates; enumerate all C(6,3) = 20
 * family triples and all line triples, checking containment and
 * non-concurrency via exact integer arithmetic.
 */

int main() {
    const int n = 36;

    int a[6] = {1, 0, 1, 1, 1, 2};
    int b[6] = {0, 1, 1, -1, 2, 1};

    vector<vector<int>> lines(6);
    for (int k = 0; k <= n; k++) lines[0].push_back(k);
    for (int k = 0; k <= n; k++) lines[1].push_back(k);
    for (int k = 0; k <= n; k++) lines[2].push_back(k);
    for (int k = -(n - 1); k <= n - 1; k++) lines[3].push_back(k);
    for (int k = 1; k <= 2 * n - 1; k++) lines[4].push_back(k);
    for (int k = 1; k <= 2 * n - 1; k++) lines[5].push_back(k);

    long long total = 0;

    for (int f1 = 0; f1 < 6; f1++) {
        for (int f2 = f1 + 1; f2 < 6; f2++) {
            for (int f3 = f2 + 1; f3 < 6; f3++) {
                int a1 = a[f1], b1 = b[f1];
                int a2 = a[f2], b2 = b[f2];
                int a3 = a[f3], b3 = b[f3];

                int d12 = a1 * b2 - a2 * b1;
                int d13 = a1 * b3 - a3 * b1;
                int d23 = a2 * b3 - a3 * b2;
                if (d12 == 0 || d13 == 0 || d23 == 0) continue;

                for (int c1 : lines[f1]) {
                    for (int c2 : lines[f2]) {
                        long long nu12 = (long long)c1 * b2 - (long long)c2 * b1;
                        long long nv12 = (long long)a1 * c2 - (long long)a2 * c1;
                        long long dd12 = d12;
                        if (dd12 < 0) { nu12 = -nu12; nv12 = -nv12; dd12 = -dd12; }
                        if (nu12 < 0 || nv12 < 0 || nu12 + nv12 > (long long)n * dd12)
                            continue;

                        for (int c3 : lines[f3]) {
                            long long nu13 = (long long)c1 * b3 - (long long)c3 * b1;
                            long long nv13 = (long long)a1 * c3 - (long long)a3 * c1;
                            long long dd13 = d13;
                            if (dd13 < 0) { nu13 = -nu13; nv13 = -nv13; dd13 = -dd13; }
                            if (nu13 < 0 || nv13 < 0 || nu13 + nv13 > (long long)n * dd13)
                                continue;

                            long long nu23 = (long long)c2 * b3 - (long long)c3 * b2;
                            long long nv23 = (long long)a2 * c3 - (long long)a3 * c2;
                            long long dd23 = d23;
                            if (dd23 < 0) { nu23 = -nu23; nv23 = -nv23; dd23 = -dd23; }
                            if (nu23 < 0 || nv23 < 0 || nu23 + nv23 > (long long)n * dd23)
                                continue;

                            // Non-concurrency: P12 != P13
                            if (nu12 * dd13 == nu13 * dd12 && nv12 * dd13 == nv13 * dd12)
                                continue;

                            total++;
                        }
                    }
                }
            }
        }
    }

    cout << total << endl;
    return 0;
}
