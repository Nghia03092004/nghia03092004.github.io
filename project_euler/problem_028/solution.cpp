#include <bits/stdc++.h>
using namespace std;

int main() {
    long long N = 1001;
    // Closed-form: S = (4N^3 + 3N^2 + 8N - 9) / 6
    long long answer = (4*N*N*N + 3*N*N + 8*N - 9) / 6;
    cout << answer << endl;
    return 0;
}
