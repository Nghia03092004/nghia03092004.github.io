#include <bits/stdc++.h>
using namespace std;

int main(){
    // Problem 82: Path Sum Three Ways
    // Minimal path sum from left column to right column,
    // moving right, up, or down.

    int n = 80;
    vector<vector<long long>> M(n, vector<long long>(n));

    ifstream fin("p082_matrix.txt");
    if(fin.is_open()){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                fin >> M[i][j];
                char ch;
                if(j < n-1) fin >> ch;
            }
        }
        fin.close();
    } else {
        n = 5;
        M = {
            {131, 673, 234, 103, 18},
            {201, 96, 342, 965, 150},
            {630, 803, 746, 422, 111},
            {537, 699, 497, 121, 956},
            {805, 732, 524, 37, 331}
        };
        // Expected for 5x5: 994
    }

    // DP column by column
    vector<long long> dp(n);
    for(int i = 0; i < n; i++)
        dp[i] = M[i][0];

    for(int j = 1; j < n; j++){
        vector<long long> ndp(n);

        // Start with cost of coming from the left
        for(int i = 0; i < n; i++)
            ndp[i] = dp[i] + M[i][j];

        // Downward pass
        for(int i = 1; i < n; i++)
            ndp[i] = min(ndp[i], ndp[i-1] + M[i][j]);

        // Upward pass
        for(int i = n-2; i >= 0; i--)
            ndp[i] = min(ndp[i], ndp[i+1] + M[i][j]);

        dp = ndp;
    }

    cout << *min_element(dp.begin(), dp.end()) << endl;
    // Full 80x80 answer: 260324

    return 0;
}
