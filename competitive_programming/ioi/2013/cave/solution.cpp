#include <bits/stdc++.h>
using namespace std;

// Grader functions:
// int tryCombination(int S[]) - returns first closed door, or -1 if all open
// void answer(int S[], int D[]) - report solution
// S[i] = correct position (0/1) for switch i
// D[i] = which door switch i controls

int tryCombination(int S[]);
void answer(int S[], int D[]);

void exploreCave(int N){
    vector<int> switchPos(N, -1);  // correct position for each switch
    vector<int> switchDoor(N, -1); // which door each switch controls
    vector<bool> determined(N, false); // whether switch i is determined

    for(int door = 0; door < N; door++){
        // Collect undetermined switches
        vector<int> unknown;
        for(int i = 0; i < N; i++){
            if(!determined[i]) unknown.push_back(i);
        }

        // First, determine the "test value" for unknown switches
        // that makes door 'door' close (so we can detect it).
        // Try setting all unknown switches to 0:
        int *S = new int[N];
        for(int i = 0; i < N; i++){
            if(determined[i]){
                S[i] = switchPos[i]; // known correct value
            } else {
                S[i] = 0;
            }
        }
        int result = tryCombination(S);
        int testVal; // the value for unknown switches that closes door 'door'
        if(result == door){
            // All unknowns at 0 -> door is closed
            // So the controlling switch's correct value is 1 (currently at 0 = wrong)
            testVal = 0; // setting unknown to 0 closes the door
        } else {
            // Door is open with all unknowns at 0
            // So correct value is 0 for the controlling switch
            // Set unknowns to 1 to close it
            testVal = 1;
        }

        // Binary search on unknown switches
        int lo = 0, hi = (int)unknown.size() - 1;
        while(lo < hi){
            int mid = (lo + hi) / 2;
            // Set switches unknown[lo..mid] to testVal, rest to 1-testVal
            for(int i = 0; i < N; i++){
                if(determined[i]){
                    S[i] = switchPos[i];
                } else {
                    S[i] = 1 - testVal; // default: opposite of testVal
                }
            }
            for(int i = lo; i <= mid; i++){
                S[unknown[i]] = testVal;
            }

            result = tryCombination(S);
            if(result == door){
                // The controlling switch is in [lo..mid] (with testVal, door closes)
                hi = mid;
            } else {
                // Door is open -> controlling switch NOT in [lo..mid]
                lo = mid + 1;
            }
        }

        // Switch unknown[lo] controls door 'door'
        int sw = unknown[lo];
        switchDoor[sw] = door;
        switchPos[sw] = 1 - testVal; // testVal closes it, so correct = opposite
        determined[sw] = true;

        delete[] S;
    }

    int *S = new int[N], *D = new int[N];
    for(int i = 0; i < N; i++){
        S[i] = switchPos[i];
        D[i] = switchDoor[i];
    }
    answer(S, D);
    delete[] S; delete[] D;
}

int main(){
    int N;
    cin >> N;
    exploreCave(N);
    return 0;
}

int tryCombination(int S[]){ return -1; }
void answer(int S[], int D[]){}
