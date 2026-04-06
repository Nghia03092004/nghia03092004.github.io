#include <bits/stdc++.h>
using namespace std;

int main(){
    // Sphere packing in a tube of radius R=50mm.
    // 21 spheres with radii 30..50 mm.
    // Bitmask DP to find optimal ordering.

    const int N = 21;
    const double R = 50.0;
    double radii[N];
    for(int i = 0; i < N; i++) radii[i] = 30.0 + i;

    // Precompute vertical gaps
    double dz[N][N];
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++){
            double s = radii[i] + radii[j];
            double val = 4.0 * R * (s - R);
            dz[i][j] = (val > 0) ? sqrt(val) : 0.0;
        }

    // dp[mask][last] = min partial length
    int FULL = (1 << N);
    vector<vector<double>> dp(FULL, vector<double>(N, 1e18));

    // Initialize: place one sphere
    for(int i = 0; i < N; i++)
        dp[1 << i][i] = radii[i];

    for(int mask = 1; mask < FULL; mask++){
        for(int j = 0; j < N; j++){
            if(!(mask & (1 << j))) continue;
            if(dp[mask][j] >= 1e17) continue;
            // Try adding sphere k
            for(int k = 0; k < N; k++){
                if(mask & (1 << k)) continue;
                int nmask = mask | (1 << k);
                double cost = dp[mask][j] + dz[j][k];
                if(cost < dp[nmask][k])
                    dp[nmask][k] = cost;
            }
        }
    }

    double best = 1e18;
    for(int j = 0; j < N; j++)
        best = min(best, dp[FULL-1][j] + radii[j]);

    // Convert mm to micrometers
    long long ans = llround(best * 1000.0);
    cout << ans << endl;

    return 0;
}
