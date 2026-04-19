#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

namespace {

const long double EPS = 1e-12L;
const long double INF = 1e100L;

struct Point {
    long double x;
    long double y;
};

Point operator+(const Point& a, const Point& b) {
    return {a.x + b.x, a.y + b.y};
}

Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y};
}

Point operator*(const Point& a, long double t) {
    return {a.x * t, a.y * t};
}

long double dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

long double cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

long double norm(const Point& a) {
    return sqrtl(dot(a, a));
}

int sgn(long double x) {
    if (x > EPS) {
        return 1;
    }
    if (x < -EPS) {
        return -1;
    }
    return 0;
}

bool point_on_segment(const Point& p, const Point& a, const Point& b) {
    return fabsl(cross(a - p, b - p)) <= 1e-10L && dot(a - p, b - p) <= 1e-10L;
}

// Returns 1 for inside, 0 for boundary, -1 for outside.
int point_location(const vector<Point>& poly, const Point& p) {
    int winding = 0;
    int n = static_cast<int>(poly.size());
    for (int i = 0; i < n; ++i) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];

        if (point_on_segment(p, a, b)) {
            return 0;
        }

        if (a.y <= p.y + EPS) {
            if (b.y > p.y + EPS && cross(b - a, p - a) > EPS) {
                ++winding;
            }
        } else {
            if (b.y <= p.y + EPS && cross(b - a, p - a) < -EPS) {
                --winding;
            }
        }
    }
    return winding == 0 ? -1 : 1;
}

bool segment_inside_polygon(const vector<Point>& poly, const Point& p, const Point& q) {
    vector<long double> parameters;
    parameters.push_back(0.0L);
    parameters.push_back(1.0L);

    Point d = q - p;
    long double dd = dot(d, d);
    int n = static_cast<int>(poly.size());

    for (int i = 0; i < n; ++i) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];
        Point edge = b - a;
        Point ap = a - p;
        long double den = cross(d, edge);

        if (fabsl(den) <= EPS) {
            if (fabsl(cross(d, ap)) <= EPS && dd > EPS) {
                long double ta = dot(a - p, d) / dd;
                long double tb = dot(b - p, d) / dd;
                if (ta > -EPS && ta < 1.0L + EPS) {
                    parameters.push_back(max(0.0L, min(1.0L, ta)));
                }
                if (tb > -EPS && tb < 1.0L + EPS) {
                    parameters.push_back(max(0.0L, min(1.0L, tb)));
                }
            }
            continue;
        }

        long double t = cross(ap, edge) / den;
        long double u = cross(ap, d) / den;
        if (t > -EPS && t < 1.0L + EPS && u > -EPS && u < 1.0L + EPS) {
            parameters.push_back(max(0.0L, min(1.0L, t)));
        }
    }

    sort(parameters.begin(), parameters.end());
    vector<long double> unique_parameters;
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (unique_parameters.empty() || fabsl(parameters[i] - unique_parameters.back()) > 1e-11L) {
            unique_parameters.push_back(parameters[i]);
        }
    }

    for (size_t i = 0; i + 1 < unique_parameters.size(); ++i) {
        long double l = unique_parameters[i];
        long double r = unique_parameters[i + 1];
        if (r - l < 1e-11L) {
            continue;
        }
        Point mid = p + d * ((l + r) * 0.5L);
        if (point_location(poly, mid) < 0) {
            return false;
        }
    }

    for (int i = 0; i < n; ++i) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];
        Point edge = b - a;
        Point ap = a - p;
        long double den = cross(d, edge);
        if (fabsl(den) <= EPS) {
            continue;
        }
        long double t = cross(ap, edge) / den;
        long double u = cross(ap, d) / den;
        if (t > EPS && t < 1.0L - EPS && u > EPS && u < 1.0L - EPS) {
            return false;
        }
    }

    return true;
}

