#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 54: Poker Hands
 *
 * Usage:
 *   Place 'poker.txt' in the same directory and run:
 *       ./solution
 *   Or pipe via stdin:
 *       ./solution < poker.txt
 *
 *   Download poker.txt from: https://projecteuler.net/problem=54
 *   (requires a free Project Euler account)
 *
 * Answer: 376
 */

int card_value(char c) {
    if (c >= '2' && c <= '9') return c - '0';
    if (c == 'T') return 10;
    if (c == 'J') return 11;
    if (c == 'Q') return 12;
    if (c == 'K') return 13;
    if (c == 'A') return 14;
    return 0;
}

// Compute comparison key kappa(H) as a vector for lexicographic comparison
vector<int> evaluate(vector<pair<int,char>>& hand) {
    vector<int> vals;
    set<char> suits;
    map<int,int> freq;
    for (auto& [v, s] : hand) {
        vals.push_back(v);
        suits.insert(s);
        freq[v]++;
    }
    sort(vals.begin(), vals.end());

    bool is_flush = (suits.size() == 1);

    bool is_straight = false;
    int straight_high = 0;
    set<int> unique_vals(vals.begin(), vals.end());
    if (unique_vals.size() == 5) {
        if (vals[4] - vals[0] == 4) {
            is_straight = true;
            straight_high = vals[4];
        }
        if (unique_vals.count(14) && unique_vals.count(2) &&
            unique_vals.count(3) && unique_vals.count(4) && unique_vals.count(5)) {
            is_straight = true;
            straight_high = 5;
        }
    }

    // Sort values by (frequency desc, value desc)
    vector<pair<int,int>> freq_val;
    for (auto& [v, f] : freq) freq_val.push_back({f, v});
    sort(freq_val.begin(), freq_val.end(), [](auto& a, auto& b) {
        if (a.first != b.first) return a.first > b.first;
        return a.second > b.second;
    });

    vector<int> tiebreakers;
    for (auto& [f, v] : freq_val) tiebreakers.push_back(v);

    vector<int> counts;
    for (auto& [f, v] : freq_val) counts.push_back(f);

    int rank = 0;
    if (is_straight && is_flush) {
        rank = (straight_high == 14) ? 9 : 8;
        tiebreakers = {straight_high};
    } else if (counts.size() >= 1 && counts[0] == 4) {
        rank = 7;
    } else if (counts.size() >= 2 && counts[0] == 3 && counts[1] == 2) {
        rank = 6;
    } else if (is_flush) {
        rank = 5;
    } else if (is_straight) {
        rank = 4;
        tiebreakers = {straight_high};
    } else if (counts.size() >= 1 && counts[0] == 3) {
        rank = 3;
    } else if (counts.size() >= 2 && counts[0] == 2 && counts[1] == 2) {
        rank = 2;
    } else if (counts.size() >= 1 && counts[0] == 2) {
        rank = 1;
    } else {
        rank = 0;
    }

    vector<int> result = {rank};
    for (int t : tiebreakers) result.push_back(t);
    return result;
}

int main() {
    ifstream fin("poker.txt");
    if (!fin.is_open()) fin.open("p054_poker.txt");
    istream& in = fin.is_open() ? fin : cin;

    int p1_wins = 0;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        vector<pair<int,char>> h1(5), h2(5);
        string card;
        bool valid = true;
        for (int i = 0; i < 5; i++) {
            if (!(iss >> card) || card.size() < 2) { valid = false; break; }
            h1[i] = {card_value(card[0]), card[1]};
        }
        for (int i = 0; i < 5; i++) {
            if (!(iss >> card) || card.size() < 2) { valid = false; break; }
            h2[i] = {card_value(card[0]), card[1]};
        }
        if (!valid) continue;

        auto s1 = evaluate(h1);
        auto s2 = evaluate(h2);
        if (s1 > s2) p1_wins++;
    }

    cout << p1_wins << endl;
    return 0;
}
