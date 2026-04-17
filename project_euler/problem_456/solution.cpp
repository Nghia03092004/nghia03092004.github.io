#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto solve = [](int N) -> long long {
        // Generate points using recurrence
        vector<long long> px(N), py(N);
        long long xv = 1248, yv = 8421;
        for(int i = 0; i < N; i++){
            px[i] = xv - 16161;
            py[i] = yv - 15051;
            xv = (xv * 1248) % 32323;
            yv = (yv * 8421) % 30103;
        }

        // Sort by angle using exact arithmetic
        // angle in [-pi, pi), map to [0, 2pi) using half-planes
        // Upper half (y > 0) or (y == 0, x > 0): first half [0, pi)
        // Lower half (y < 0) or (y == 0, x < 0): second half [pi, 2pi)
        auto half = [](long long x, long long y) -> int {
            if(y > 0) return 0;
            if(y < 0) return 1;
            if(x > 0) return 0;
            return 1; // y == 0, x < 0 (or x == 0 which shouldn't happen)
        };

        // Compare angles: return true if a comes before b in [0, 2pi) order
        auto angleLess = [&](int a, int b) -> bool {
            int ha = half(px[a], py[a]);
            int hb = half(px[b], py[b]);
            if(ha != hb) return ha < hb;
            // Same half: use cross product
            long long cross = px[a] * py[b] - py[a] * px[b];
            return cross > 0;
        };

        vector<int> idx(N);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b){
            return angleLess(a, b);
        });

        vector<long long> sx(N), sy(N);
        for(int i = 0; i < N; i++){
            sx[i] = px[idx[i]];
            sy[i] = py[idx[i]];
        }

        // Check if point j is strictly less than pi ahead of point i (in sorted circular order)
        // Two points have angular difference exactly pi iff they are anti-parallel:
        // cross(i,j) = 0 and dot(i,j) < 0
        // Angular difference < pi iff:
        //   - half(i) == half(j) and cross(i,j) > 0 [same half, j ahead of i within half]
        //     but this is angle < pi within one half
        //   - half(i) != half(j) and... we're wrapping
        //
        // Easier: point j is within pi counterclockwise from i iff cross(i,j) > 0.
        // If cross(i,j) == 0, then either same direction (angle = 0) or anti-parallel (angle = pi).
        //   dot > 0 => same direction => angle = 0
        //   dot < 0 => anti-parallel => angle = pi
        //
        // For counting, we want f(i) = # points in half-open [angle_i, angle_i + pi).
        // "j at same angle as i" has angular difference 0, so included.
        // "j at angle_i + pi" has angular difference pi, so excluded.
        //
        // In sorted order (starting from i going forward through the circular array):
        // - Points at same angle: cross(i,j)=0, dot>0 => included
        // - Points with cross(i,j)>0 => included (angle in (0,pi))
        // - Points with cross(i,j)=0, dot<0 => excluded (angle = pi)
        // - Points with cross(i,j)<0 => excluded (angle > pi)
        //
        // BUT: in sorted circular order, we go from angle_i counterclockwise through 2pi.
        // The transition from "included" to "excluded" happens when we hit angle_i + pi.
        // In sorted order: first same-angle points, then cross>0 points, then cross=0/dot<0, then cross<0.

        long long bad = 0;
        int j = 0;
        for(int i = 0; i < N; i++){
            if(j <= i) j = i + 1;
            while(j < i + N){
                int jj = j % N;
                long long cross = sx[i] * sy[jj] - sy[i] * sx[jj];
                if(cross > 0){
                    j++;
                    continue;
                }
                if(cross == 0){
                    long long dot = sx[i] * sx[jj] + sy[i] * sy[jj];
                    if(dot > 0){
                        // same direction, angle difference = 0, include
                        j++;
                        continue;
                    }
                }
                break;
            }
            long long fi = (long long)(j - i - 1);
            bad += fi * (fi - 1) / 2;
        }

        long long total = (long long)N * (N - 1) / 2;
        total = total * (N - 2) / 3;
        return total - bad;
    };

    printf("C(8) = %lld\n", solve(8));
    printf("C(600) = %lld\n", solve(600));
    printf("C(40000) = %lld\n", solve(40000));
    printf("C(2000000) = %lld\n", solve(2000000));

    return 0;
}
