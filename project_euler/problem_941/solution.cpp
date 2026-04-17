#include <bits/stdc++.h>
using namespace std;
int main(){
    long long total=0;
    int cnt=0;
    for(int k=1;k<=30;k++){
        int half=(k+1)/2;
        for(int fh=(1<<(half-1));fh<(1<<half);fh++){
            long long val=0;
            vector<int> bits;
            int t=fh;
            for(int i=0;i<half;i++){bits.push_back(t&1);t>>=1;}
            reverse(bits.begin(),bits.end());
            vector<int> full=bits;
            if(k%2==1){
                for(int i=half-2;i>=0;i--) full.push_back(bits[i]);
            } else {
                for(int i=half-1;i>=0;i--) full.push_back(bits[i]);
            }
            if((int)full.size()!=k) continue;
            for(int b:full) val=val*2+b;
            if(val>0&&val<(1LL<<30)){total+=val;cnt++;}
        }
    }
    cout<<total<<endl;
    return 0;
}
