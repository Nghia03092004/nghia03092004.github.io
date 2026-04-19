// IOI 1993 - Day 2, Task 2: The School Bus
// Capacitated VRP: bus makes multiple round trips from depot.
// Bitmask DP: TSP on feasible subsets + set partition DP.
// O(2^n * n^2 + 3^n), feasible for n <= 15.
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 16;
const double INF = 1e18;

int n, capacity;
int students[MAXN];
double dist_mat[MAXN + 1][MAXN + 1]; // 0 = depot, 1..n = locations
double tspDp[1 << MAXN][MAXN];
double tourCost[1 << MAXN];
int totalStudents[1 << MAXN];
bool feasible[1 << MAXN];
double partDp[1 << MAXN];

int main() {
    scanf("%d%d", &n, &capacity);

    double dx[MAXN + 1], dy[MAXN + 1];
    scanf("%lf%lf", &dx[0], &dy[0]); // depot
    for (int i = 1; i <= n; i++)
        scanf("%lf%lf%d", &dx[i], &dy[i], &students[i - 1]);

    // Compute distance matrix
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            dist_mat[i][j] = hypot(dx[i] - dx[j], dy[i] - dy[j]);

    int full = (1 << n) - 1;

    // Precompute feasibility for each subset
    for (int S = 0; S <= full; S++) {
        totalStudents[S] = 0;
        for (int i = 0; i < n; i++)
            if (S & (1 << i))
                totalStudents[S] += students[i];
        feasible[S] = (totalStudents[S] <= capacity);
    }

    // TSP DP: tspDp[S][v] = min cost from depot visiting all in S, ending at v
    for (int S = 0; S <= full; S++)
        for (int i = 0; i < n; i++)
            tspDp[S][i] = INF;

    for (int i = 0; i < n; i++)
        tspDp[1 << i][i] = dist_mat[0][i + 1];

    for (int S = 1; S <= full; S++) {
        if (!feasible[S]) continue;
        for (int v = 0; v < n; v++) {
            if (!(S & (1 << v)) || tspDp[S][v] >= INF) continue;
            for (int u = 0; u < n; u++) {
                if (S & (1 << u)) continue;
                int newS = S | (1 << u);
                if (!feasible[newS]) continue;
                double cost = tspDp[S][v] + dist_mat[v + 1][u + 1];
                if (cost < tspDp[newS][u])
                    tspDp[newS][u] = cost;
            }
        }
    }

    // Compute round-trip tour cost for each feasible subset
    for (int S = 0; S <= full; S++) {
        tourCost[S] = INF;
        if (!feasible[S]) continue;
        if (S == 0) { tourCost[S] = 0; continue; }
        for (int v = 0; v < n; v++) {
            if (!(S & (1 << v))) continue;
            tourCost[S] = min(tourCost[S], tspDp[S][v] + dist_mat[v + 1][0]);
        }
    }

    // Set partition DP: partition all locations into feasible trips
    for (int S = 0; S <= full; S++) partDp[S] = INF;
    partDp[0] = 0;

    for (int S = 1; S <= full; S++) {
        for (int T = S; T > 0; T = (T - 1) & S) {
            if (feasible[T] && tourCost[T] < INF)
                partDp[S] = min(partDp[S], partDp[S ^ T] + tourCost[T]);
        }
    }

    printf("%.2f\n", partDp[full]);
    return 0;
}
