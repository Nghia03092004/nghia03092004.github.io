// IOI 1993 - Day 1, Task 2: The Primes
// Fill a 5x5 grid so each row, column, and both diagonals are 5-digit primes,
// all rows have the same digit sum S, and grid[0][0] = given digit.
// Backtracking with prefix pruning on columns and diagonals.
#include <bits/stdc++.h>
using namespace std;

bool sieve[100000];
int S, startDigit;
int grid[5][5];
vector<string> solutions;

// Primes with digit sum S, stored as digit arrays
vector<vector<int>> candidates;

// Valid prefixes of length 1..5 among primes with digit sum S
set<int> validPrefixes[6];

int digitSum(int p) {
    int s = 0;
    while (p) { s += p % 10; p /= 10; }
    return s;
}

bool checkPartialCols(int rowsFilled) {
    for (int c = 0; c < 5; c++) {
        int prefix = 0;
        for (int r = 0; r < rowsFilled; r++)
            prefix = prefix * 10 + grid[r][c];
        if (!validPrefixes[rowsFilled].count(prefix))
            return false;
    }
    return true;
}

bool checkPartialDiags(int rowsFilled) {
    // Main diagonal
    int prefix = 0;
    for (int i = 0; i < rowsFilled; i++)
        prefix = prefix * 10 + grid[i][i];
    if (!validPrefixes[rowsFilled].count(prefix))
        return false;

    // Anti-diagonal
    prefix = 0;
    for (int i = 0; i < rowsFilled; i++)
        prefix = prefix * 10 + grid[i][4 - i];
    if (!validPrefixes[rowsFilled].count(prefix))
        return false;

    return true;
}

void solve(int row) {
    if (row == 5) {
        string sol;
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++)
                sol += (char)('0' + grid[r][c]);
            sol += '\n';
        }
        solutions.push_back(sol);
        return;
    }

    for (auto& d : candidates) {
        if (row == 0 && d[0] != startDigit) continue;

        for (int c = 0; c < 5; c++) grid[row][c] = d[c];

        if (!checkPartialCols(row + 1)) continue;
        if (!checkPartialDiags(row + 1)) continue;

        solve(row + 1);
    }
}

int main() {
    // Sieve of Eratosthenes
    memset(sieve, true, sizeof(sieve));
    sieve[0] = sieve[1] = false;
    for (int i = 2; i < 100000; i++)
        if (sieve[i])
            for (long long j = (long long)i * i; j < 100000; j += i)
                sieve[j] = false;

    scanf("%d%d", &S, &startDigit);

    // Build candidate list and prefix sets
    for (int p = 10000; p <= 99999; p++) {
        if (!sieve[p] || digitSum(p) != S) continue;
        vector<int> d(5);
        int tmp = p;
        for (int i = 4; i >= 0; i--) { d[i] = tmp % 10; tmp /= 10; }
        candidates.push_back(d);

        int prefix = 0;
        for (int len = 1; len <= 5; len++) {
            prefix = prefix * 10 + d[len - 1];
            validPrefixes[len].insert(prefix);
        }
    }

    solve(0);

    if (solutions.empty()) {
        printf("NONE\n");
    } else {
        sort(solutions.begin(), solutions.end());
        for (int i = 0; i < (int)solutions.size(); i++) {
            if (i > 0) printf("\n");
            printf("%s", solutions[i].c_str());
        }
    }
    return 0;
}
