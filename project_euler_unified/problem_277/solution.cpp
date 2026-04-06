#include <bits/stdc++.h>
using namespace std;

int main(){
    // Modified Collatz sequence:
    // D: a -> a/3       (a % 3 == 0)
    // U: a -> (4a+2)/3  (a % 3 == 1)
    // d: a -> (2a-1)/3  (a % 3 == 2)
    //
    // Given sequence string, find least a1 > 10^15 producing it.
    //
    // Approach: work backwards from the end of the sequence.
    // After processing all steps, a1 must satisfy a1 ≡ r (mod M).
    // We build this by processing the sequence from right to left.
    //
    // If we know a_{k+1}, we can find a_k:
    // D: a_k = 3 * a_{k+1}        (and a_k % 3 == 0, always satisfied)
    // U: a_k = (3*a_{k+1} - 2)/4  (and a_k % 3 == 1)
    // d: a_k = (3*a_{k+1} + 1)/2  (and a_k % 3 == 2)
    //
    // We represent the constraint on a_1 as: a_1 ≡ r (mod M)
    // Starting from the end, a_{L+1} can be anything, so initially
    // a_{L+1} ≡ 0 (mod 1) (no constraint).
    //
    // Actually, let's think forward with rational arithmetic.
    // a_i = (num_i * a_1 + const_i) / den_i
    // The residue constraint a_i % 3 == required gives us info about a_1.
    //
    // Alternatively, use Python-style big integers via __int128 or just careful modular arithmetic.
    // Since we're dealing with modular constraints, let's track (r, M) where a_1 ≡ r (mod M).

    string seq = "UDDDUdddDDUDDddDdDddDDUDDdUUDd";

    // Forward approach:
    // After k steps, a_{k+1} = (alpha * a_1 + beta) where alpha, beta are rationals.
    // We represent alpha = anum/aden, beta = bnum/bden.
    // But to avoid fractions, track: a_{k+1} * D = A * a_1 + B (all integers).
    // Initially (before step 0): a_1 * 1 = 1 * a_1 + 0, so D=1, A=1, B=0.
    //
    // Step type determines:
    //   a_i = (A * a_1 + B) / D
    //   Required: a_i % 3 == req
    //   This means (A * a_1 + B) / D ≡ req (mod 3)
    //   i.e., A * a_1 + B ≡ req * D (mod 3*D)
    //   i.e., A * a_1 ≡ req * D - B (mod 3*D)
    //
    //   We need gcd(A, 3*D) | (req*D - B), then we can find a_1 mod (3*D/gcd).
    //   This combines with existing constraint a_1 ≡ r (mod M) via CRT.
    //
    // Then update for next step:
    //   D: a_{i+1} = a_i / 3 = (A*a_1 + B) / (3D)  => new D' = 3D, A'=A, B'=B
    //   U: a_{i+1} = (4*a_i + 2)/3 = (4(A*a_1+B)/D + 2)/3 = (4A*a_1 + 4B + 2D)/(3D)
    //      => D'=3D, A'=4A, B'=4B+2D
    //   d: a_{i+1} = (2*a_i - 1)/3 = (2(A*a_1+B)/D - 1)/3 = (2A*a_1 + 2B - D)/(3D)
    //      => D'=3D, A'=2A, B'=2B-D

    // We'll use __int128 to handle large numbers.
    // Actually, the modulus M after 31 steps could be up to 3^31 * 4^... which is huge.
    // Let's use Python for this. But the problem says C++.
    // With careful reduction, we can use long long or __int128.
    // 3^31 ~ 6.17 * 10^14, and with factors of 2 and 4, M could be larger.
    // Actually M divides 3^31 (since each step introduces a factor of 3 in denominator).
    // Wait, U multiplies A by 4, d multiplies A by 2. So A can grow as 4^(count_U) * 2^(count_d).
    // The constraint involves A * a_1 ≡ ... (mod 3*D).
    // D = 3^step (since each step multiplies D by 3).
    // A involves factors of 4 (for U) and 2 (for d) and 1 (for D).

    // Let me use __int128 for safety.
    typedef __int128 lll;

    auto gcd128 = [](lll a, lll b) -> lll {
        if (a < 0) a = -a;
        if (b < 0) b = -b;
        while (b) { a %= b; swap(a, b); }
        return a;
    };

    // Extended GCD
    function<lll(lll, lll, lll&, lll&)> extgcd = [&](lll a, lll b, lll &x, lll &y) -> lll {
        if (b == 0) { x = 1; y = 0; return a; }
        lll x1, y1;
        lll g = extgcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - (a / b) * y1;
        return g;
    };

    // CRT: combine a ≡ r1 (mod m1) and a ≡ r2 (mod m2)
    // Returns (r, m) or (-1, -1) if no solution
    auto crt = [&](lll r1, lll m1, lll r2, lll m2) -> pair<lll,lll> {
        lll g = gcd128(m1, m2);
        if ((r2 - r1) % g != 0) return {-1, -1};
        lll lcm = m1 / g * m2;
        lll x, y;
        extgcd(m1, m2, x, y);
        // solution: r1 + m1 * ((r2-r1)/g * x) mod lcm
        lll diff = (r2 - r1) / g;
        // We need to be careful with overflow. Use __int128.
        lll t = diff % (m2 / g) * (x % (m2 / g)) % (m2 / g);
        lll r = r1 + m1 * t;
        r = ((r % lcm) + lcm) % lcm;
        return {r, lcm};
    };

    lll D = 1, A = 1, B = 0;
    lll r = 0, M = 1; // a_1 ≡ r (mod M), initially no constraint

    for (char c : seq) {
        // a_i = (A * a_1 + B) / D
        // Determine required residue
        int req;
        if (c == 'D') req = 0;
        else if (c == 'U') req = 1;
        else req = 2; // 'd'

        // Constraint: (A * a_1 + B) ≡ req * D (mod 3 * D)
        // => A * a_1 ≡ req * D - B (mod 3 * D)
        lll rhs = (lll)req * D - B;
        lll mod = 3 * D;

        // Reduce: A * a_1 ≡ rhs (mod mod)
        lll g = gcd128(A, mod);
        // rhs must be divisible by g
        // (it should be by construction)
        lll A_red = A / g;
        lll rhs_red = rhs / g;
        lll mod_red = mod / g;

        // Solve A_red * a_1 ≡ rhs_red (mod mod_red)
        lll inv_x, inv_y;
        lll gg = extgcd(A_red, mod_red, inv_x, inv_y);
        // gg should be 1
        lll sol = (rhs_red % mod_red * (inv_x % mod_red) % mod_red + mod_red) % mod_red;

        // Combine with existing constraint via CRT
        auto [nr, nM] = crt(r, M, sol, mod_red);
        r = nr; M = nM;

        // Update A, B, D for next step
        if (c == 'D') {
            // D' = 3D, A' = A, B' = B
            D *= 3;
        } else if (c == 'U') {
            B = 4 * B + 2 * D;
            A = 4 * A;
            D *= 3;
        } else {
            B = 2 * B - D;
            A = 2 * A;
            D *= 3;
        }

        // Simplify by dividing out common factors of A, B, D
        lll common = gcd128(gcd128(A, B), D);
        if (common > 1) {
            A /= common;
            B /= common;
            D /= common;
        }
    }

    // Now a_1 ≡ r (mod M), find smallest a_1 > 10^15
    lll threshold = 1000000000000000LL; // 10^15
    lll answer;
    if (r > threshold) {
        answer = r;
    } else {
        lll k = (threshold + 1 - r + M - 1) / M;
        answer = r + k * M;
    }

    // Print answer (need to convert __int128 to string)
    auto to_string128 = [](lll x) -> string {
        if (x == 0) return "0";
        bool neg = false;
        if (x < 0) { neg = true; x = -x; }
        string s;
        while (x > 0) {
            s += ('0' + (int)(x % 10));
            x /= 10;
        }
        if (neg) s += '-';
        reverse(s.begin(), s.end());
        return s;
    };

    cout << to_string128(answer) << endl;
    return 0;
}
