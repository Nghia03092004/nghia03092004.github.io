#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace {

struct Coordinate {
    int x;
    int y;
};

int main_impl() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int dx, dy;
    cin >> dx >> dy;

    vector<string> raw(dy);
    for (int row = 0; row < dy; ++row) {
        cin >> raw[row];
    }

    vector<Coordinate> markers;
    for (int row = 0; row < dy; ++row) {
        int y = dy - row;
        for (int x = 1; x <= dx; ++x) {
            if (raw[row][x - 1] == 'X') {
                markers.push_back({x, y});
            }
        }
    }

    const int radius = max(dx - 1, dy - 1);
    const int side = 2 * radius + 1;
    const int sequence_length = side * side;
    const int word_count = (sequence_length + 63) / 64;

    vector<int> visit_time(side * side, -1);
    auto offset_index = [radius, side](int x, int y) {
        return (y + radius) * side + (x + radius);
    };

    int covered = 0;
    int x = 0;
    int y = 0;
    int time = 0;
    visit_time[offset_index(0, 0)] = 0;
    covered = 1;

    const int dir_x[4] = {0, 1, 0, -1};
    const int dir_y[4] = {1, 0, -1, 0};
    for (int step_length = 1, dir = 0; covered < sequence_length; ++step_length) {
        for (int repeat = 0; repeat < 2; ++repeat, dir = (dir + 1) % 4) {
            for (int step = 0; step < step_length; ++step) {
                x += dir_x[dir];
                y += dir_y[dir];
                ++time;
                if (-radius <= x && x <= radius && -radius <= y && y <= radius) {
                    visit_time[offset_index(x, y)] = time;
                    ++covered;
                    if (covered == sequence_length) {
                        break;
                    }
                }
            }
            if (covered == sequence_length) {
                break;
            }
        }
    }

    const int starts = dx * dy;
    vector<uint64_t> sequences(static_cast<size_t>(starts) * word_count, 0ULL);
    auto set_bit = [&](int id, int pos) {
        sequences[static_cast<size_t>(id) * word_count + pos / 64] |= 1ULL << (pos % 64);
    };
    auto get_word = [&](int id, int word) -> uint64_t {
        return sequences[static_cast<size_t>(id) * word_count + word];
    };

    for (int sy = 1; sy <= dy; ++sy) {
        for (int sx = 1; sx <= dx; ++sx) {
            int id = (sy - 1) * dx + (sx - 1);
            for (size_t i = 0; i < markers.size(); ++i) {
                int rx = markers[i].x - sx;
                int ry = markers[i].y - sy;
                int pos = visit_time[offset_index(rx, ry)];
                set_bit(id, pos);
            }
        }
    }

    vector<int> order(starts);
    iota(order.begin(), order.end(), 0);

    auto sequence_less = [&](int a, int b) {
        for (int word = 0; word < word_count; ++word) {
            uint64_t wa = get_word(a, word);
            uint64_t wb = get_word(b, word);
            if (wa != wb) {
                uint64_t diff = wa ^ wb;
                int bit = __builtin_ctzll(diff);
                return ((wa >> bit) & 1ULL) < ((wb >> bit) & 1ULL);
            }
        }
        return false;
    };

    sort(order.begin(), order.end(), sequence_less);

    auto lcp = [&](int a, int b) {
        int shared = 0;
        for (int word = 0; word < word_count; ++word) {
            uint64_t wa = get_word(a, word);
            uint64_t wb = get_word(b, word);
            if (wa == wb) {
                shared += 64;
                continue;
            }
            uint64_t diff = wa ^ wb;
            shared += __builtin_ctzll(diff);
            break;
        }
        if (shared > sequence_length) {
            shared = sequence_length;
        }
        return shared;
    };

    vector<int> answer(starts, 0);
    for (int i = 0; i < starts; ++i) {
        int id = order[i];
        int best = 0;
        if (i > 0) {
            best = max(best, lcp(id, order[i - 1]));
        }
        if (i + 1 < starts) {
            best = max(best, lcp(id, order[i + 1]));
        }
        answer[id] = best;
    }

    double expected = 0.0;
    int maximum = 0;
    for (int id = 0; id < starts; ++id) {
        expected += answer[id];
        maximum = max(maximum, answer[id]);
    }
    expected /= starts;

    cout << fixed << setprecision(3) << expected << '\n';
    cout << maximum << '\n';

    bool first = true;
    for (int sy = 1; sy <= dy; ++sy) {
        for (int sx = 1; sx <= dx; ++sx) {
            int id = (sy - 1) * dx + (sx - 1);
            if (answer[id] == maximum) {
                if (!first) {
                    cout << ' ';
                }
                first = false;
                cout << '(' << sx << ',' << sy << ')';
            }
        }
    }
    cout << '\n';

    return 0;
}

}  // namespace

int main() {
    return main_impl();
}
