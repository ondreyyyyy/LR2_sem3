#include <iostream>
#include "set.h"

using namespace std;

int main() {
    int N;
    cin >> N;
    
    Set* positions = createSet();
    int truthCount = 0;
    
    for (int i = 0; i < N; i++) {
        int a, b;
        cin >> a >> b;
        
        if (a >= 0 && b >= 0 && a + b == N - 1) {
            string position = to_string(a) + "," + to_string(b);
            
            if (!setContains(positions, position)) {
                setInsert(positions, position);
                truthCount++;
            }
        }
    }
    
    cout << truthCount << endl;
    
    return 0;
}