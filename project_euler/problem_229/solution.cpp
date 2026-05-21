#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

const unsigned int SliceSize = 1000 * 1000;
const unsigned char One = 1 << 0;
const unsigned char Two = 1 << 1;
const unsigned char Three = 1 << 2;
const unsigned char Seven = 1 << 3;
const unsigned char All = One | Two | Three | Seven;

int main() {
    const unsigned int limit = 2000000000U;
    const unsigned int exclusive = limit + 1;
    const unsigned int maxA = (unsigned int)sqrt((double)limit);

    vector<unsigned int> b1(maxA + 1, 1);
    vector<unsigned int> b2(maxA + 1, 1);
    vector<unsigned int> b3(maxA + 1, 1);
    vector<unsigned int> b7(maxA + 1, 1);

    unsigned int count = 0;
    unsigned int from = 0;

    while (from < exclusive) {
        unsigned int to = from + SliceSize;
        if (to > exclusive) {
            to = exclusive;
        }

        vector<unsigned char> used(to - from, 0);

        for (unsigned int a = 1; (uint64_t)a * a + (uint64_t)b1[a] * b1[a] < to; a++) {
            unsigned int aa = a * a;

            unsigned int b = b1[a];
            for (; aa + (uint64_t)b * b < to; b++) {
                used[aa + b * b - from] |= One;
            }
            b1[a] = b;

            b = b2[a];
            for (; aa + 2ULL * b * b < to; b++) {
                used[aa + 2U * b * b - from] |= Two;
            }
            b2[a] = b;

            b = b3[a];
            for (; aa + 3ULL * b * b < to; b++) {
                used[aa + 3U * b * b - from] |= Three;
            }
            b3[a] = b;

            b = b7[a];
            for (; aa + 7ULL * b * b < to; b++) {
                used[aa + 7U * b * b - from] |= Seven;
            }
            b7[a] = b;
        }

        for (unsigned char value : used) {
            if (value == All) {
                count++;
            }
        }

        from = to;
    }

    cout << count << '\n';
    return 0;
}
