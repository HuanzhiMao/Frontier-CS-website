#include <iostream>
#include <vector>
#include <deque>
using namespace std;

vector<pair<int, int>> moves;
deque<int> baskets[4];

void hanoi(int n, int from, int to, int aux) {
    if (n == 1) {
        int ball = baskets[from].back();
        baskets[from].pop_back();
        baskets[to].push_front(ball);
        moves.push_back({from, to});
        return;
    }
    
    hanoi(n-1, from, aux, to);
    
    int ball = baskets[from].back();
    baskets[from].pop_back();
    baskets[to].push_front(ball);
    moves.push_back({from, to});
    
    hanoi(n-1, aux, to, from);
}

int main() {
    int n;
    cin >> n;
    
    for (int i = 1; i <= n; i++) {
        baskets[1].push_back(i);
    }
    
    hanoi(n, 1, 3, 2);
    
    cout << moves.size() << endl;
    for (auto move : moves) {
        cout << move.first << " " << move.second << endl;
    }
    
    return 0;
}