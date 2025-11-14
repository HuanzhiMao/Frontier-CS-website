#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y, d;
};

struct Line {
    int x1, y1, x2, y2;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, K;
    cin >> N >> K;
    
    vector<int> a(11);
    for (int i = 1; i <= 10; i++) {
        cin >> a[i];
    }
    
    vector<Point> strawberries(N);
    for (int i = 0; i < N; i++) {
        cin >> strawberries[i].x >> strawberries[i].y;
        strawberries[i].d = 0;
    }
    
    // Assign strawberry values based on requirement
    int idx = 0;
    for (int d = 1; d <= 10; d++) {
        for (int cnt = 0; cnt < a[d] * d; cnt++) {
            if (idx < N) {
                strawberries[idx++].d = d;
            }
        }
    }
    
    vector<Line> cuts;
    
    // Simple strategy: try to separate strawberries by creating regions
    // Use random cuts to create different regions
    srand(42);
    
    for (int cut = 0; cut < min(K, 30); cut++) {
        int x1 = (rand() % 20001) - 10000;
        int y1 = (rand() % 20001) - 10000;
        int x2 = (rand() % 20001) - 10000;
        int y2 = (rand() % 20001) - 10000;
        
        // Make sure the line is not degenerate
        if (x1 == x2 && y1 == y2) {
            x2 += 1;
        }
        
        cuts.push_back({x1, y1, x2, y2});
    }
    
    cout << cuts.size() << "\n";
    for (const auto& cut : cuts) {
        cout << cut.x1 << " " << cut.y1 << " " << cut.x2 << " " << cut.y2 << "\n";
    }
    
    return 0;
}