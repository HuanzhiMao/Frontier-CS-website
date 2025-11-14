#include <bits/stdc++.h>
using namespace std;

struct Move {
    int x1, y1, x2, y2;
};

vector<Move> moves;
int pyramid[30][30];

bool isAdjacent(int x1, int y1, int x2, int y2) {
    if (x1 == x2 - 1 && y1 == y2 - 1) return true;
    if (x1 == x2 - 1 && y1 == y2) return true;
    if (x1 == x2 && y1 == y2 - 1) return true;
    if (x1 == x2 && y1 == y2 + 1) return true;
    if (x1 == x2 + 1 && y1 == y2) return true;
    if (x1 == x2 + 1 && y1 == y2 + 1) return true;
    return false;
}

void swapBalls(int x1, int y1, int x2, int y2) {
    swap(pyramid[x1][y1], pyramid[x2][y2]);
    moves.push_back({x1, y1, x2, y2});
}

bool needsImprovement() {
    for (int x = 0; x < 29; x++) {
        for (int y = 0; y <= x; y++) {
            if (pyramid[x][y] > pyramid[x+1][y] || pyramid[x][y] > pyramid[x+1][y+1]) {
                return true;
            }
        }
    }
    return false;
}

void bubbleSort() {
    for (int iter = 0; iter < 1000 && needsImprovement() && moves.size() < 9500; iter++) {
        bool improved = false;
        
        for (int x = 0; x < 29; x++) {
            for (int y = 0; y <= x; y++) {
                int current = pyramid[x][y];
                int left = pyramid[x+1][y];
                int right = pyramid[x+1][y+1];
                
                if (current > left) {
                    swapBalls(x, y, x+1, y);
                    improved = true;
                }
                else if (current > right) {
                    swapBalls(x, y, x+1, y+1);
                    improved = true;
                }
            }
        }
        
        // Also try horizontal swaps
        for (int x = 0; x < 30; x++) {
            for (int y = 0; y < x; y++) {
                if (pyramid[x][y] > pyramid[x][y+1]) {
                    swapBalls(x, y, x, y+1);
                    improved = true;
                }
            }
        }
        
        if (!improved) break;
    }
}

int main() {
    for (int x = 0; x < 30; x++) {
        for (int y = 0; y <= x; y++) {
            cin >> pyramid[x][y];
        }
    }
    
    bubbleSort();
    
    cout << moves.size() << "\n";
    for (auto& move : moves) {
        cout << move.x1 << " " << move.y1 << " " << move.x2 << " " << move.y2 << "\n";
    }
    
    return 0;
}