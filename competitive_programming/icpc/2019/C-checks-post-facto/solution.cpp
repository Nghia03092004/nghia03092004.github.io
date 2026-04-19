#include <bits/stdc++.h>
using namespace std;

namespace {

struct Move {
    bool is_jump;
    vector<int> seq;
};

struct Piece {
    int color;   // 0 = black, 1 = white
    bool king;
    int origin;
};

enum class ActionType {
    success,
    add_start_piece,
    add_captured_piece,
    upgrade_origin,
    need_blocker,
    fail,
};

struct SimResult {
    ActionType type = ActionType::fail;
    int square = -1;
    int color = -1;
    int origin = -1;
    array<char, 32> final_board{};
};

int sq_row[32], sq_col[32];
int board_id[8][8];

bool is_king(char c) {
    return c == 'B' || c == 'W';
}

int piece_color(char c) {
    return (c == 'b' || c == 'B') ? 0 : 1;
}

char king_char(int color) {
    return color == 0 ? 'B' : 'W';
}

char man_char(int color) {
    return color == 0 ? 'b' : 'w';
}

bool man_allowed(int color, int sq) {
    int row = sq_row[sq];
    if (color == 0) {
        return row != 7;
    }
    return row != 0;
}

char minimal_piece(int color, int sq) {
    if (man_allowed(color, sq)) {
        return man_char(color);
    }
    return king_char(color);
}

bool same_color(char a, char b) {
    return piece_color(a) == piece_color(b);
}

bool insert_piece(array<char, 32>& start, int sq, char need, bool require_change) {
    char& cur = start[sq];
    if (cur == '.') {
        cur = need;
        return true;
    }
    if (!same_color(cur, need)) {
        return false;
    }
    if (is_king(need) && !is_king(cur)) {
        cur = need;
        return true;
    }
    return !require_change;
}

bool upgrade_origin(array<char, 32>& start, int origin, int color) {
    return insert_piece(start, origin, king_char(color), false);
}

bool in_bounds(int r, int c) {
    return 0 <= r && r < 8 && 0 <= c && c < 8;
}

int other_color(int color) {
    return color ^ 1;
}

bool promotion_row(int color, int sq) {
    int row = sq_row[sq];
    return color == 0 ? row == 7 : row == 0;
}

vector<pair<int, int> > jump_options_from(
    int sq,
    const array<int, 32>& board,
    const vector<Piece>& pieces
) {
    vector<pair<int, int> > result;
    int id = board[sq];
    if (id == -1) {
        return result;
    }
    const Piece& piece = pieces[id];
    static const int dr_all[4] = {1, 1, -1, -1};
    static const int dc_all[4] = {1, -1, 1, -1};
    for (int dir = 0; dir < 4; ++dir) {
        int dr = dr_all[dir];
        int dc = dc_all[dir];
        if (!piece.king) {
            if (piece.color == 0 && dr != 1) {
                continue;
            }
            if (piece.color == 1 && dr != -1) {
                continue;
            }
        }
        int mr = sq_row[sq] + dr;
        int mc = sq_col[sq] + dc;
        int tr = sq_row[sq] + 2 * dr;
        int tc = sq_col[sq] + 2 * dc;
        if (!in_bounds(tr, tc) || !in_bounds(mr, mc)) {
            continue;
        }
        int mid = board_id[mr][mc];
        int to = board_id[tr][tc];
        if (mid == -1 || to == -1) {
            continue;
        }
        if (board[to] != -1) {
            continue;
        }
        if (board[mid] == -1) {
            continue;
        }
        if (pieces[board[mid]].color == piece.color) {
            continue;
        }
        result.push_back(make_pair(mid, to));
    }
    return result;
}

pair<int, int> first_jump_for_player(
    int color,
    const array<int, 32>& board,
    const vector<Piece>& pieces
) {
    for (int sq = 0; sq < 32; ++sq) {
        if (board[sq] == -1 || pieces[board[sq]].color != color) {
            continue;
        }
        vector<pair<int, int> > opts = jump_options_from(sq, board, pieces);
        if (!opts.empty()) {
            return make_pair(sq, opts[0].second);
        }
    }
    return make_pair(-1, -1);
}

SimResult simulate(const array<char, 32>& start, int first_player, const vector<Move>& moves) {
    array<int, 32> board;
    board.fill(-1);
    vector<Piece> pieces;
    for (int sq = 0; sq < 32; ++sq) {
        if (start[sq] == '.') {
            continue;
        }
        Piece piece;
        piece.color = piece_color(start[sq]);
        piece.king = is_king(start[sq]);
        piece.origin = sq;
        board[sq] = (int)pieces.size();
        pieces.push_back(piece);
    }

    int player = first_player;
    for (const Move& move : moves) {
        int from = move.seq[0];
        if (board[from] == -1) {
            SimResult res;
            res.type = ActionType::add_start_piece;
            res.square = from;
            res.color = player;
            return res;
        }
        int id = board[from];
        if (pieces[id].color != player) {
            return SimResult();
        }

        if (!move.is_jump) {
            int to = move.seq[1];
            int dr = sq_row[to] - sq_row[from];
            int dc = sq_col[to] - sq_col[from];
            if (abs(dr) != 1 || abs(dc) != 1) {
                return SimResult();
            }
            if (!pieces[id].king) {
                if ((player == 0 && dr != 1) || (player == 1 && dr != -1)) {
                    SimResult res;
                    res.type = ActionType::upgrade_origin;
                    res.origin = pieces[id].origin;
                    res.color = player;
                    return res;
                }
            }

            pair<int, int> forced = first_jump_for_player(player, board, pieces);
            if (forced.first != -1) {
                SimResult res;
                res.type = ActionType::need_blocker;
                res.square = forced.second;
                return res;
            }

            if (board[to] != -1) {
                return SimResult();
            }
            board[from] = -1;
            board[to] = id;
            if (!pieces[id].king && promotion_row(player, to)) {
                pieces[id].king = true;
            }
        } else {
            int cur = from;
            bool promoted_now = false;
            for (int step = 1; step < (int)move.seq.size(); ++step) {
                int to = move.seq[step];
                int dr = sq_row[to] - sq_row[cur];
                int dc = sq_col[to] - sq_col[cur];
                if (abs(dr) != 2 || abs(dc) != 2) {
                    return SimResult();
                }
                if (!pieces[id].king) {
                    if ((player == 0 && dr != 2) || (player == 1 && dr != -2)) {
                        SimResult res;
                        res.type = ActionType::upgrade_origin;
                        res.origin = pieces[id].origin;
                        res.color = player;
                        return res;
                    }
                }
                int mr = (sq_row[cur] + sq_row[to]) / 2;
                int mc = (sq_col[cur] + sq_col[to]) / 2;
                int mid = board_id[mr][mc];
                if (board[mid] == -1) {
                    SimResult res;
                    res.type = ActionType::add_captured_piece;
                    res.square = mid;
                    res.color = other_color(player);
                    return res;
                }
                if (pieces[board[mid]].color == player) {
                    return SimResult();
                }
                if (board[to] != -1) {
                    return SimResult();
                }

                board[cur] = -1;
                board[mid] = -1;
                board[to] = id;
                cur = to;

                if (!pieces[id].king && promotion_row(player, cur)) {
                    if (step + 1 < (int)move.seq.size()) {
                        SimResult res;
                        res.type = ActionType::upgrade_origin;
                        res.origin = pieces[id].origin;
                        res.color = player;
                        return res;
                    }
                    pieces[id].king = true;
                    promoted_now = true;
                }
            }
            if (!promoted_now) {
                vector<pair<int, int> > extra = jump_options_from(cur, board, pieces);
                if (!extra.empty()) {
                    SimResult res;
                    res.type = ActionType::need_blocker;
                    res.square = extra[0].second;
                    return res;
                }
            }
        }

        player = other_color(player);
    }

    SimResult res;
    res.type = ActionType::success;
    res.final_board.fill('.');
    for (int sq = 0; sq < 32; ++sq) {
        if (board[sq] == -1) {
            continue;
        }
        const Piece& piece = pieces[board[sq]];
        res.final_board[sq] = piece.king ? king_char(piece.color) : man_char(piece.color);
    }
    return res;
}

string serialize(const array<char, 32>& start) {
    return string(start.begin(), start.end());
}

array<char, 32> answer_start, answer_final;
unordered_set<string> seen;

bool dfs(array<char, 32> start, int first_player, const vector<Move>& moves) {
    string key = serialize(start);
    if (!seen.insert(key).second) {
        return false;
    }

    while (true) {
        SimResult res = simulate(start, first_player, moves);
        if (res.type == ActionType::success) {
            answer_start = start;
            answer_final = res.final_board;
            return true;
        }
        if (res.type == ActionType::fail) {
            return false;
        }
        if (res.type == ActionType::add_start_piece || res.type == ActionType::add_captured_piece) {
            char need = minimal_piece(res.color, res.square);
            if (!insert_piece(start, res.square, need, true)) {
                return false;
            }
            continue;
        }
        if (res.type == ActionType::upgrade_origin) {
            if (!upgrade_origin(start, res.origin, res.color)) {
                return false;
            }
            continue;
        }
        if (res.type == ActionType::need_blocker) {
            for (int color = 0; color < 2; ++color) {
                array<char, 32> next = start;
                char need = minimal_piece(color, res.square);
                if (!insert_piece(next, res.square, need, true)) {
                    continue;
                }
                if (dfs(next, first_player, moves)) {
                    return true;
                }
            }
            return false;
        }
    }
}

Move parse_move(const string& s) {
    Move move;
    move.is_jump = s.find('x') != string::npos;
    int value = 0;
    for (char ch : s) {
        if (isdigit(ch)) {
            value = value * 10 + (ch - '0');
        } else {
            move.seq.push_back(value - 1);
            value = 0;
        }
    }
    move.seq.push_back(value - 1);
    return move;
}

vector<string> render_board(const array<char, 32>& board) {
    vector<string> out(8, string(8, '-'));
    for (int sq = 0; sq < 32; ++sq) {
        out[sq_row[sq]][sq_col[sq]] = board[sq];
    }
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board_id[r][c] == -1) {
                out[r][c] = '-';
            } else if (out[r][c] == '-') {
                out[r][c] = '.';
            }
        }
    }
    return out;
}

void init_board_map() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            board_id[r][c] = -1;
        }
    }
    int id = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if ((r + c) % 2 == 1) {
                board_id[r][c] = id;
                sq_row[id] = r;
                sq_col[id] = c;
                ++id;
            }
        }
    }
}

void solve() {
    init_board_map();

    char first_char;
    int n;
    cin >> first_char >> n;
    vector<Move> moves(n);
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        moves[i] = parse_move(s);
    }

    array<char, 32> start;
    start.fill('.');
    int first_player = (first_char == 'B' ? 0 : 1);
    seen.clear();
    bool ok = dfs(start, first_player, moves);
    (void)ok;

    vector<string> before = render_board(answer_start);
    vector<string> after = render_board(answer_final);
    for (int r = 0; r < 8; ++r) {
        cout << before[r] << ' ' << after[r] << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
