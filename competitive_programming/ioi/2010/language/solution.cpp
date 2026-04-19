#include <bits/stdc++.h>
using namespace std;

// IOI 2010 - Language
// This task is heuristic by nature. We use a hashed character n-gram model
// with online updates and smoothed log-likelihood scoring.

namespace {

constexpr int NLANG = 56;
constexpr int UNI_DIM = 1 << 12;
constexpr int BI_DIM = 1 << 15;
constexpr int TRI_DIM = 1 << 16;

constexpr double ALPHA = 0.25;
constexpr double UNI_W = 0.5;
constexpr double BI_W = 1.5;
constexpr double TRI_W = 3.0;
constexpr double PRIOR_W = 0.35;

int unigram[NLANG][UNI_DIM];
int bigram[NLANG][BI_DIM];
int trigram[NLANG][TRI_DIM];
int total_uni[NLANG];
int total_bi[NLANG];
int total_tri[NLANG];
int doc_count[NLANG];

struct SparseCounts {
    vector<pair<int, int>> uni;
    vector<pair<int, int>> bi;
    vector<pair<int, int>> tri;
};

uint32_t mix_bits(uint32_t x) {
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}

int hash1(int a) {
    return int(mix_bits(uint32_t(a)) & (UNI_DIM - 1));
}

int hash2(int a, int b) {
    uint32_t x = uint32_t(a) * 1000003U ^ uint32_t(b) * 9176U ^ 0x9e3779b9U;
    return int(mix_bits(x) & (BI_DIM - 1));
}

int hash3(int a, int b, int c) {
    uint32_t x = uint32_t(a) * 73856093U ^
                 uint32_t(b) * 19349663U ^
                 uint32_t(c) * 83492791U;
    return int(mix_bits(x) & (TRI_DIM - 1));
}

vector<pair<int, int>> compress(vector<int>& ids) {
    sort(ids.begin(), ids.end());
    vector<pair<int, int>> out;
    for (int id : ids) {
        if (out.empty() || out.back().first != id) {
            out.push_back({id, 1});
        } else {
            out.back().second++;
        }
    }
    return out;
}

SparseCounts extract_features(const int* E, int len) {
    SparseCounts feats;

    vector<int> uni_ids;
    uni_ids.reserve(len);
    for (int i = 0; i < len; i++) {
        uni_ids.push_back(hash1(E[i]));
    }
    feats.uni = compress(uni_ids);

    if (len >= 2) {
        vector<int> bi_ids;
        bi_ids.reserve(len - 1);
        for (int i = 0; i + 1 < len; i++) {
            bi_ids.push_back(hash2(E[i], E[i + 1]));
        }
        feats.bi = compress(bi_ids);
    }

    if (len >= 3) {
        vector<int> tri_ids;
        tri_ids.reserve(len - 2);
        for (int i = 0; i + 2 < len; i++) {
            tri_ids.push_back(hash3(E[i], E[i + 1], E[i + 2]));
        }
        feats.tri = compress(tri_ids);
    }

    return feats;
}

double score_language(const SparseCounts& feats, int lang) {
    if (doc_count[lang] == 0) {
        return -1e100;
    }

    double score = PRIOR_W * log(1.0 + doc_count[lang]);

    double uni_denom = log(total_uni[lang] + ALPHA * UNI_DIM);
    for (const auto& [id, cnt] : feats.uni) {
        score += UNI_W * cnt * (log(unigram[lang][id] + ALPHA) - uni_denom);
    }

    if (!feats.bi.empty()) {
        double bi_denom = log(total_bi[lang] + ALPHA * BI_DIM);
        for (const auto& [id, cnt] : feats.bi) {
            score += BI_W * cnt * (log(bigram[lang][id] + ALPHA) - bi_denom);
        }
    }

    if (!feats.tri.empty()) {
        double tri_denom = log(total_tri[lang] + ALPHA * TRI_DIM);
        for (const auto& [id, cnt] : feats.tri) {
            score += TRI_W * cnt * (log(trigram[lang][id] + ALPHA) - tri_denom);
        }
    }

    return score;
}

}  // namespace

int classify(int* E, int len) {
    SparseCounts feats = extract_features(E, len);

    int best_lang = 0;
    double best_score = -1e100;
    bool have_model = false;

    for (int lang = 0; lang < NLANG; lang++) {
        if (doc_count[lang] == 0) {
            continue;
        }
        have_model = true;
        double cur = score_language(feats, lang);
        if (cur > best_score) {
            best_score = cur;
            best_lang = lang;
        }
    }

    return have_model ? best_lang : 0;
}

void learn(int* E, int len, int lang) {
    SparseCounts feats = extract_features(E, len);
    doc_count[lang]++;

    for (const auto& [id, cnt] : feats.uni) {
        unigram[lang][id] += cnt;
        total_uni[lang] += cnt;
    }
    for (const auto& [id, cnt] : feats.bi) {
        bigram[lang][id] += cnt;
        total_bi[lang] += cnt;
    }
    for (const auto& [id, cnt] : feats.tri) {
        trigram[lang][id] += cnt;
        total_tri[lang] += cnt;
    }
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        int len;
        cin >> len;
        vector<int> E(len);
        for (int i = 0; i < len; i++) {
            cin >> E[i];
        }

        int guess = classify(E.data(), len);
        cout << guess << '\n';

        int correct;
        cin >> correct;
        learn(E.data(), len, correct);
    }
    return 0;
}
