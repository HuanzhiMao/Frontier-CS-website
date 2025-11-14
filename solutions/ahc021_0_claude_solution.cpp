#include <bits/stdc++.h>
using namespace std;

const int N = 30;
int pyramid[N][N];
vector<tuple<int, int, int, int>> operations;

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
    operations.push_back({x1, y1, x2, y2});
}

vector<pair<int, int>> getNeighbors(int x, int y) {
    vector<pair<int, int>> neighbors;
    
    if (x > 0 && y > 0) neighbors.push_back({x-1, y-1});
    if (x > 0) neighbors.push_back({x-1, y});
    if (y > 0) neighbors.push_back({x, y-1});
    if (y < x) neighbors.push_back({x, y+1});
    if (x < N-1) neighbors.push_back({x+1, y});
    if (x < N-1 && y < x) neighbors.push_back({x+1, y+1});
    
    return neighbors;
}

void bubbleSort() {
    for (int iter = 0; iter < 50; iter++) {
        for (int x = 0; x < N - 1; x++) {
            for (int y = 0; y <= x; y++) {
                int current = pyramid[x][y];
                int left_child = pyramid[x+1][y];
                int right_child = pyramid[x+1][y+1];
                
                if (current > left_child) {
                    swapBalls(x, y, x+1, y);
                }
                if (pyramid[x][y] > pyramid[x+1][y+1]) {
                    swapBalls(x, y, x+1, y+1);
                }
            }
        }
    }
}

int main() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            cin >> pyramid[i][j];
        }
    }
    
    bubbleSort();
    
    cout << operations.size() << endl;
    for (auto op : operations) {
        cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << " " << get<3>(op) << endl;
    }
    
    return 0;
}