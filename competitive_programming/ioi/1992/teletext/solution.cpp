// IOI 1992 - Problem 2: Teletext
// Pattern matching OCR: decode characters from pixel grid using Hamming distance.
#include <bits/stdc++.h>
using namespace std;

int main() {
    int charW, charH, numChars;
    scanf("%d%d%d", &charW, &charH, &numChars);

    vector<char> charLabel(numChars);
    vector<vector<string>> patterns(numChars, vector<string>(charH));

    for (int k = 0; k < numChars; k++) {
        char label[4];
        scanf(" %c", &label[0]);
        charLabel[k] = label[0];
        for (int r = 0; r < charH; r++) {
            char buf[256];
            scanf("%s", buf);
            patterns[k][r] = buf;
        }
    }

    int imgH, imgW;
    scanf("%d%d", &imgH, &imgW);
    vector<string> image(imgH);
    for (int r = 0; r < imgH; r++) {
        char buf[1024];
        scanf("%s", buf);
        image[r] = buf;
    }

    int textRows = imgH / charH;
    int textCols = imgW / charW;

    for (int tr = 0; tr < textRows; tr++) {
        for (int tc = 0; tc < textCols; tc++) {
            int bestDist = INT_MAX;
            char bestChar = '?';

            for (int k = 0; k < numChars; k++) {
                int dist = 0;
                for (int r = 0; r < charH && dist < bestDist; r++) {
                    for (int c = 0; c < charW; c++) {
                        int ir = tr * charH + r;
                        int ic = tc * charW + c;
                        if (image[ir][ic] != patterns[k][r][c])
                            dist++;
                    }
                }
                if (dist < bestDist) {
                    bestDist = dist;
                    bestChar = charLabel[k];
                    if (dist == 0) break; // exact match
                }
            }
            putchar(bestChar);
        }
        putchar('\n');
    }
    return 0;
}
