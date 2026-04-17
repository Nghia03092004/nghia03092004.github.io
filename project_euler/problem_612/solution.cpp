#include <bits/stdc++.h>
using namespace std;

bool is_prime(int n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i+2) == 0) return false;
    return true;
}

int main() {
    int N = 1000, count = 0;
    for (int n = 2; n <= N; n++) {
        string s = to_string(n);
        sort(s.begin(), s.end());
        bool found = false;
        do {
            if (s[0] != '0' && is_prime(stoi(s))) { found = true; break; }
        } while (next_permutation(s.begin(), s.end()));
        if (found) count++;
    }
    cout << count << endl;
    return 0;
}
