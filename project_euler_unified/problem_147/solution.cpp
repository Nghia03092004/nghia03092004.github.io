#include <bits/stdc++.h>
using namespace std;

// Count diagonal rectangles in a w x h grid.
// Uses doubled coordinates to avoid floating point.
// Two types of diagonal unit squares based on parity of center position.
// For each starting square, scan along rotated axes to enumerate all valid rectangles.
unsigned long long diagonal(int width, int height) {
    int a = width, b = height;
    if (a < b) swap(a, b);

    unsigned long long count = 0;
    for (int i = 0; i < a; i++)
        for (int j = 0; j < b; j++)
            for (int parity = 0; parity <= 1; parity++) {
                int startX = 2 * i + 1 + parity;
                int startY = 2 * j + 2 - parity;

                bool stop = false;
                int maxHeight = INT_MAX;

                for (int cw = 0; !stop; cw++) {
                    int currentX = startX + cw;
                    int currentY = startY - cw;
                    if (currentY <= 0) break;

                    for (int ch = 0; ch < maxHeight; ch++) {
                        int endX = currentX + ch;
                        int endY = currentY + ch;
                        if (endX >= 2 * a || endY >= 2 * b) {
                            if (maxHeight > ch) maxHeight = ch;
                            stop = (ch == 0);
                            break;
                        }
                        count++;
                    }
                }
            }
    return count;
}

int main() {
    int M = 47, N = 43;

    // Cache diagonal counts since diagonal(w,h) = diagonal(h,w)
    map<pair<int,int>, unsigned long long> cache;

    unsigned long long total = 0;
    for (int w = 1; w <= M; w++) {
        for (int h = 1; h <= N; h++) {
            // Axis-aligned rectangles in w x h grid
            unsigned long long axis = (unsigned long long)w * (w + 1) / 2 * h * (h + 1) / 2;

            // Diagonal rectangles (with caching)
            int a = min(w, h), b = max(w, h);
            auto key = make_pair(a, b);
            if (cache.find(key) == cache.end())
                cache[key] = diagonal(w, h);
            unsigned long long diag = cache[key];

            total += axis + diag;
        }
    }

    cout << total << endl;
    return 0;
}
