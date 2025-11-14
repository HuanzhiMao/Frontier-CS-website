#include <iostream>
#include <vector>
#include <deque>

using namespace std;

vector<pair<int, int>> moves;
deque<int> baskets[4];

int getCenter(int basket) {
    int size = baskets[basket].size();
    if (size == 0) return -1;
    return (size - 1) / 2;
}

void moveBall(int from, int to) {
    int centerIdx = getCenter(from);
    int ball = baskets[from][centerIdx];
    baskets[from].erase(baskets[from].begin() + centerIdx);
    
    int targetIdx = getCenter(to);
    if (baskets[to].empty()) {
        baskets[to].push_back(ball);
    } else {
        baskets[to].insert(baskets[to].begin() + targetIdx + 1, ball);
    }
    
    moves.push_back({from, to});
}

void hanoi(int n, int from, int to, int aux) {
    if (n == 1) {
        moveBall(from, to);
        return;
    }
    
    hanoi(n - 1, from, aux, to);
    moveBall(from, to);
    hanoi(n - 1, aux, to, from);
}

int main() {
    int n;
    cin >> n;
    
    for (int i = 1; i <= n; i++) {
        baskets[1].push_back(i);
    }
    
    hanoi(n, 1, 3, 2);
    
    cout << moves.size() << endl;
    for (auto& move : moves) {
        cout << move.first << " " << move.second << endl;
    }
    
    return 0;
}