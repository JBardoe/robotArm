#include <stdio.h>

int depth[] = {0,3,3};

void solve(int numDisks, int start, int away, int end){
    
    if(numDisks == 0){
        move(start, depth[start]++, end, ((depth[end]--)-1));
    }
    else{
        solve(numDisks-1, start, end, away);
        
        move(start, depth[start]++, end, ((depth[end]--)-1));
        
        solve(numDisks-1, away, start, end);
    }
}


int main() {
    solve(2, 0, 1, 2);
}
