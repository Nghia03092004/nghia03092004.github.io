#include <bits/stdc++.h>
using namespace std;
int main(){
    const int LIM=10000;
    map<int,set<int>> pm;
    for(int s=3;s<=8;s++){
        for(int k=1;;k++){
            int v=k*((s-2)*k-(s-4))/2;
            if(v>LIM) break;
            pm[v].insert(s);
        }
    }
    int cnt=0;
    for(auto&[v,st]:pm) if(st.size()>=2) cnt++;
    cout<<cnt<<endl;
    return 0;
}
