// IOI 2001 - Double Crypt
// Meet-in-the-middle attack on Double AES encryption.
//
// Problem: Given plaintext P and ciphertext C where C = AES(AES(P, k1), k2),
// find keys k1 and k2. Only the leftmost s hex digits of each key are
// relevant (rest are zero), so key space = 16^s (at most 16^5 = 1048576).
//
// Algorithm:
//   Phase 1: For all possible k1, compute mid = AES_encrypt(P, k1),
//            store (mid -> k1) in a hash map.
//   Phase 2: For all possible k2, compute mid = AES_decrypt(C, k2),
//            look up mid in the hash map. If found, output k1 and k2.
//
// Complexity: O(16^s) time and space.
//
// Input format:
//   Line 1: integer s (1 <= s <= 5)
//   Line 2: plaintext P as 32 hex digits
//   Line 3: ciphertext C as 32 hex digits
//
// Output format:
//   Line 1: key k1 as 32 hex digits
//   Line 2: key k2 as 32 hex digits

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <string>
#include <vector>

// ============================================================
// AES-128 Implementation (single-block ECB)
// ============================================================

typedef unsigned char u8;

// AES S-box
static const u8 sbox[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

// AES inverse S-box
static const u8 inv_sbox[256] = {
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

// Round constants for key expansion
static const u8 rcon[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

// Galois field multiplication
static u8 gmul(u8 a, u8 b) {
    u8 p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        bool hi = a & 0x80;
        a <<= 1;
        if (hi) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

// AES state is 4x4 bytes stored in column-major order: state[col][row]
// But we use a flat 16-byte array indexed as state[row + 4*col] for simplicity,
// matching the AES spec where byte index = row + 4*col.

static void sub_bytes(u8 state[16]) {
    for (int i = 0; i < 16; i++)
        state[i] = sbox[state[i]];
}

static void inv_sub_bytes(u8 state[16]) {
    for (int i = 0; i < 16; i++)
        state[i] = inv_sbox[state[i]];
}

// ShiftRows: row r is shifted left by r positions.
// state layout: state[row + 4*col], so row i has indices i, i+4, i+8, i+12.
static void shift_rows(u8 state[16]) {
    u8 t;
    // Row 1: shift left by 1
    t = state[1]; state[1] = state[5]; state[5] = state[9]; state[9] = state[13]; state[13] = t;
    // Row 2: shift left by 2
    t = state[2]; state[2] = state[10]; state[10] = t;
    t = state[6]; state[6] = state[14]; state[14] = t;
    // Row 3: shift left by 3 (= shift right by 1)
    t = state[15]; state[15] = state[11]; state[11] = state[7]; state[7] = state[3]; state[3] = t;
}

static void inv_shift_rows(u8 state[16]) {
    u8 t;
    // Row 1: shift right by 1
    t = state[13]; state[13] = state[9]; state[9] = state[5]; state[5] = state[1]; state[1] = t;
    // Row 2: shift right by 2
    t = state[2]; state[2] = state[10]; state[10] = t;
    t = state[6]; state[6] = state[14]; state[14] = t;
    // Row 3: shift right by 3 (= shift left by 1)
    t = state[3]; state[3] = state[7]; state[7] = state[11]; state[11] = state[15]; state[15] = t;
}

// MixColumns: each column is multiplied by a fixed matrix in GF(2^8).
static void mix_columns(u8 state[16]) {
    for (int c = 0; c < 4; c++) {
        int base = 4 * c;
        u8 a0 = state[base], a1 = state[base+1], a2 = state[base+2], a3 = state[base+3];
        state[base]   = gmul(a0,2) ^ gmul(a1,3) ^ a2 ^ a3;
        state[base+1] = a0 ^ gmul(a1,2) ^ gmul(a2,3) ^ a3;
        state[base+2] = a0 ^ a1 ^ gmul(a2,2) ^ gmul(a3,3);
        state[base+3] = gmul(a0,3) ^ a1 ^ a2 ^ gmul(a3,2);
    }
}

static void inv_mix_columns(u8 state[16]) {
    for (int c = 0; c < 4; c++) {
        int base = 4 * c;
        u8 a0 = state[base], a1 = state[base+1], a2 = state[base+2], a3 = state[base+3];
        state[base]   = gmul(a0,0x0e) ^ gmul(a1,0x0b) ^ gmul(a2,0x0d) ^ gmul(a3,0x09);
        state[base+1] = gmul(a0,0x09) ^ gmul(a1,0x0e) ^ gmul(a2,0x0b) ^ gmul(a3,0x0d);
        state[base+2] = gmul(a0,0x0d) ^ gmul(a1,0x09) ^ gmul(a2,0x0e) ^ gmul(a3,0x0b);
        state[base+3] = gmul(a0,0x0b) ^ gmul(a1,0x0d) ^ gmul(a2,0x09) ^ gmul(a3,0x0e);
    }
}

static void add_round_key(u8 state[16], const u8 rk[16]) {
    for (int i = 0; i < 16; i++)
        state[i] ^= rk[i];
}

// Key expansion: 128-bit key -> 11 round keys (each 16 bytes)
static void key_expansion(const u8 key[16], u8 round_keys[11][16]) {
    // Copy key into first round key
    // AES key schedule works on 32-bit words. Key = W[0..3], expand to W[0..43].
    u8 W[176]; // 44 words * 4 bytes
    memcpy(W, key, 16);
    for (int i = 4; i < 44; i++) {
        u8 temp[4];
        memcpy(temp, W + 4*(i-1), 4);
        if (i % 4 == 0) {
            // RotWord
            u8 t = temp[0];
            temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = t;
            // SubWord
            for (int j = 0; j < 4; j++) temp[j] = sbox[temp[j]];
            // XOR Rcon
            temp[0] ^= rcon[i/4];
        }
        for (int j = 0; j < 4; j++)
            W[4*i + j] = W[4*(i-4) + j] ^ temp[j];
    }
    // Copy into round_keys array
    for (int r = 0; r < 11; r++)
        memcpy(round_keys[r], W + 16*r, 16);
}

// AES-128 encrypt a single 16-byte block
static void aes_encrypt(const u8 input[16], const u8 key[16], u8 output[16]) {
    u8 rk[11][16];
    key_expansion(key, rk);

    u8 state[16];
    memcpy(state, input, 16);

    add_round_key(state, rk[0]);

    for (int round = 1; round <= 9; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, rk[round]);
    }

    // Final round (no MixColumns)
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, rk[10]);

    memcpy(output, state, 16);
}

// AES-128 decrypt a single 16-byte block
static void aes_decrypt(const u8 input[16], const u8 key[16], u8 output[16]) {
    u8 rk[11][16];
    key_expansion(key, rk);

    u8 state[16];
    memcpy(state, input, 16);

    add_round_key(state, rk[10]);

    for (int round = 9; round >= 1; round--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, rk[round]);
        inv_mix_columns(state);
    }

    // Final inverse round (no InvMixColumns)
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, rk[0]);

    memcpy(output, state, 16);
}

