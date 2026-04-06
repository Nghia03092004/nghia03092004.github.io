#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 318: 2011 Nines
 *
 * For p < q with p+q <= 2011, pq not a perfect square, and (sqrt(q)-sqrt(p)) < 1:
 *   beta = p + q - 2*sqrt(pq)
 *   N(p,q) = ceil(-2011 / log10(beta))
 *
 * Sum all N(p,q).
 */

int main(){
    const int L = 2011;
    long long total = 0;

    for(int p = 1; p < L; p++){
        double sp = sqrt((double)p);
        int max_q = min(L - p, (int)((sp + 1.0) * (sp + 1.0)) + 1);

        for(int q = p + 1; q <= max_q; q++){
            if(p + q > L) break;

            // Check if pq is a perfect square
            long long pq = (long long)p * q;
            long long sq = (long long)sqrt((double)pq);
            // Adjust for floating point
            while(sq * sq > pq) sq--;
            while((sq + 1) * (sq + 1) <= pq) sq++;
            if(sq * sq == pq) continue;

            double beta = (double)p + (double)q - 2.0 * sqrt((double)pq);
            if(beta <= 0.0 || beta >= 1.0) continue;

            double n = -2011.0 / log10(beta);
            long long N = (long long)ceil(n - 1e-9); // ceil with small tolerance
            // More careful: use exact ceil
            N = (long long)ceil(n);
            // Handle floating point: if N * (-log10(beta)) is very close to 2011
            // we need to be careful. For safety:
            if((double)N * (-log10(beta)) < 2011.0 - 1e-9) N++;

            total += N;
        }
    }

    cout << total << endl;
    return 0;
}
