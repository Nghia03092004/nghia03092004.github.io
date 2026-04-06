#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

bool check_descartes(ll a, ll b, ll c) {
    // Check a^2*b^2 + b^2*c^2 + a^2*c^2 == 2*a*b*c*(a+b+c)
    ll lhs = a*a*b*b + b*b*c*c + a*a*c*c;
    ll rhs = 2*a*b*c*(a+b+c);
    return lhs == rhs;
}

int main() {
    // Brute-force search for small N
    int N = 1000;
    ll total_sum = 0;
    int count = 0;

    for (ll a = 1; a <= N; a++) {
        for (ll b = a; a + b <= N; b++) {
            for (ll c = b; a + b + c <= N; c++) {
                if (check_descartes(a, b, c)) {
                    total_sum += a + b + c;
                    count++;
                    if (count <= 20) {
                        cout << "(" << a << ", " << b << ", " << c
                             << "), sum = " << (a+b+c) << endl;
                    }
                }
            }
        }
    }

    cout << "\nTotal triples with a+b+c <= " << N << ": " << count << endl;
    cout << "Sum of (a+b+c): " << total_sum << endl;

    return 0;
}