long double ray_cutoff(const vector<Point>& poly, const Point& sculpture, const Point& direction) {
    int n = static_cast<int>(poly.size());
    long double dd = dot(direction, direction);

    auto parameter_on_ray = [&](const Point& p) {
        return dot(direction, p - sculpture) / dd;
    };

    long double both_sides = INF;
    long double positive_side = INF;
    long double negative_side = INF;

    for (int i = 0; i < n; ++i) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];
        int sa = sgn(cross(direction, a - sculpture));
        int sb = sgn(cross(direction, b - sculpture));
        if (sa == 0 || sb == 0) {
            continue;
        }
        if (sa != sb) {
            Point edge = b - a;
            long double den = cross(direction, edge);
            long double t = cross(a - sculpture, edge) / den;
            if (t > EPS) {
                both_sides = min(both_sides, t);
            }
        }
    }

    vector<int> on_ray(n, 0);
    vector<long double> ray_parameter(n, 0.0L);
    for (int i = 0; i < n; ++i) {
        long double t = parameter_on_ray(poly[i]);
        if (sgn(cross(direction, poly[i] - sculpture)) == 0 && t > EPS) {
            on_ray[i] = 1;
            ray_parameter[i] = t;
        }
    }

    for (int i = 0; i < n; ++i) {
        if (!on_ray[i]) {
            continue;
        }
        int left = (i - 1 + n) % n;
        while (on_ray[left]) {
            left = (left - 1 + n) % n;
        }
        int right = (i + 1) % n;
        while (on_ray[right]) {
            right = (right + 1) % n;
        }

        int sl = sgn(cross(direction, poly[left] - sculpture));
        int sr = sgn(cross(direction, poly[right] - sculpture));
        long double t = ray_parameter[i];

        if (sl != sr) {
            both_sides = min(both_sides, t);
        } else if (sl > 0) {
            positive_side = min(positive_side, t);
        } else {
            negative_side = min(negative_side, t);
        }
    }

    long double left_block = min(both_sides, positive_side);
    long double right_block = min(both_sides, negative_side);
    return max(left_block, right_block);
}

struct Segment {
    Point a;
    Point b;
};

long double distance_point_to_segment(const Point& p, const Segment& segment) {
    Point d = segment.b - segment.a;
    long double dd = dot(d, d);
    if (dd <= EPS) {
        return norm(p - segment.a);
    }
    long double t = dot(p - segment.a, d) / dd;
    t = max(0.0L, min(1.0L, t));
    Point projection = segment.a + d * t;
    return norm(p - projection);
}

long double visible_distance_to_segment(const vector<Point>& poly, const Point& from, const Segment& segment) {
    vector<long double> events;
    events.push_back(0.0L);
    events.push_back(1.0L);

    Point dir = segment.b - segment.a;
    for (size_t i = 0; i < poly.size(); ++i) {
        Point to_vertex = poly[i] - from;
        long double den = cross(dir, to_vertex);
        long double num = cross(segment.a - from, to_vertex);

        if (fabsl(den) <= EPS) {
            if (fabsl(num) <= EPS) {
                long double dd = dot(dir, dir);
                if (dd > EPS) {
                    long double t = dot(poly[i] - segment.a, dir) / dd;
                    if (t > -EPS && t < 1.0L + EPS) {
                        events.push_back(max(0.0L, min(1.0L, t)));
                    }
                }
            }
            continue;
        }

        long double t = -num / den;
        if (t > -EPS && t < 1.0L + EPS) {
            events.push_back(max(0.0L, min(1.0L, t)));
        }
    }

    sort(events.begin(), events.end());
    vector<long double> unique_events;
    for (size_t i = 0; i < events.size(); ++i) {
        if (unique_events.empty() || fabsl(events[i] - unique_events.back()) > 1e-11L) {
            unique_events.push_back(events[i]);
        }
    }

    long double best = INF;

    for (size_t i = 0; i < unique_events.size(); ++i) {
        Point candidate = segment.a + dir * unique_events[i];
        if (segment_inside_polygon(poly, from, candidate)) {
            best = min(best, norm(candidate - from));
        }
    }

    for (size_t i = 0; i + 1 < unique_events.size(); ++i) {
        long double l = unique_events[i];
        long double r = unique_events[i + 1];
        if (r - l < 1e-11L) {
            continue;
        }

        Point mid = segment.a + dir * ((l + r) * 0.5L);
        if (!segment_inside_polygon(poly, from, mid)) {
            continue;
        }

        Segment visible_part = {segment.a + dir * l, segment.a + dir * r};
        best = min(best, distance_point_to_segment(from, visible_part));
    }

    return best;
}

