#include <bits/stdc++.h>
using namespace std;
int main() {
    // P(G,k) = k^10 - 15k^9 + 105k^8 - 455k^7 + 1360k^6 - 2942k^5
    //          + 4635k^4 - 5275k^3 + 4120k^2 - 2040k + 462
    long long k = 4;
    long long coeffs[] = {1,-15,105,-455,1360,-2942,4635,-5275,4120,-2040,462};
    long long result = 0, pk = 1;
    for (int i = 10; i >= 0; i--) {
        // Horner's method
    }
    // Direct computation
    result = 1;
    for (int i = 0; i < 11; i++) {
        long long term = coeffs[i];
        for (int j = 0; j < 10-i; j++) term *= k;
        result += (i == 0 ? 0 : term);  // first term already counted
    }
    // Simpler: just evaluate
    result = 0;
    long long kp = 1;
    for (int i = 10; i >= 0; i--) {
        result += coeffs[10-i] * kp;
        kp *= k;
    }
    cout << result << endl;
    return 0;
}
