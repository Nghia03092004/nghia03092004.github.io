#include <bits/stdc++.h>
using namespace std;

int main(){
    const int R = 105;
    const long long R2 = (long long)R * R;

    // Collect non-origin lattice points inside/on circle
    // We'll sort by angle using cross/dot product comparisons to avoid floating point

    // Group points by direction (reduced direction vector)
    // For the semicircle counting, we use a standard approach:
    // Sort by angle, then two-pointer

    struct Point {
        int x, y;
    };

    vector<Point> pts;
    for(int x = -R; x <= R; x++){
        for(int y = -R; y <= R; y++){
            if(x == 0 && y == 0) continue;
            if((long long)x*x + (long long)y*y <= R2){
                pts.push_back({x, y});
            }
        }
    }

    int N = pts.size();

    // Sort by angle using atan2 (careful with precision)
    // Use a comparator based on half-plane then cross product
    auto half = [](const Point& p) -> int {
        // 0 for upper half-plane (y > 0, or y == 0 && x > 0)
        // 1 for lower half-plane
        if(p.y != 0) return (p.y > 0) ? 0 : 1;
        return (p.x > 0) ? 0 : 1;
    };

    sort(pts.begin(), pts.end(), [&](const Point& a, const Point& b){
        int ha = half(a), hb = half(b);
        if(ha != hb) return ha < hb;
        long long cross = (long long)a.x * b.y - (long long)a.y * b.x;
        return cross > 0; // a before b if cross > 0 (a is counterclockwise before b)
    });

    // Two-pointer: for each point i, count points strictly in (angle_i, angle_i + pi)
    // A point j is in the open semicircle (angle_i, angle_i + pi) if:
    //   cross(i, j) > 0 (j is strictly counterclockwise from i)
    //   AND dot-check that j is not past pi from i
    // Actually: j is in (theta_i, theta_i + pi) iff
    //   cross(i, j) > 0  (strictly counterclockwise, less than pi away)

    // Duplicate the array for wraparound
    vector<Point> ext(2 * N);
    for(int i = 0; i < N; i++){
        ext[i] = pts[i];
        ext[i + N] = pts[i]; // wrapped
    }

    // cross(a, b) > 0 means b is strictly CCW from a (angle between 0 and pi)
    // cross(a, b) = 0 means collinear
    // cross(a, b) < 0 means b is CW or past pi

    // For point i, we want j such that cross(pts[i], pts[j % N]) > 0
    // This means pts[j] is in the open semicircle (theta_i, theta_i + pi)

    long long bad = 0;
    int j = 0;
    for(int i = 0; i < N; i++){
        if(j <= i) j = i + 1;
        // Advance j while cross(pts[i], ext[j]) > 0
        while(j < i + N){
            long long cross = (long long)pts[i].x * ext[j].y - (long long)pts[i].y * ext[j].x;
            if(cross <= 0) break;
            j++;
        }
        long long cnt = j - i - 1;
        bad += cnt * (cnt - 1) / 2;
    }

    long long total = (long long)N * (N - 1) * (N - 2) / 6;
    cout << total - bad << endl;

    return 0;
}
