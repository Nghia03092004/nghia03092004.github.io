#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

/*
 * Problem 295: Lenticular Holes
 *
 * Two circles with lattice-point centers intersecting at two lattice points P,Q
 * form a lenticular hole if the lens region has no interior lattice points.
 * Count distinct (r1, r2) pairs with r1 <= r2 <= N.
 *
 * Key parameterization: if PQ direction is (a,b) with a,b odd, gcd(a,b)=g,
 * then D = a^2+b^2 and 4*r^2 = D*(1+m^2) for odd integer m.
 * The lenticular (no lattice point in lens) condition restricts valid m values.
 *
 * Algorithm:
 * 1. D=2 (direction (1,1)): no blocking, all odd m valid. K*(K+1)/2 pairs.
 * 2. D>2: for each direction, compute blocking thresholds from lattice points
 *    inside the disk U^2+W^2 < D. Merge representations of the same D using
 *    a staircase (non-dominated) union of valid pair regions.
 * 3. Subtract collisions: pairs counted by multiple D values (D=2 and D>2,
 *    or between different D>2 values). Use inclusion-exclusion for triple+ counted.
 */

int main() {
    ll N = 100000LL;
    ll N2 = N * N;
    ll FN2 = 4 * N2;

    // D=2: no blocking, all odd m from 1 to max_m
    ll mm2sq = 2*N2 - 1;
    ll mm2 = (ll)sqrt((double)mm2sq);
    while (mm2*mm2 > mm2sq) mm2--;
    if (mm2 % 2 == 0) mm2--;
    ll K_D2 = (mm2 + 1) / 2;
    ll D2_pairs = K_D2 * (K_D2 + 1) / 2;

    auto is_in_D2 = [&](ll v) -> bool {
        if (v <= 0 || v % 2 != 0) return false;
        ll q = v / 2 - 1;
        if (q < 1) return false;
        ll sq = (ll)sqrt((double)q);
        while (sq*sq < q) sq++;
        while (sq*sq > q) sq--;
        if (sq*sq != q) return false;
        return (sq >= 1 && sq <= mm2 && sq % 2 == 1);
    };

    ll gb_max = (ll)(2.0 * sqrt((double)N)) + 100;

    struct Rep { ll M_lo, M_hi; };
    map<ll, vector<Rep>> D_reps;
    map<ll, ll> D_max_m;

    // Enumerate primitive directions (a1,b1) with a1 < b1, both odd, gcd=1
    // and odd multipliers g. D = g^2*(a1^2+b1^2).
    for (ll b1 = 3; b1 <= gb_max; b1 += 2) {
        for (ll a1 = 1; a1 < b1; a1 += 2) {
            if (__gcd(a1, b1) != 1LL) continue;
            ll D1 = a1*a1 + b1*b1;
            if (D1 > FN2) continue;
            for (ll g = 1; g*g*D1 <= FN2; g += 2) {
                if (g*b1 > gb_max) break;
                ll D = g*g * D1, a = g*a1, b = g*b1;
                ll max_m_sq = FN2/D - 1;
                if (max_m_sq < 1) continue;
                ll max_m = (ll)sqrt((double)max_m_sq);
                while (max_m*max_m > max_m_sq) max_m--;
                if (max_m % 2 == 0) max_m--;
                if (max_m < 1) continue;
                // Quick filter using (1,1) blocking lower bound
                if ((D-2) > max_m * 2 * (b-a)) continue;

                // Compute blocking thresholds
                bool hzc = false;
                ll thn=0,thd=1; bool ht=false;
                ll tln=0,tld=1; bool htl=false;
                ll sqD = (ll)sqrt((double)D)+2;
                for (ll U=1;U<=sqD&&!hzc;U+=2) for (ll W=1;W<=sqD&&!hzc;W+=2) {
                    ll UW2=U*U+W*W; if(UW2>=D) continue; ll S=D-UW2;
                    for(int su=-1;su<=1&&!hzc;su+=2) for(int sw=-1;sw<=1&&!hzc;sw+=2) {
                        ll uu=su*U,ww=sw*W;
                        if(uu==a&&ww==b) continue; if(uu==-a&&ww==-b) continue;
                        ll C=uu*b-ww*a; if(C==0){hzc=true;break;}
                        if(C>0){if(!ht||(lll)S*thd>(lll)thn*2*C){thn=S;thd=2*C;ht=true;}}
                        else{ll ac=-C;if(!htl||(lll)S*tld>(lll)tln*2*ac){tln=S;tld=2*ac;htl=true;}}
                    }
                }
                if(hzc) continue;

                // Compute M_lo, M_hi (smallest odd integers exceeding the thresholds)
                auto sog=[](ll n,ll d)->ll{ll q=n/d;return(q%2==0)?q+1:q+2;};
                ll M_lo,M_hi;
                if(ht&&htl){if((lll)thn*tld<=(lll)tln*thd){M_lo=sog(thn,thd);M_hi=sog(tln,tld);}
                else{M_lo=sog(tln,tld);M_hi=sog(thn,thd);}}
                else if(ht){M_lo=M_hi=sog(thn,thd);}
                else if(htl){M_lo=M_hi=sog(tln,tld);}
                else{M_lo=M_hi=1;}
                if(M_lo<1)M_lo=1;if(M_hi<1)M_hi=1;
                if(M_hi>max_m)continue;

                D_reps[D].push_back({M_lo, M_hi});
                D_max_m[D] = max_m;
            }
        }
    }

    // For each D, compute union of valid pair sets using staircase of non-dominated reps
    ll formula_total = D2_pairs;
    unordered_map<ll, vector<ll>> v_to_Ds;

    for (auto& [D, reps] : D_reps) {
        ll max_m = D_max_m[D];

        // Build non-dominated staircase
        sort(reps.begin(), reps.end(), [](const Rep& a, const Rep& b) {
            return a.M_hi < b.M_hi || (a.M_hi == b.M_hi && a.M_lo < b.M_lo);
        });
        vector<Rep> nd;
        for (auto& r : reps) {
            bool dom = false;
            for (auto& s : nd) if (s.M_lo<=r.M_lo && s.M_hi<=r.M_hi) { dom=true; break; }
            if (!dom) {
                vector<Rep> nn;
                for (auto& s : nd) if (!(r.M_lo<=s.M_lo && r.M_hi<=s.M_hi)) nn.push_back(s);
                nn.push_back(r); nd = nn;
            }
        }
        sort(nd.begin(), nd.end(), [](const Rep& a, const Rep& b) { return a.M_hi < b.M_hi; });

        // Count pairs using staircase regions
        int k = (int)nd.size();
        ll pairs = 0;
        for (int i = 0; i < k; i++) {
            ll b_lo = nd[i].M_hi;
            ll b_hi = (i+1<k) ? nd[i+1].M_hi-2 : max_m;
            if (b_hi%2==0) b_hi--;
            if (b_hi < b_lo) continue;
            ll a_min = nd[i].M_lo;
            ll eff_b_lo = max(b_lo, a_min);
            if (eff_b_lo%2==0) eff_b_lo++;
            if (eff_b_lo > b_hi) continue;
            ll T = (b_hi - eff_b_lo)/2;
            ll c = (eff_b_lo - a_min)/2 + 1;
            pairs += (T+1)*c + T*(T+1)/2;
        }
        formula_total += pairs;

        // Collect v values for collision detection
        ll overall_min_m = nd.back().M_lo;
        for (ll m = overall_min_m; m <= max_m; m += 2) {
            ll v = D*(1+m*m); if(v>FN2) continue;
            v_to_Ds[v].push_back(D);
        }
    }

    // Subtract overcounting with D=2
    ll overcount_D2 = 0;
    for (auto& [D, reps] : D_reps) {
        ll max_m = D_max_m[D];
        vector<Rep> nd;
        {
            vector<Rep> sr = reps;
            sort(sr.begin(), sr.end(), [](const Rep& a, const Rep& b) {
                return a.M_hi < b.M_hi || (a.M_hi == b.M_hi && a.M_lo < b.M_lo);
            });
            for (auto& r : sr) {
                bool dom=false; for(auto&s:nd)if(s.M_lo<=r.M_lo&&s.M_hi<=r.M_hi){dom=true;break;}
                if(!dom){vector<Rep>nn;for(auto&s:nd)if(!(r.M_lo<=s.M_lo&&r.M_hi<=s.M_hi))nn.push_back(s);nn.push_back(r);nd=nn;}
            }
            sort(nd.begin(), nd.end(), [](const Rep& a, const Rep& b) { return a.M_hi < b.M_hi; });
        }
        ll overall_min_m = nd.back().M_lo;
        vector<ll> d2_ms;
        for (ll m = overall_min_m; m <= max_m; m += 2)
            if (is_in_D2(D*(1+m*m))) d2_ms.push_back(m);
        for (size_t i = 0; i < d2_ms.size(); i++)
            for (size_t j = i; j < d2_ms.size(); j++) {
                ll a = d2_ms[i], b = d2_ms[j];
                for (auto& r : nd) if (a >= r.M_lo && b >= r.M_hi) { overcount_D2++; break; }
            }
    }

    // Subtract inter-D>2 overcounting using inclusion-exclusion
    unordered_map<ll, set<ll>> v_unique_Ds;
    for (auto& [v, Ds] : v_to_Ds)
        v_unique_Ds[v] = set<ll>(Ds.begin(), Ds.end());

    map<pair<ll,ll>, vector<ll>> D_pair_shared;
    for (auto& [v, ds] : v_unique_Ds) {
        if (ds.size() <= 1) continue;
        vector<ll> dv(ds.begin(), ds.end());
        for (size_t i = 0; i < dv.size(); i++)
            for (size_t j = i+1; j < dv.size(); j++)
                D_pair_shared[{dv[i], dv[j]}].push_back(v);
    }

    ll overcount_inter = 0;
    for (auto& [dp, shared_vs] : D_pair_shared) {
        auto [D_i, D_j] = dp;
        auto get_nd = [&](ll D) -> vector<Rep> {
            vector<Rep> nd;
            auto& reps = D_reps[D];
            vector<Rep> sr = reps;
            sort(sr.begin(), sr.end(), [](const Rep& a, const Rep& b) {
                return a.M_hi < b.M_hi || (a.M_hi == b.M_hi && a.M_lo < b.M_lo);
            });
            for (auto& r : sr) {
                bool dom=false; for(auto&s:nd)if(s.M_lo<=r.M_lo&&s.M_hi<=r.M_hi){dom=true;break;}
                if(!dom){vector<Rep>nn;for(auto&s:nd)if(!(r.M_lo<=s.M_lo&&r.M_hi<=s.M_hi))nn.push_back(s);nn.push_back(r);nd=nn;}
            }
            sort(nd.begin(), nd.end(), [](const Rep& a, const Rep& b) { return a.M_hi < b.M_hi; });
            return nd;
        };
        auto is_valid_pair = [](const vector<Rep>& nd, ll a, ll b) -> bool {
            for (auto& r : nd) if (a >= r.M_lo && b >= r.M_hi) return true;
            return false;
        };
        auto nd_i = get_nd(D_i);
        auto nd_j = get_nd(D_j);
        vector<ll> ms;
        for (ll v : shared_vs) {
            ll f = v / D_i;
            ll m2 = f - 1;
            ll m = (ll)sqrt((double)m2);
            while (m*m < m2) m++;
            while (m*m > m2) m--;
            if (m*m == m2 && m >= 1 && m % 2 == 1) ms.push_back(m);
        }
        sort(ms.begin(), ms.end());
        for (size_t ii = 0; ii < ms.size(); ii++)
            for (size_t jj = ii; jj < ms.size(); jj++) {
                ll a = ms[ii], b = ms[jj];
                if (!is_valid_pair(nd_i, a, b)) continue;
                if (!is_valid_pair(nd_j, a, b)) continue;
                ll v1 = D_i * (1 + a*a), v2 = D_i * (1 + b*b);
                if (is_in_D2(v1) && is_in_D2(v2)) continue;
                overcount_inter++;
            }
    }

    // Triple+ correction: for v with k >= 3 D>2 sources, the pairwise counting
    // over-subtracts. Apply inclusion-exclusion correction.
    ll triple_adjust = 0;
    for (auto& [v, ds] : v_unique_Ds) {
        ll k = (ll)ds.size();
        if (k >= 3 && !is_in_D2(v)) {
            triple_adjust -= (k-1)*(k-2)/2;
        }
    }
    overcount_inter += triple_adjust;

    cout << formula_total - overcount_D2 - overcount_inter << endl;
    return 0;
}
