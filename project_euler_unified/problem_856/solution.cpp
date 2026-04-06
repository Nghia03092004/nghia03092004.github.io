#include <bits/stdc++.h>
using namespace std;

map<tuple<int,int,int,int,int>, double> memo;

double E(int n1, int n2, int n3, int n4, int c) {
    int R = n1 + 2*n2 + 3*n3 + 4*n4 + c;
    if (R == 0) return 0.0;

    auto key = make_tuple(n1, n2, n3, n4, c);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    double match_prob = (double)c / R;
    double nonmatch_prob = 1.0 - match_prob;

    if (nonmatch_prob < 1e-15) {
        memo[key] = 1.0;
        return 1.0;
    }

    int total_other = n1 + 2*n2 + 3*n3 + 4*n4;
    double ev_nonmatch = 0.0;

    int nj_arr[5] = {0, n1, n2, n3, n4};
    for (int j = 1; j <= 4; j++) {
        if (nj_arr[j] == 0) continue;
        double prob_j = (double)(j * nj_arr[j]) / total_other;

        int nn1 = n1, nn2 = n2, nn3 = n3, nn4 = n4;
        if (j == 1) nn1--;
        else if (j == 2) nn2--;
        else if (j == 3) nn3--;
        else nn4--;

        if (c == 1) nn1++;
        else if (c == 2) nn2++;
        else if (c == 3) nn3++;

        int new_c = j - 1;
        ev_nonmatch += prob_j * E(nn1, nn2, nn3, nn4, new_c);
    }

    double result = 1.0 + nonmatch_prob * ev_nonmatch;
    memo[key] = result;
    return result;
}

int main() {
    double answer = 1.0 + E(0, 0, 0, 12, 3);
    printf("%.8f\n", answer);
    return 0;
}
