#include <bits/stdc++.h>
using namespace std;

int main(){
    // Peter: 9 four-sided dice. Colin: 6 six-sided dice.
    // P(Peter > Colin) rounded to 7 decimal places.

    // Compute frequency distribution for sum of n dice with faces 1..f
    auto dice_dist = [](int n, int f) -> vector<long long> {
        int maxsum = n * f;
        vector<long long> freq(maxsum + 1, 0);
        freq[0] = 1;
        for(int die = 0; die < n; die++){
            vector<long long> nf(maxsum + 1, 0);
            for(int s = 0; s <= maxsum; s++){
                if(freq[s] == 0) continue;
                for(int face = 1; face <= f && s + face <= maxsum; face++){
                    nf[s + face] += freq[s];
                }
            }
            freq = nf;
        }
        return freq;
    };

    auto peter = dice_dist(9, 4);  // indices 0..36, nonzero for 9..36
    auto colin = dice_dist(6, 6);  // indices 0..36, nonzero for 6..36

    // Cumulative sum for Colin
    vector<long long> colin_cum(37, 0);
    for(int s = 1; s <= 36; s++){
        colin_cum[s] = colin_cum[s-1] + colin[s];
    }

    // P(Peter > Colin) = sum over p of peter[p] * colin_cum[p-1]
    long long win = 0;
    for(int p = 9; p <= 36; p++){
        win += peter[p] * colin_cum[p-1];
    }

    double total = (double)(1LL << 18) * 46656.0; // 4^9 * 6^6 = 262144 * 46656
    double prob = (double)win / (262144.0 * 46656.0);

    cout << fixed << setprecision(7) << prob << endl;
    return 0;
}
