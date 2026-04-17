#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int K = 2000000;
    const long long MOD = 50515093LL;

    // Generate sequence s_0, s_1, s_2, ...
    // s_0 = 290797, s_{n+1} = s_n^2 mod 50515093
    // P_n = (s_{2n}, s_{2n+1})
    vector<pair<long long,long long>> pts(K);
    long long s = 290797;
    for(int i = 0; i < K; i++){
        long long x = s;
        s = (s * s) % MOD;
        long long y = s;
        s = (s * s) % MOD;
        pts[i] = {x, y};
    }

    // Sort by x
    sort(pts.begin(), pts.end());

    // Sweep line with set sorted by (y, x)
    auto cmp = [](const pair<long long,long long>& a, const pair<long long,long long>& b){
        if(a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    };
    set<pair<long long,long long>, decltype(cmp)> active(cmp);

    double best = 1e18;
    int left = 0;

    for(int i = 0; i < K; i++){
        long long d = (long long)ceil(best);

        // Remove points too far left
        while(left < i && pts[i].first - pts[left].first > d){
            active.erase(pts[left]);
            left++;
        }

        // Check nearby points in y range
        pair<long long,long long> lo = {-1, pts[i].second - d};
        auto it = active.lower_bound(lo);

        for(; it != active.end() && it->second <= pts[i].second + d; ++it){
            double dx = (double)(pts[i].first - it->first);
            double dy = (double)(pts[i].second - it->second);
            double dist = sqrt(dx*dx + dy*dy);
            if(dist < best){
                best = dist;
            }
        }

        active.insert(pts[i]);
    }

    cout << fixed << setprecision(9) << best << endl;

    return 0;
}
