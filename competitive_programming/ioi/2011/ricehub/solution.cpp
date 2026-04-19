#include <bits/stdc++.h>
using namespace std;

int besthub(int R, int L, int X[], long long B){
    // X is sorted, 0-indexed
    vector<long long> prefix(R + 1, 0);
    for(int i = 0; i < R; i++){
        prefix[i + 1] = prefix[i] + X[i];
    }

    // Cost of connecting fields [l..r] to median position X[m], m = (l+r)/2
    auto cost = [&](int l, int r) -> long long {
        int m = (l + r) / 2;
        // Left part: X[m] * (m - l + 1) - sum(X[l..m])
        long long leftCost = (long long)X[m] * (m - l + 1) - (prefix[m + 1] - prefix[l]);
        // Right part: sum(X[m..r]) - X[m] * (r - m + 1)
        long long rightCost = (prefix[r + 1] - prefix[m]) - (long long)X[m] * (r - m + 1);
        return leftCost + rightCost;
    };

    int ans = 0;
    int l = 0;
    for(int r = 0; r < R; r++){
        while(cost(l, r) > B){
            l++;
        }
        ans = max(ans, r - l + 1);
    }

    return ans;
}

int main(){
    int R, L;
    long long B;
    cin >> R >> L >> B;

    int *X = new int[R];
    for(int i = 0; i < R; i++) cin >> X[i];

    cout << besthub(R, L, X, B) << "\n";
    delete[] X;
    return 0;
}
