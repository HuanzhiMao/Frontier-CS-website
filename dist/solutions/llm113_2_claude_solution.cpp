#include <iostream>
#include <vector>
#include <deque>
using namespace std;

vector<pair<int, int>> moves;
deque<int> baskets[4];

int getCenter(int basket) {
    int size = baskets[basket].size();
    if (size == 0) return -1;
    return (size - 1) / 2 + (size % 2 == 0 ? 1 : 0) - 1;
}

void moveCenter(int from, int to) {
    int centerIdx = getCenter(from);
    int ball = baskets[from][centerIdx];
    baskets[from].erase(baskets[from].begin() + centerIdx);
    
    int targetCenterIdx = getCenter(to);
    if (baskets[to].empty()) {
        baskets[to].push_back(ball);
    } else {
        baskets[to].insert(baskets[to].begin() + targetCenterIdx + (baskets[to].size() % 2 == 0 ? 1 : 0), ball);
    }
    
    moves.push_back({from, to});
}

void hanoi(int n, int from, int to, int aux) {
    if (n == 1) {
        moveCenter(from, to);
        return;
    }
    
    hanoi(n - 1, from, aux, to);
    moveCenter(from, to);
    hanoi(n - 1, aux, to, from);
}

int main() {
    int N;
    cin >> N;
    
    for (int i = 1; i <= N; i++) {
        baskets[1].push_back(i);
    }
    
    hanoi(N, 1, 3, 2);
    
    cout << moves.size() << endl;
    for (auto move : moves) {
        cout << move.first << " " << move.second << endl;
    }
    
    return 0;
}