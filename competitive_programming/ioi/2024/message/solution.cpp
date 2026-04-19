#include <bits/stdc++.h>

using namespace std;

vector<bool> send_packet(vector<bool> A);

namespace {

constexpr int kPacketBits = 31;
constexpr int kSafeBits = 16;

}  // namespace

void send_message(vector<bool> M, vector<bool> C) {
    int packet_count = max(kSafeBits, static_cast<int>((M.size() + 15) / 16) + 2);
    vector<vector<bool>> packets(packet_count, vector<bool>(kPacketBits, false));

    vector<int> nxt(kPacketBits, 1);
    for (int bit = 0; bit < kPacketBits; ++bit) {
        if (C[bit]) {
            continue;
        }
        while (C[(bit + nxt[bit]) % kPacketBits]) {
            ++nxt[bit];
        }
        packets[nxt[bit] - 1][bit] = true;
    }

    int pos = 0;
    for (int packet = 0; packet < packet_count; ++packet) {
        for (int bit = 0; bit < kPacketBits; ++bit) {
            if (C[bit] || nxt[bit] > packet) {
                continue;
            }
            packets[packet][bit] = (pos < static_cast<int>(M.size()) ? M[pos]
                                                                      : pos == static_cast<int>(M.size()));
            ++pos;
        }
    }

    for (const auto& packet : packets) {
        send_packet(packet);
    }
}

vector<bool> receive_message(vector<vector<bool>> R) {
    vector<int> nxt(kPacketBits, 0);
    int prefix_packets = min(static_cast<int>(R.size()), kSafeBits);
    for (int bit = 0; bit < kPacketBits; ++bit) {
        for (int packet = 0; packet < prefix_packets; ++packet) {
            if (R[packet][bit]) {
                nxt[bit] = packet + 1;
                break;
            }
        }
    }

    vector<bool> controlled(kPacketBits, true);
    for (int start = 0; start < kPacketBits; ++start) {
        int cycle_len = 0;
        vector<bool> in_cycle(kPacketBits, false);
        for (int bit = (start + nxt[start]) % kPacketBits; !in_cycle[bit];
             bit = (bit + nxt[bit]) % kPacketBits) {
            in_cycle[bit] = true;
            ++cycle_len;
        }
        if (cycle_len == kSafeBits) {
            for (int bit = 0; bit < kPacketBits; ++bit) {
                controlled[bit] = !in_cycle[bit];
            }
            break;
        }
    }

    vector<bool> M;
    for (int packet = 0; packet < static_cast<int>(R.size()); ++packet) {
        for (int bit = 0; bit < kPacketBits; ++bit) {
            if (!controlled[bit] && nxt[bit] <= packet) {
                M.push_back(R[packet][bit]);
            }
        }
    }

    while (!M.empty() && !M.back()) {
        M.pop_back();
    }
    if (!M.empty()) {
        M.pop_back();
    }
    return M;
}
