#include <bits/stdc++.h>

using namespace std;

void send(int value);
void output(int value);

namespace {

constexpr int kAlphabet = 257;  // 0..255 plus one blank symbol.
constexpr int kMaxMessageBytes = 64;
constexpr int kMaxEncodedLength = 261;
constexpr int kMaxChooseN = 256 + kMaxEncodedLength;
constexpr int kLimbs = 10;

struct BigInt {
    array<uint64_t, kLimbs> limb{};

    BigInt(uint64_t value = 0) {
        limb.fill(0);
        limb[0] = value;
    }

    bool operator<(const BigInt& other) const {
        for (int i = kLimbs - 1; i >= 0; --i) {
            if (limb[i] != other.limb[i]) {
                return limb[i] < other.limb[i];
            }
        }
        return false;
    }

    BigInt& operator+=(const BigInt& other) {
        unsigned __int128 carry = 0;
        for (int i = 0; i < kLimbs; ++i) {
            unsigned __int128 sum = static_cast<unsigned __int128>(limb[i]) +
                                    other.limb[i] + carry;
            limb[i] = static_cast<uint64_t>(sum);
            carry = sum >> 64;
        }
        return *this;
    }

    BigInt& operator-=(const BigInt& other) {
        uint64_t borrow = 0;
        for (int i = 0; i < kLimbs; ++i) {
            uint64_t old = limb[i];
            uint64_t sub = other.limb[i] + borrow;
            limb[i] = old - sub;
            borrow = (old < sub) || (borrow && sub == 0);
        }
        return *this;
    }

    void add_small(uint64_t value) {
        unsigned __int128 carry = value;
        for (int i = 0; i < kLimbs && carry > 0; ++i) {
            unsigned __int128 sum = static_cast<unsigned __int128>(limb[i]) + carry;
            limb[i] = static_cast<uint64_t>(sum);
            carry = sum >> 64;
        }
    }

    void shift_left_8() {
        uint64_t carry = 0;
        for (int i = 0; i < kLimbs; ++i) {
            uint64_t next = limb[i] >> 56;
            limb[i] = (limb[i] << 8) | carry;
            carry = next;
        }
    }

    void shift_right_8() {
        uint64_t carry = 0;
        for (int i = kLimbs - 1; i >= 0; --i) {
            uint64_t next = limb[i] << 56;
            limb[i] = (limb[i] >> 8) | carry;
            carry = next;
        }
    }

    int low_byte() const {
        return static_cast<int>(limb[0] & 255);
    }
};

bool initialized = false;
vector<vector<BigInt>> binom;
array<int, kMaxMessageBytes + 1> best_length{};

void init_tables() {
    if (initialized) {
        return;
    }
    initialized = true;

    binom.assign(kMaxChooseN + 1, vector<BigInt>(257, BigInt(0)));
    binom[0][0] = 1;
    for (int n = 1; n <= kMaxChooseN; ++n) {
        binom[n][0] = 1;
        for (int k = 1; k <= min(n, 256); ++k) {
            binom[n][k] = binom[n - 1][k - 1];
            binom[n][k] += binom[n - 1][k];
        }
    }

    best_length[0] = 0;
    BigInt states = 1;
    int length = 0;
    for (int n = 1; n <= kMaxMessageBytes; ++n) {
        states.shift_left_8();
        while (binom[length + 256][256] < states) {
            ++length;
        }
        best_length[n] = length;
    }
}

BigInt message_to_rank(int N, const int M[]) {
    BigInt value = 0;
    for (int i = N - 1; i >= 0; --i) {
        value.shift_left_8();
        value.add_small(M[i]);
    }
    return value;
}

vector<int> unrank_counts(int total, BigInt rank) {
    vector<int> counts(kAlphabet, 0);
    int remaining = total;
    for (int symbol = 0; symbol + 1 < kAlphabet; ++symbol) {
        int tail = kAlphabet - symbol - 2;
        for (int cnt = 0; cnt <= remaining; ++cnt) {
            const BigInt& ways = binom[remaining - cnt + tail][tail];
            if (rank < ways) {
                counts[symbol] = cnt;
                remaining -= cnt;
                break;
            }
            rank -= ways;
        }
    }
    counts.back() = remaining;
    return counts;
}

BigInt rank_counts(const vector<int>& counts) {
    BigInt rank = 0;
    int remaining = accumulate(counts.begin(), counts.end(), 0);
    for (int symbol = 0; symbol + 1 < kAlphabet; ++symbol) {
        int tail = kAlphabet - symbol - 2;
        for (int cnt = 0; cnt < counts[symbol]; ++cnt) {
            rank += binom[remaining - cnt + tail][tail];
        }
        remaining -= counts[symbol];
    }
    return rank;
}

}  // namespace

void encode(int N, int M[]) {
    init_tables();

    int total = best_length[N];
    BigInt rank = message_to_rank(N, M);
    vector<int> counts = unrank_counts(total, rank);

    for (int value = 0; value < 256; ++value) {
        for (int rep = 0; rep < counts[value]; ++rep) {
            send(value);
        }
    }
}

void decode(int N, int L, int X[]) {
    init_tables();

    int total = best_length[N];
    vector<int> counts(kAlphabet, 0);
    for (int i = 0; i < L; ++i) {
        ++counts[X[i]];
    }
    counts.back() = total - L;

    BigInt rank = rank_counts(counts);
    for (int i = 0; i < N; ++i) {
        output(rank.low_byte());
        rank.shift_right_8();
    }
}
