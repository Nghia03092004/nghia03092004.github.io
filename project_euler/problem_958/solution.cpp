#include <bits/stdc++.h>
using namespace std;
int main(){
    // Powers of 2 give unique binary representation
    // 500000 < 2^20, so exactly 1 subset
    int target=500000;
    int bits=0;
    while((1<<bits)<=target) bits++;
    if(target < (1<<20)){
        cout<<1<<endl;
    } else {
        cout<<0<<endl;
    }
    return 0;
}
