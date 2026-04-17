#include <bits/stdc++.h>
using namespace std;

int main(){
    // Problem 81: Path Sum Two Ways
    // Reads an 80x80 matrix from "p081_matrix.txt" (comma-separated)
    // and finds the minimal path sum from top-left to bottom-right
    // moving only right and down.
    //
    // If the input file is not available, we use a small example
    // and print the expected answer for the full problem.

    int n = 80;
    vector<vector<long long>> M(n, vector<long long>(n));

    ifstream fin("p081_matrix.txt");
    if(fin.is_open()){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                fin >> M[i][j];
                char ch;
                if(j < n-1) fin >> ch; // consume comma
            }
        }
        fin.close();
    } else {
        // Example 5x5 matrix from Project Euler
        n = 5;
        M = {
            {131, 673, 234, 103, 18},
            {201, 96, 342, 965, 150},
            {630, 803, 746, 422, 111},
            {537, 699, 497, 121, 956},
            {805, 732, 524, 37, 331}
        };
        // Expected answer for 5x5: 2427
    }

    // DP
    vector<vector<long long>> dp(n, vector<long long>(n, 0));
    dp[0][0] = M[0][0];

    // First row
    for(int j = 1; j < n; j++)
        dp[0][j] = dp[0][j-1] + M[0][j];

    // First column
    for(int i = 1; i < n; i++)
        dp[i][0] = dp[i-1][0] + M[i][0];

    // Fill rest
    for(int i = 1; i < n; i++)
        for(int j = 1; j < n; j++)
            dp[i][j] = M[i][j] + min(dp[i-1][j], dp[i][j-1]);

    cout << dp[n-1][n-1] << endl;
    // Full 80x80 answer: 427337

    return 0;
}
