#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

int main() {
    int n = 50;
    // Sieve primes up to n
    vector<int> primes;
    vector<bool> sieve(n + 1, true);
    for (int i = 2; i <= n; i++) {
        if (sieve[i]) {
            primes.push_back(i);
            for (int j = 2*i; j <= n; j += i) sieve[j] = false;
        }
    }
    int m = primes.size();
    
    // Build GF(2) matrix
    vector<vector<int>> rows(n, vector<int>(m, 0));
    for (int k = 1; k <= n; k++) {
        int temp = k;
        for (int i = 0; i < m; i++)
            while (temp % primes[i] == 0) { rows[k-1][i] ^= 1; temp /= primes[i]; }
    }
    
    // Gaussian elimination
    int rank = 0;
    for (int col = 0; col < m; col++) {
        int pivot = -1;
        for (int row = rank; row < n; row++)
            if (rows[row][col]) { pivot = row; break; }
        if (pivot < 0) continue;
        swap(rows[rank], rows[pivot]);
        for (int row = 0; row < n; row++)
            if (row != rank && rows[row][col])
                for (int c = 0; c < m; c++)
                    rows[row][c] ^= rows[rank][c];
        rank++;
    }
    
    // 2^(n-rank) - 1
    lll result = 1;
    for (int i = 0; i < n - rank; i++) result *= 2;
    cout << (ll)(result - 1) << endl;
    return 0;
}
