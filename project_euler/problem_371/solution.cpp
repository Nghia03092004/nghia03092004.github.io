#include <bits/stdc++.h>
using namespace std;

int main(){
    // E(k, s): expected plates from state (k, s)
    // k = number of active complementary pairs (one element seen)
    // s = whether 500 has been seen
    // Pairs: (1,999),(2,998),...,(499,501) -> 499 pairs
    // 500+500=1000 special, 000 irrelevant

    // E(k,1) = (1000 + 2*(499-k)*E(k+1,1)) / (999-k)
    // E(k,0) = (1000 + E(k,1) + 2*(499-k)*E(k+1,0)) / (999-k)
    // Base: E(499,1) = 1000/500 = 2
    //        E(499,0) = (1000 + 2) / 500

    vector<double> E0(500, 0.0), E1(500, 0.0);

    E1[499] = 1000.0 / 500.0;
    E0[499] = (1000.0 + E1[499]) / 500.0;

    for(int k = 498; k >= 0; k--){
        E1[k] = (1000.0 + 2.0*(499-k)*E1[k+1]) / (999.0 - k);
        E0[k] = (1000.0 + E1[k] + 2.0*(499-k)*E0[k+1]) / (999.0 - k);
    }

    printf("%.8f\n", E0[0]);
    return 0;
}
