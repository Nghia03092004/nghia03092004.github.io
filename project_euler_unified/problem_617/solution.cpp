#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

bool is_palindrome(ll n) {
    string s = to_string(n);
    string r = s;
    reverse(r.begin(), r.end());
    return s == r;
}

int main() {
    int count = 0;
    for (int n = 1; n <= 10000; n++)
        if (is_palindrome((ll)n * n)) count++;
    cout << "Palindromic squares for n<=10000: " << count << endl;
    return 0;
}
