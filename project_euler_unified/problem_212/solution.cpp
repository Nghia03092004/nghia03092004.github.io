#include <bits/stdc++.h>
using namespace std;

int main() {
    const int NCUBOIDS = 50000;
    const int SLEN = 6 * NCUBOIDS;

    vector<long long> S(SLEN + 1);
    for (int k = 1; k <= 55; k++) {
        long long k3 = (long long)k * k * k;
        S[k] = ((100003LL - 200003LL * k + 300007LL * k3) % 1000000LL + 1000000LL) % 1000000LL;
    }
    for (int k = 56; k <= SLEN; k++)
        S[k] = (S[k - 24] + S[k - 55]) % 1000000LL;

    struct Cuboid { int x1, y1, z1, x2, y2, z2; };
    vector<Cuboid> cuboids(NCUBOIDS);

    for (int n = 1; n <= NCUBOIDS; n++) {
        int i = n - 1;
        cuboids[i] = {
            (int)(S[6*n-5] % 10000), (int)(S[6*n-4] % 10000), (int)(S[6*n-3] % 10000),
            (int)(S[6*n-5] % 10000 + 1 + S[6*n-2] % 399),
            (int)(S[6*n-4] % 10000 + 1 + S[6*n-1] % 399),
            (int)(S[6*n-3] % 10000 + 1 + S[6*n] % 399)
        };
    }

    vector<int> zcoords;
    for (int i = 0; i < NCUBOIDS; i++) {
        zcoords.push_back(cuboids[i].z1);
        zcoords.push_back(cuboids[i].z2);
    }
    sort(zcoords.begin(), zcoords.end());
    zcoords.erase(unique(zcoords.begin(), zcoords.end()), zcoords.end());

    long long totalVolume = 0;

    for (int zi = 0; zi + 1 < (int)zcoords.size(); zi++) {
        int z0 = zcoords[zi], z1 = zcoords[zi + 1];
        int dz = z1 - z0;

        vector<int> active;
        for (int i = 0; i < NCUBOIDS; i++)
            if (cuboids[i].z1 <= z0 && z1 <= cuboids[i].z2)
                active.push_back(i);
        if (active.empty()) continue;

        vector<int> localx;
        for (int i : active) {
            localx.push_back(cuboids[i].x1);
            localx.push_back(cuboids[i].x2);
        }
        sort(localx.begin(), localx.end());
        localx.erase(unique(localx.begin(), localx.end()), localx.end());

        for (int xi = 0; xi + 1 < (int)localx.size(); xi++) {
            int x0 = localx[xi], x1 = localx[xi + 1];
            int dx = x1 - x0;

            vector<pair<int, int>> yintervals;
            for (int i : active)
                if (cuboids[i].x1 <= x0 && x1 <= cuboids[i].x2)
                    yintervals.push_back({cuboids[i].y1, cuboids[i].y2});
            if (yintervals.empty()) continue;

            sort(yintervals.begin(), yintervals.end());
            long long ylen = 0;
            int cy0 = yintervals[0].first, cy1 = yintervals[0].second;
            for (int j = 1; j < (int)yintervals.size(); j++) {
                if (yintervals[j].first >= cy1) {
                    ylen += cy1 - cy0;
                    cy0 = yintervals[j].first;
                    cy1 = yintervals[j].second;
                } else {
                    cy1 = max(cy1, yintervals[j].second);
                }
            }
            ylen += cy1 - cy0;
            totalVolume += (long long)dz * dx * ylen;
        }
    }

    cout << totalVolume << endl;
    return 0;
}
