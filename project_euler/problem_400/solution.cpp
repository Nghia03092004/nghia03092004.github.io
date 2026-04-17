#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 400: Fibonacci Tree Game
 *
 * Fibonacci tree T(k): T(0) = empty, T(1) = single node,
 * T(k) = root + T(k-1) as left child + T(k-2) as right child.
 *
 * Game: players alternate removing a node + its subtree.
 * Player forced to take the root loses.
 *
 * f(k) = number of winning first moves on T(k).
 * Find last 18 digits of f(10000).
 *
 * Solution: Green Hackenbush + Colon Principle.
 * nim(k) = 1 + (nim(k-1) XOR nim(k-2)), nim(0)=0, nim(1)=1.
 * Track h(k, target) = count of non-root nodes in T(k) whose removal
 * changes nim(k) to target. Use rolling dictionaries.
 *
 * Answer: 438505383468410633
 */

typedef long long ll;
typedef unsigned long long ull;
typedef unordered_map<int, ull> HMap;

const ull MOD = 1000000000000000000ULL; // 10^18
const int K = 10000;

int nim_val[K + 1];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Compute nim values
    nim_val[0] = 0;
    nim_val[1] = 1;
    for (int k = 2; k <= K; k++) {
        nim_val[k] = 1 + (nim_val[k-1] ^ nim_val[k-2]);
    }

    // h_prev2 = h[k-2], h_prev1 = h[k-1]
    HMap h_prev2, h_prev1, hk;
    ull f_answer = 0;

    for (int k = 2; k <= K; k++) {
        hk.clear();

        // Case A: remove root of T(k-1)
        int tA = 1 + nim_val[k-2];
        hk[tA] = (hk[tA] + 1) % MOD;

        // Case B: remove root of T(k-2)
        if (k - 2 >= 1) {
            int tB = 1 + nim_val[k-1];
            hk[tB] = (hk[tB] + 1) % MOD;
        }

        // Case C: non-root nodes inside T(k-1)
        int nk2 = nim_val[k-2];
        for (auto& [t_inner, cnt] : h_prev1) {
            int t_outer = 1 + (t_inner ^ nk2);
            hk[t_outer] = (hk[t_outer] + cnt) % MOD;
        }

        // Case D: non-root nodes inside T(k-2)
        if (k - 2 >= 1) {
            int nk1 = nim_val[k-1];
            for (auto& [t_inner, cnt] : h_prev2) {
                int t_outer = 1 + (t_inner ^ nk1);
                hk[t_outer] = (hk[t_outer] + cnt) % MOD;
            }
        }

        // Compute f(k)
        if (k == K) {
            ull fk = 0;
            if (nim_val[k-2] == 0) fk++;
            if (k - 2 >= 1 && nim_val[k-1] == 0) fk++;

            auto it1 = h_prev1.find(nim_val[k-2]);
            if (it1 != h_prev1.end()) fk = (fk + it1->second) % MOD;

            if (k - 2 >= 1) {
                auto it2 = h_prev2.find(nim_val[k-1]);
                if (it2 != h_prev2.end()) fk = (fk + it2->second) % MOD;
            }
            f_answer = fk;
        }

        h_prev2 = move(h_prev1);
        h_prev1 = move(hk);
        hk = HMap();  // reset after move
    }

    cout << "f(" << K << ") last 18 digits = " << f_answer << endl;

    // Verification for small values
    {
        // Recompute for small K
        int KS = 30;
        int nim_s[31];
        nim_s[0] = 0; nim_s[1] = 1;
        for (int k = 2; k <= KS; k++)
            nim_s[k] = 1 + (nim_s[k-1] ^ nim_s[k-2]);

        HMap hp2, hp1;
        cout << "\nVerification (small k):" << endl;
        cout << "  k   f(k)" << endl;

        for (int k = 2; k <= KS; k++) {
            HMap h;
            int tA2 = 1 + nim_s[k-2];
            h[tA2]++;
            if (k - 2 >= 1) {
                int tB2 = 1 + nim_s[k-1];
                h[tB2]++;
            }
            for (auto& [ti, c] : hp1) {
                int to2 = 1 + (ti ^ nim_s[k-2]);
                h[to2] += c;
            }
            if (k - 2 >= 1) {
                for (auto& [ti, c] : hp2) {
                    int to2 = 1 + (ti ^ nim_s[k-1]);
                    h[to2] += c;
                }
            }

            ull fk = 0;
            if (nim_s[k-2] == 0) fk++;
            if (k - 2 >= 1 && nim_s[k-1] == 0) fk++;
            auto it1 = hp1.find(nim_s[k-2]);
            if (it1 != hp1.end()) fk += it1->second;
            if (k - 2 >= 1) {
                auto it2 = hp2.find(nim_s[k-1]);
                if (it2 != hp2.end()) fk += it2->second;
            }

            if (k <= 15 || k == 30)
                cout << "  " << k << "   " << fk << endl;

            hp2 = move(hp1);
            hp1 = move(h);
        }
    }

    return 0;
}
