#include <bits/stdc++.h>
using namespace std;

struct Form {
    long long a, b, c;
};

// Gauss reduction of binary quadratic form ax^2 + bxy + cy^2
Form gauss_reduce(long long a, long long b, long long c) {
    while (true) {
        if (a > c) { swap(a, c); }
        if (abs(b) > a) {
            // Size-reduce b
            long long k = (long long)round((double)b / (2.0 * a));
            c = c - k * b + k * k * a;
            b = b - 2 * k * a;
        } else {
            break;
        }
    }
    if (a > c) swap(a, c);
    return {a, b, c};
}

int main() {
    // Verify with known forms
    vector<tuple<int,int,int>> test_forms = {
        {3, 2, 5}, {7, 3, 4}, {10, 6, 10}, {1, 1, 1}
    };

    for (auto& [a, b, c] : test_forms) {
        Form reduced = gauss_reduce(a, b, c);
        cout << "Q = " << a << "x^2 + " << b << "xy + " << c << "y^2"
             << " -> shortest = " << reduced.a << endl;
    }

    // Compute sum for the family Q_n = n*x^2 + xy + n*y^2
    int N = 200;
    long long total = 0;
    for (int n = 1; n <= N; n++) {
        Form r = gauss_reduce(n, 1, n);
        total += r.a;
    }
    cout << "\nSum of shortest vectors for n=1.." << N << ": " << total << endl;

    return 0;
}
