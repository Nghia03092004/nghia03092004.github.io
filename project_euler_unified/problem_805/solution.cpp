#include <bits/stdc++.h>
using namespace std;
int main() {
    long long N = 10000, total = 0;
    for (long long n = 10; n <= N; n++) {
        string s = to_string(n);
        string shifted_s = s.substr(1) + s[0];
        long long shifted = stoll(shifted_s);
        if (shifted > 0) total += n % shifted;
    }
    cout << total << endl;
    return 0;
}
