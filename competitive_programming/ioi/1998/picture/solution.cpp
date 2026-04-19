// IOI 1998 - Picture
// Total perimeter of union of rectangles via line sweep
// Sweep vertically for horizontal edges, horizontally for vertical edges
// Time: O(n^2), Space: O(n)
#include <bits/stdc++.h>
using namespace std;

struct Rect {
    int x1, y1, x2, y2;
};

struct Event {
    int y, x1, x2, type; // type: +1 = open, -1 = close
    bool operator<(const Event& o) const {
        if (y != o.y) return y < o.y;
        return type < o.type;
    }
};

int n;
Rect rects[5001];

// Compute horizontal perimeter contribution by sweeping vertically
long long sweepHorizontal() {
    vector<Event> events;
    for (int i = 0; i < n; i++) {
        events.push_back({rects[i].y1, rects[i].x1, rects[i].x2, +1});
        events.push_back({rects[i].y2, rects[i].x1, rects[i].x2, -1});
    }
    sort(events.begin(), events.end());

    // Coordinate compress x values
    vector<int> xs;
    for (int i = 0; i < n; i++) {
        xs.push_back(rects[i].x1);
        xs.push_back(rects[i].x2);
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    int M = (int)xs.size();

    vector<int> cnt(M, 0);
    long long perimeter = 0;
    int prevCovered = 0;

    int i = 0;
    while (i < (int)events.size()) {
        int curY = events[i].y;
        while (i < (int)events.size() && events[i].y == curY) {
            int x1 = events[i].x1, x2 = events[i].x2, t = events[i].type;
            for (int j = 0; j + 1 < M; j++)
                if (xs[j] >= x1 && xs[j + 1] <= x2)
                    cnt[j] += t;
            i++;
        }

        int covered = 0;
        for (int j = 0; j + 1 < M; j++)
            if (cnt[j] > 0)
                covered += xs[j + 1] - xs[j];

        perimeter += abs(covered - prevCovered);
        prevCovered = covered;
    }

    return perimeter;
}

// Compute vertical perimeter by swapping x/y and reusing horizontal sweep
long long sweepVertical() {
    Rect orig[5001];
    for (int i = 0; i < n; i++) orig[i] = rects[i];
    for (int i = 0; i < n; i++) {
        rects[i].x1 = orig[i].y1;
        rects[i].x2 = orig[i].y2;
        rects[i].y1 = orig[i].x1;
        rects[i].y2 = orig[i].x2;
    }
    long long result = sweepHorizontal();
    for (int i = 0; i < n; i++) rects[i] = orig[i];
    return result;
}

int main() {
    scanf("%d", &n);
    if (n == 0) {
        printf("0\n");
        return 0;
    }
    for (int i = 0; i < n; i++)
        scanf("%d %d %d %d", &rects[i].x1, &rects[i].y1,
              &rects[i].x2, &rects[i].y2);

    printf("%lld\n", sweepHorizontal() + sweepVertical());
    return 0;
}
