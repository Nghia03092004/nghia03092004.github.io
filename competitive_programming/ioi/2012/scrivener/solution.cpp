#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1000005;
const int LOG = 20;

// Each node in the character tree
struct Node {
    char ch;
    int parent;
    int jump[LOG]; // binary lifting
};

Node nodes[MAXN];
int nodeCount = 0;

// Version info
struct Version {
    int tail;   // index of last character node (-1 if empty)
    int len;    // length of string
};

Version versions[MAXN];
int versionCount = 0;
int opCount = 0; // total operations so far

void Init(){
    // Version 0: empty string
    versions[0] = {-1, 0};
    versionCount = 1;
    opCount = 0;
}

void TypeLetter(char c){
    opCount++;
    int newNode = nodeCount++;
    nodes[newNode].ch = c;
    nodes[newNode].parent = versions[versionCount - 1].tail;

    // Set up binary lifting
    nodes[newNode].jump[0] = nodes[newNode].parent;
    for(int k = 1; k < LOG; k++){
        int prev = nodes[newNode].jump[k-1];
        if(prev == -1) nodes[newNode].jump[k] = -1;
        else nodes[newNode].jump[k] = nodes[prev].jump[k-1];
    }

    versions[versionCount] = {newNode, versions[versionCount - 1].len + 1};
    versionCount++;
}

void UndoCommands(int u){
    opCount++;
    // Go back u operations: the version at time (opCount - u - 1)
    // Wait, opCount is already incremented. The version we want is
    // the one that was current at operation (opCount - u - 1).
    // versions array is indexed by operation number.
    // Before this undo, there were versionCount-1 versions (indices 0..versionCount-2).
    // We want the version that existed u operations before this one.
    // That's version index (versionCount - 1 - u).
    int targetVersion = versionCount - 1 - u;
    versions[versionCount] = versions[targetVersion];
    versionCount++;
}

char GetLetter(int p){
    opCount++;
    int cur = versionCount - 1;
    int tail = versions[cur].tail;
    int len = versions[cur].len;

    // We want position p (0-indexed from start).
    // Tail is position len-1. We need to go back (len - 1 - p) steps.
    int stepsBack = len - 1 - p;
    int node = tail;
    for(int k = LOG - 1; k >= 0; k--){
        if(stepsBack >= (1 << k)){
            node = nodes[node].jump[k];
            stepsBack -= (1 << k);
        }
    }

    // Record this operation as a version (GetLetter is also an operation for undo purposes)
    versions[versionCount] = versions[versionCount - 1];
    versionCount++;

    return nodes[node].ch;
}

int main(){
    Init();

    int Q;
    cin >> Q;
    while(Q--){
        char op;
        cin >> op;
        if(op == 'T'){
            char c;
            cin >> c;
            TypeLetter(c);
        } else if(op == 'U'){
            int u;
            cin >> u;
            UndoCommands(u);
        } else { // 'P'
            int p;
            cin >> p;
            cout << GetLetter(p) << "\n";
        }
    }
    return 0;
}
