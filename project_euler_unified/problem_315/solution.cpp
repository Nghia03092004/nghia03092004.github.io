#include <bits/stdc++.h>
using namespace std;

// Problem 315: Digital Root Clocks
// For each prime p in (10^7, 2*10^7), compute Sam(p) - Max(p)
// where Sam and Max display the digital root chain on 7-segment displays.
// Sam turns everything off then on; Max only changes differing segments.
// Difference = 2 * (common segments at each transition).

int main() {
    const int LO = 10000000;   // 10^7
    const int HI = 20000000;   // 2*10^7

    // Segment bitmasks for digits 0-9 (matching the Project Euler display)
    // Bits: a=0(top), b=1(top-right), c=2(bot-right), d=3(bot), e=4(bot-left), f=5(top-left), g=6(mid)
    // Key: digit 7 has 4 segments (a,b,c,f) including top-left vertical
    // 0:a,b,c,d,e,f(6) 1:b,c(2) 2:a,b,d,e,g(5) 3:a,b,c,d,g(5) 4:b,c,f,g(4)
    // 5:a,c,d,f,g(5) 6:a,c,d,e,f,g(6) 7:a,b,c,f(4) 8:all(7) 9:a,b,c,d,f,g(6)
    const int seg[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f};

    // Precompute segment count and common segments between digits
    int seg_count[10];
    int common[10][10];
    for (int i = 0; i < 10; i++) {
        seg_count[i] = __builtin_popcount(seg[i]);
        for (int j = 0; j < 10; j++) {
            common[i][j] = __builtin_popcount(seg[i] & seg[j]);
        }
    }

    // Function to compute total segments for a number
    auto total_segs = [&](int n) -> int {
        if (n == 0) return seg_count[0];
        int s = 0;
        while (n > 0) {
            s += seg_count[n % 10];
            n /= 10;
        }
        return s;
    };

    // Function to compute common segments between two numbers
    // (digit by digit, right-aligned; missing digits have 0 common)
    auto common_segs = [&](int a, int b) -> int {
        int s = 0;
        while (a > 0 && b > 0) {
            s += common[a % 10][b % 10];
            a /= 10;
            b /= 10;
        }
        return s;
    };

    // Function to compute digit sum
    auto digit_sum = [](int n) -> int {
        int s = 0;
        while (n > 0) {
            s += n % 10;
            n /= 10;
        }
        return s;
    };

    // Sieve of Eratosthenes
    vector<bool> is_composite(HI, false);
    for (int i = 2; (long long)i * i < HI; i++) {
        if (!is_composite[i]) {
            for (int j = i * i; j < HI; j += i) {
                is_composite[j] = true;
            }
        }
    }

    long long answer = 0;

    for (int p = LO + 1; p < HI; p++) {
        if (is_composite[p]) continue;

        // Compute the digital root chain and accumulate 2 * common segments
        // Chain: p -> digit_sum(p) -> digit_sum(digit_sum(p)) -> ... -> single digit

        int current = p;
        while (current >= 10) {
            int next = digit_sum(current);
            answer += 2 * common_segs(current, next);
            current = next;
        }
        // The chain also includes the initial turn-on and final turn-off,
        // but these are the same for Sam and Max, so they don't contribute
        // to the difference.
    }

    cout << answer << endl;

    return 0;
}