// ============================================================
// Utility: hex string <-> byte array conversion
// ============================================================

static void hex_to_bytes(const char* hex, u8 bytes[16]) {
    for (int i = 0; i < 16; i++) {
        unsigned int val;
        sscanf(hex + 2*i, "%2x", &val);
        bytes[i] = (u8)val;
    }
}

static void bytes_to_hex(const u8 bytes[16], char hex[33]) {
    for (int i = 0; i < 16; i++)
        sprintf(hex + 2*i, "%02X", bytes[i]);
    hex[32] = '\0';
}

// Build a key byte array from a key index and s.
// The leftmost s hex digits encode the key; the rest are zero.
// Hex digit order: the 32-char hex string has hex[0] as the most significant nibble
// of byte[0]. So "leftmost 4*s bits" = the first s hex digits = the high nibbles
// of the first ceil(s/2) bytes.
static void key_from_index(int idx, int s, u8 key[16]) {
    memset(key, 0, 16);
    // idx encodes s hex digits. Digit 0 is the most significant (leftmost).
    // Place them into the key bytes starting from byte 0.
    for (int d = s - 1; d >= 0; d--) {
        int nibble = idx & 0xF;
        idx >>= 4;
        int byte_pos = d / 2;
        if (d % 2 == 0) {
            // High nibble of byte_pos
            key[byte_pos] |= (nibble << 4);
        } else {
            // Low nibble of byte_pos
            key[byte_pos] |= nibble;
        }
    }
}

// ============================================================
// Meet-in-the-middle attack
// ============================================================

// We use a hash map from 128-bit intermediate value to key index.
// For the hash map key, we use a std::string of 16 bytes (the raw block).

int main() {
    int s;
    char hex_p[64], hex_c[64];

    if (scanf("%d", &s) != 1) {
        fprintf(stderr, "Failed to read s\n");
        return 1;
    }
    scanf("%s", hex_p);
    scanf("%s", hex_c);

    u8 plaintext[16], ciphertext[16];
    hex_to_bytes(hex_p, plaintext);
    hex_to_bytes(hex_c, ciphertext);

    int key_space = 1;
    for (int i = 0; i < s; i++) key_space *= 16; // 16^s

    // Phase 1: Encrypt plaintext with all possible k1, store intermediate -> k1_index
    std::unordered_map<std::string, int> table;
    table.reserve(key_space * 2);

    u8 key[16], mid[16];

    fprintf(stderr, "Phase 1: encrypting with %d possible k1 values...\n", key_space);
    for (int k1 = 0; k1 < key_space; k1++) {
        key_from_index(k1, s, key);
        aes_encrypt(plaintext, key, mid);
        std::string mid_str((char*)mid, 16);
        table[mid_str] = k1;
    }

    // Phase 2: Decrypt ciphertext with all possible k2, look for match
    fprintf(stderr, "Phase 2: decrypting with %d possible k2 values...\n", key_space);
    for (int k2 = 0; k2 < key_space; k2++) {
        key_from_index(k2, s, key);
        aes_decrypt(ciphertext, key, mid);
        std::string mid_str((char*)mid, 16);
        auto it = table.find(mid_str);
        if (it != table.end()) {
            int k1 = it->second;

            // Verify: encrypt P with k1, then encrypt result with k2
            u8 key1[16], key2[16], check1[16], check2[16];
            key_from_index(k1, s, key1);
            key_from_index(k2, s, key2);
            aes_encrypt(plaintext, key1, check1);
            aes_encrypt(check1, key2, check2);

            if (memcmp(check2, ciphertext, 16) == 0) {
                char hex_k1[33], hex_k2[33];
                bytes_to_hex(key1, hex_k1);
                bytes_to_hex(key2, hex_k2);
                printf("%s\n", hex_k1);
                printf("%s\n", hex_k2);
                fprintf(stderr, "Found keys: k1=%s k2=%s\n", hex_k1, hex_k2);
                return 0;
            }
        }
    }

    fprintf(stderr, "No solution found.\n");
    return 1;
}