long long gcd_ll(long long a, long long b) {
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

void solve() {
    int n;
    cin >> n;

    vector<Point> polygon(n);
    vector<pair<int, int> > polygon_int(n);
    for (int i = 0; i < n; ++i) {
        cin >> polygon_int[i].first >> polygon_int[i].second;
        polygon[i] = {static_cast<long double>(polygon_int[i].first),
                      static_cast<long double>(polygon_int[i].second)};
    }

    Point guard;
    Point sculpture;
    cin >> guard.x >> guard.y;
    cin >> sculpture.x >> sculpture.y;

    map<pair<int, int>, pair<long double, Point> > best_segment;
    for (int i = 0; i < n; ++i) {
        long long dx = polygon_int[i].first - static_cast<long long>(llround(sculpture.x));
        long long dy = polygon_int[i].second - static_cast<long long>(llround(sculpture.y));
        long long g = gcd_ll(llabs(dx), llabs(dy));
        dx /= g;
        dy /= g;

        Point direction = polygon[i] - sculpture;
        long double cutoff = ray_cutoff(polygon, sculpture, direction);
        Point endpoint = sculpture + direction * cutoff;
        long double projection = dot(endpoint - sculpture,
                                     {static_cast<long double>(dx), static_cast<long double>(dy)});

        pair<int, int> key = {static_cast<int>(dx), static_cast<int>(dy)};
        if (best_segment.find(key) == best_segment.end() ||
            projection > best_segment[key].first + 1e-10L) {
            best_segment[key] = {projection, endpoint};
        }
    }

    vector<Segment> target_segments;
    for (map<pair<int, int>, pair<long double, Point> >::const_iterator it = best_segment.begin();
         it != best_segment.end(); ++it) {
        target_segments.push_back({sculpture, it->second.second});
    }

    vector<Point> nodes;
    nodes.push_back(guard);
    for (int i = 0; i < n; ++i) {
        nodes.push_back(polygon[i]);
    }

    int node_count = static_cast<int>(nodes.size());
    vector<vector<pair<int, long double> > > graph(node_count);
    for (int i = 0; i < node_count; ++i) {
        for (int j = i + 1; j < node_count; ++j) {
            if (segment_inside_polygon(polygon, nodes[i], nodes[j])) {
                long double w = norm(nodes[i] - nodes[j]);
                graph[i].push_back({j, w});
                graph[j].push_back({i, w});
            }
        }
    }

    vector<long double> dist(node_count, INF);
    vector<int> used(node_count, 0);
    dist[0] = 0.0L;
    for (int iter = 0; iter < node_count; ++iter) {
        int best = -1;
        for (int i = 0; i < node_count; ++i) {
            if (!used[i] && (best == -1 || dist[i] < dist[best])) {
                best = i;
            }
        }
        if (best == -1 || dist[best] >= INF / 2) {
            break;
        }
        used[best] = 1;
        for (size_t i = 0; i < graph[best].size(); ++i) {
            int to = graph[best][i].first;
            long double w = graph[best][i].second;
            if (dist[to] > dist[best] + w) {
                dist[to] = dist[best] + w;
            }
        }
    }

    long double answer = INF;

    if (ray_cutoff(polygon, sculpture, guard - sculpture) >= 1.0L - 1e-11L) {
        answer = 0.0L;
    }

    for (int i = 0; i < node_count; ++i) {
        if (dist[i] >= INF / 2) {
            continue;
        }

        if (ray_cutoff(polygon, sculpture, nodes[i] - sculpture) >= 1.0L - 1e-11L) {
            answer = min(answer, dist[i]);
        }

        for (size_t j = 0; j < target_segments.size(); ++j) {
            long double last_leg = visible_distance_to_segment(polygon, nodes[i], target_segments[j]);
            if (last_leg < INF / 2) {
                answer = min(answer, dist[i] + last_leg);
            }
        }
    }

    cout << setprecision(15) << static_cast<double>(answer) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
