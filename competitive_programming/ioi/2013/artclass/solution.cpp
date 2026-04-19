#include <bits/stdc++.h>
using namespace std;

int style(int H, int W, int R[500][500], int G[500][500], int B[500][500]){
    // Compute average color
    double avgR = 0, avgG = 0, avgB = 0;
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            avgR += R[i][j];
            avgG += G[i][j];
            avgB += B[i][j];
        }
    }
    int total = H * W;
    avgR /= total; avgG /= total; avgB /= total;

    // Compute color variance
    double varR = 0, varG = 0, varB = 0;
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            varR += (R[i][j] - avgR) * (R[i][j] - avgR);
            varG += (G[i][j] - avgG) * (G[i][j] - avgG);
            varB += (B[i][j] - avgB) * (B[i][j] - avgB);
        }
    }
    varR /= total; varG /= total; varB /= total;
    double totalVar = varR + varG + varB;

    // Compute edge density (average absolute difference with neighbors)
    double edgeSum = 0;
    int edgeCount = 0;
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W - 1; j++){
            edgeSum += abs(R[i][j] - R[i][j+1]) + abs(G[i][j] - G[i][j+1])
                     + abs(B[i][j] - B[i][j+1]);
            edgeCount++;
        }
    }
    for(int i = 0; i < H - 1; i++){
        for(int j = 0; j < W; j++){
            edgeSum += abs(R[i][j] - R[i+1][j]) + abs(G[i][j] - G[i+1][j])
                     + abs(B[i][j] - B[i+1][j]);
            edgeCount++;
        }
    }
    double edgeDensity = edgeSum / edgeCount;

    // Green ratio
    double greenRatio = avgG / (avgR + avgG + avgB + 1);

    // Classification heuristic
    if(totalVar < 1000 && edgeDensity < 15){
        return 1; // Modern art: few colors, uniform regions
    }
    if(edgeDensity > 60){
        return 3; // Impressionist: very textured
    }
    if(greenRatio > 0.38 && totalVar > 2000){
        return 4; // Landscape: lots of green, varied
    }
    if(edgeDensity < 25 && totalVar < 3000){
        return 1;
    }
    if(edgeDensity > 40){
        return 3;
    }
    if(greenRatio > 0.35){
        return 4;
    }
    return 2; // Default: standard painting
}

int main(){
    int H, W;
    cin >> H >> W;
    int R[500][500], G[500][500], B[500][500];
    for(int i = 0; i < H; i++)
        for(int j = 0; j < W; j++)
            cin >> R[i][j] >> G[i][j] >> B[i][j];
    cout << style(H, W, R, G, B) << "\n";
    return 0;
}
