#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int MONTHS = 12;
constexpr int YEAR_DAYS = 365;
const int MONTH_LEN[MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int day_of_year(int month, int day) {
    int ans = 0;
    for (int i = 0; i < month - 1; ++i) {
        ans += MONTH_LEN[i];
    }
    return ans + day - 1;
}

int parse_date(const string& s) {
    const int year = stoi(s.substr(0, 4));
    const int month = stoi(s.substr(5, 2));
    const int day = stoi(s.substr(8, 2));
    return year * YEAR_DAYS + day_of_year(month, day);
}

string format_date(int serial) {
    const int year = serial / YEAR_DAYS;
    int rem = serial % YEAR_DAYS;
    int month = 1;
    while (rem >= MONTH_LEN[month - 1]) {
        rem -= MONTH_LEN[month - 1];
        ++month;
    }
    const int day = rem + 1;

    ostringstream out;
    out << setw(4) << setfill('0') << year << '-'
        << setw(2) << setfill('0') << month << '-'
        << setw(2) << setfill('0') << day;
    return out.str();
}

void solve() {
    int n, y, d;
    cin >> n >> y >> d;
    const int max_absent = YEAR_DAYS - d;
    const int history_shift = YEAR_DAYS * y;

    vector<pair<int, int>> trips(n);
    int last_day = 0;
    for (int i = 0; i < n; ++i) {
        string l, r;
        cin >> l >> r;
        trips[i] = {parse_date(l) + history_shift, parse_date(r) + history_shift};
        last_day = max(last_day, trips[i].second);
    }

    const int limit = last_day + YEAR_DAYS * (y + 1);
    vector<int> diff(limit + 2, 0);
    for (const auto& trip : trips) {
        ++diff[trip.first];
        --diff[trip.second + 1];
    }

    vector<int> absent(limit + 1, 0);
    vector<int> pref(limit + 2, 0);
    int cur = 0;
    for (int day = 0; day <= limit; ++day) {
        cur += diff[day];
        absent[day] = (cur > 0 ? 1 : 0);
        pref[day + 1] = pref[day] + absent[day];
    }

    int answer = limit;
    for (int residue = 0; residue < YEAR_DAYS; ++residue) {
        int run = 0;
        for (int block = 0; residue + block * YEAR_DAYS + YEAR_DAYS - 1 <= limit; ++block) {
            const int start = residue + block * YEAR_DAYS;
            const int finish = start + YEAR_DAYS - 1;
            const int days_absent = pref[finish + 1] - pref[start];
            if (days_absent <= max_absent) {
                ++run;
            } else {
                run = 0;
            }

            if (run >= y) {
                const int apply_day = residue + (block + 1) * YEAR_DAYS;
                if (apply_day > last_day) {
                    answer = min(answer, apply_day);
                    break;
                }
            }
        }
    }

    cout << format_date(answer - history_shift) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
