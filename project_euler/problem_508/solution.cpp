#include <bits/stdc++.h>
using namespace std;

// Convert Gaussian integer (re + im*i) to base (i-1), return digit sum
int digit_sum_base_im1(long long re, long long im) {
    int s = 0;
    int iters = 0;
    while ((re != 0 || im != 0) && iters < 200) {
        // Digit = re mod 2 (in {0, 1})
        int d = ((re % 2) + 2) % 2;
        s += d;

        // Subtract digit and divide by (i-1)
        // (a+bi)/(i-1) = (-(a)+b)/2 + (-(a)-b)i/2
        long long a = re - d;
        long long new_re = (-a + im) / 2;
        long long new_im = (-a - im) / 2;
        re = new_re;
        im = new_im;
        iters++;
    }
    return s;
}

bool verify(long long re, long long im) {
    // Get digits
    vector<int> digits;
    long long r = re, i = im;
    int iters = 0;
    while ((r != 0 || i != 0) && iters < 200) {
        int d = ((r % 2) + 2) % 2;
        digits.push_back(d);
        long long a = r - d;
        long long nr = (-a + i) / 2;
        long long ni = (-a - i) / 2;
        r = nr;
        i = ni;
        iters++;
    }

    // Reconstruct: sum d_k * (i-1)^k
    long long sum_re = 0, sum_im = 0;
    long long pow_re = 1, pow_im = 0;
    for (int d : digits) {
        sum_re += d * pow_re;
        sum_im += d * pow_im;
        // (i-1)^(k+1) = (i-1) * (pow_re + pow_im*i)
        // = -(pow_re + pow_im) + (pow_re - pow_im)*i
        long long new_pr = -(pow_re + pow_im);
        long long new_pi = pow_re - pow_im;
        pow_re = new_pr;
        pow_im = new_pi;
    }
    return sum_re == re && sum_im == im;
}

int main() {
    // Verify
    bool all_ok = true;
    for (int a = -20; a <= 20; a++) {
        for (int b = -20; b <= 20; b++) {
            if (!verify(a, b)) {
                cout << "FAILED: " << a << " + " << b << "i" << endl;
                all_ok = false;
            }
        }
    }
    if (all_ok) cout << "All verifications passed." << endl;

    // Sum of digit sums for n = 1..N
    long long N = 1000;
    long long total = 0;
    for (long long n = 1; n <= N; n++) {
        total += digit_sum_base_im1(n, 0);
    }
    cout << "Sum of digit sums for n=1.." << N << ": " << total << endl;

    return 0;
}
