#include <bits/stdc++.h>
using namespace std;

map<tuple<int,int,int,int>, double> memo;

double E(int a2, int a3, int a4, int a5) {
    if (a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0)
        return 0.0;
    auto key = make_tuple(a2, a3, a4, a5);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    int T = a2 + a3 + a4 + a5;
    double result = 0.0;

    // Single-sheet indicator, excluding terminal state (0,0,0,1)
    if (T == 1 && !(a2 == 0 && a3 == 0 && a4 == 0 && a5 == 1))
        result = 1.0;

    if (a2 > 0)
        result += (double)a2 / T * E(a2 - 1, a3 + 1, a4 + 1, a5 + 1);
    if (a3 > 0)
        result += (double)a3 / T * E(a2, a3 - 1, a4 + 1, a5 + 1);
    if (a4 > 0)
        result += (double)a4 / T * E(a2, a3, a4 - 1, a5 + 1);
    if (a5 > 0)
        result += (double)a5 / T * E(a2, a3, a4, a5 - 1);

    memo[key] = result;
    return result;
}

int main() {
    printf("%.6f\n", E(1, 1, 1, 1));
    return 0;
}
